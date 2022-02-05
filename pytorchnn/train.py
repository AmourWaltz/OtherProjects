""" This script is modified based on the word language model example in PyTorch:
    https://github.com/pytorch/examples/tree/master/word_language_model
    An example of model training and N-best rescoring can be found here:
    egs/swbd/s5c/local/pytorchnn/run_nnlm.sh
"""

from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import argparse
import time
import math
import random
import torch
import torch.nn as nn
import torch.optim as optim
import os

import data
import model
from utils import create_exp_dir, get_logger

parser = argparse.ArgumentParser(description="Train and evaluate a neural "
                                 "language model with PyTorch.")
# Model options
parser.add_argument('--data', type=str, default='./data/pytorchnn',
                    help='location of the data corpus')
parser.add_argument('--model', type=str, default='LSTM',
                    help='type of model architecture. can be RNN_TANH, '
                    'RNN_RELU, LSTM, GRU or Transformer.')
parser.add_argument('--emsize', type=int, default=200,
                    help='size of word embeddings')
parser.add_argument('--nhid', type=int, default=200,
                    help='number of hidden units per layer')
parser.add_argument('--nlayers', type=int, default=2,
                    help='number of layers')
parser.add_argument('--nhead', type=int, default=2,
                    help='the number of heads in the encoder/decoder of the '
                    'transformer model')
parser.add_argument('--emsize_l', type=int, default=200,
                    help='size of word embeddings')
parser.add_argument('--nhid_l', type=int, default=200,
                    help='number of hidden units per layer')
parser.add_argument('--nlayers_l', type=int, default=2,
                    help='number of layers')
# XBY 1.25: uncertainty type selection
parser.add_argument('--model_var', type=str, default='none',
                    help='model variant type: [none]')

# Training options
parser.add_argument('--lr', type=float, default=0.1,
                    help='initial learning rate')
parser.add_argument('--batch-size', type=int, default=20, metavar='N',
                    help='batch size')
parser.add_argument('--epochs', type=int, default=20,
                    help='upper epoch limit')
parser.add_argument('--seq_len', type=int, default=35,
                    help='sequence length limit')
parser.add_argument('--clip', type=float, default=0.25,
                    help='gradient clipping')
parser.add_argument('--dropout', type=float, default=0.2,
                    help='dropout applied to layers')
parser.add_argument('--tied', action='store_true',
                    help='tie the word embedding and softmax weights')
parser.add_argument('--optimizer', type=str, default='SGD',
                    help='type of optimizer')
parser.add_argument('--log-interval', type=int, default=200, metavar='N',
                    help='report interval')

# Device options
parser.add_argument('--cuda', action='store_true', help='use CUDA')
parser.add_argument('--save', type=str, default='model.pt',
                    help='path to save the final model')
parser.add_argument('--seed', type=int, default=1111,
                    help='random seed')

# XBY 2.19: saving and loading directory
parser.add_argument('--resume', type=str,  default='',
                    help='path of model to resume')
parser.add_argument('--debug', action='store_true',
                    help='run in debug mode')
parser.add_argument('--work_dir', default='TFM', type=str,
                    help='experiment directory.')
parser.add_argument('--prior', default="False", type=str,
                    help='using pretrain_priored mean as prior')
parser.add_argument('--prior_path', default='steps/pytorchnn/prior', type=str,
                    help='path of prior model')
parser.add_argument('--prior2_path', default='steps/pytorchnn/prior/transformer2/', type=str,
                    help='path of prior model for mean weight in KL calculation')

args = parser.parse_args()
params = vars(args)

# build the directory of model to resume
#if not args.resume:
#    args.work_dir = os.path.join(args.work_dir, time.strftime("%Y%m%d-%H%M%S"))
#    logging = create_exp_dir(args.work_dir,
#        scripts_to_save=['train.py', 'model.py'], debug=args.debug)
#    args.save = os.path.join(args.work_dir, args.save)
#else:
#    args.work_dir = os.path.join(*args.resume.split('/')[:-1])
#    logging = get_logger(os.path.join(args.work_dir, 'log-resume.txt'),
#                         log_=not args.debug)
#    args.save = os.path.join(args.work_dir, 'model-resume.pt')

#import pdb; pdb.set_trace()

# Set the random seed for reproducibility
random.seed(args.seed)
torch.manual_seed(args.seed)
if torch.cuda.is_available():
    if not args.cuda:
        print('WARNING: You have a CUDA device, so you should probably run '
              'with --cuda')
    else:
        torch.cuda.manual_seed_all(args.seed)

print('Configurations')
for arg, p in params.items():
    print(arg, p)

device = torch.device("cuda" if args.cuda else "cpu")

#############################
# Load data
#############################
corpus = data.Corpus(args.data)

print("train set:", len(corpus.train))
print("valid set:", len(corpus.valid))
print("test set:", len(corpus.test))
print("num tokens:", len(corpus.dictionary))

def batchify(data, bsz, random_start_idx=False):
    # Work out how cleanly we can divide the dataset into bsz parts.
    nbatch = data.size(0) // bsz
    # Shuffle data
    if random_start_idx:
        start_idx = random.randint(0, data.size(0) % bsz - 1)
    else:
        start_idx = 0
    # Trim off any extra elements that wouldn't cleanly fit (remainders).
    data = data.narrow(0, start_idx, nbatch * bsz)
    # Evenly divide the data across the bsz batches
    data = data.view(bsz, -1).t().contiguous()
    return data.to(device)


eval_batch_size = 20
train_data = batchify(corpus.train, args.batch_size)
val_data = batchify(corpus.valid, eval_batch_size)
test_data = batchify(corpus.test, eval_batch_size)

#############################
# Build the model
#############################
ntokens = len(corpus.dictionary)

# XBY:2.19: selection for uncertainty type.
if args.model == 'Transformer':
    # The activation function can be 'relu' (default) or 'gelu'
    if args.model_var == 'none':
        model = model.TransformerModel(ntokens, args.emsize, args.nhead, args.nhid,
                                       args.nlayers, args.dropout, "gelu", args.tied).to(device)
        pass
    elif args.model_var == 'XL':
        model = model.TransformerXLModel(ntokens, args.emsize, args.nhead, args.nhid,
                                       args.nlayers, )
elif args.model == 'LSTM':
    if args.model_var == 'none':
        model = model.RNNModel(args.model, ntokens, args.emsize, args.nhid,
                               args.nlayers, args.dropout, args.tied).to(device)
    pass
else:
    if args.model_var == 'none':
        model = model.LSTMTransformerModel2(ntokens, args.emsize, args.nhead, args.nhid,
                                            args.nlayers, args.emsize_l, args.nhid_l, args.dropout, args.tied).to(device)
    pass
pass

total_params = sum(x.data.nelement() for x in model.parameters())
print('Args: {}'.format(args))
print('Model total parameters: {}'.format(total_params))
if args.model == 'Transformer':
    print(str(model.transformerlayers))
else:
    print(str(model.rnn))
criterion = nn.CrossEntropyLoss()

###############################################################################
# Load the model
###############################################################################

if args.prior == "True":
    #prior, _, _ = torch.load(os.path.join(args.prior_path, 'model.pt'), map_location=torch.device('cpu'))
    with open(os.path.join(args.prior_path, 'model.pt'), 'rb') as f:
        prior_dict = torch.load(f, map_location=lambda storage, loc: storage)
    pass
    #print(prior.state_dict().keys())
    model_dict = model.state_dict()
    prior_dict =  {k: v for k, v in prior_dict.items() if k in model_dict}

    #for k, v in prior_dict.items():
    #    if k in model_dict:
    #        prior_dict = {k:a v}
    #        #print("in: ", k)
    #    else:
    #        print("out: ", k)
    #    pass
    #pass
    model_dict.update(prior_dict)
    model.load_state_dict(model_dict)
pass

#############################
# Training part
#############################

def repackage_hidden(h):
    """Wraps hidden states in new Tensors, to detach them from their history."""
    if isinstance(h, torch.Tensor):
        return h.detach()
    return tuple(repackage_hidden(v) for v in h)


# Divide the source data into chunks of length args.seq_len.
def get_batch(source, i):
    seq_len = min(args.seq_len, len(source) - 1 - i)
    data = source[i: i + seq_len]
    target = source[i + 1: i + 1 + seq_len].view(-1)
    return data, target


def train():
    # Turn on training model which enables dropout.
    model.train()
    total_loss = 0.
    start_time = time.time()
    if args.model != 'Transformer':
        hidden = model.init_hidden(args.batch_size)
    for batch, i in enumerate(range(0, train_data.size(0) - 1, args.seq_len)):
        data, targets = get_batch(train_data, i)
        optimizer.zero_grad()
#        forward_start = time.time()
        if args.model == 'Transformer':
            output = model(data)
        else:
            # Starting each batch, the hidden state is detached from how it was
            # previously produced. Otherwise, the model would try
            # backpropagating all the way to start of the dataset.
            hidden = repackage_hidden(hidden)
            output, hidden = model(data, hidden)


		# XBY 2.21: mle-maximum likelihook estimation loss
        mle_loss = criterion(output.view(-1, ntokens), targets)

        loss = mle_loss
        loss.backward()

        # 'clip_grad_norm' helps prevent the exploding gradient problem.
        torch.nn.utils.clip_grad_norm_(model.parameters(), args.clip)
        optimizer.step()

        total_loss += loss.item()
        if batch % args.log_interval == 0 and batch > 0:
            cur_loss = total_loss / args.log_interval
            elapsed = time.time() - start_time
            print('| epoch {:3d} | {:5d}/{:5d} batches | lr {:02.3f} | '
                  'ms/batch {:5.2f} | loss {:5.2f} | ppl {:8.2f}'.format(
                      epoch, batch, len(train_data) // args.seq_len, lr,
                      elapsed * 1000 / args.log_interval, cur_loss,
                      math.exp(cur_loss)))
            #model_dict = model.state_dict()
#            forward_total = 0.
#            backward_total = 0.
            total_loss = 0.
            start_time = time.time()


def evaluate(source):
    # Turn on evaluation mode which disables dropout.
    model.eval()
    total_loss = 0.
    if args.model != 'Transformer':
        hidden = model.init_hidden(eval_batch_size)
    # Speed up evaluation with torch.no_grad()
    with torch.no_grad():
        for i in range(0, source.size(0) - 1, args.seq_len):
            data, targets = get_batch(source, i)
            if args.model == 'Transformer':
                output = model(data)
            else:
                output, hidden = model(data, hidden)
                hidden = repackage_hidden(hidden)
            loss = criterion(output.view(-1, ntokens), targets)
            total_loss += len(data) * loss.item()
    return total_loss / (len(source) - 1)


#############################
# Train the model
#############################
lr = args.lr
best_val_loss = None
optimizer = optim.SGD(model.parameters(), lr=args.lr, momentum=0.9,
                      weight_decay=1e-5)
counter = 0
print("Start training")
try:
    for epoch in range(1, args.epochs + 1):
        epoch_start_time = time.time()
        train()
        val_loss = evaluate(val_data)
        print('-' * 89)
        print('| end of epoch {:3d} | time: {:5.2f}s | valid loss {:5.2f} | '
              'valid ppl {:8.2f}'.format(epoch, (time.time() - epoch_start_time),
                                         val_loss, math.exp(val_loss)))
        print('-' * 89)

        model_dict = model.state_dict()

        # Save the model if validation loss is the best we've seen so far.
        # Saving state_dict is preferable.
        if not best_val_loss or val_loss < best_val_loss:
            with open(args.save, 'wb') as f:
                torch.save(model.state_dict(), f)
            best_val_loss = val_loss
        else:
            lr /= 2.
            optimizer = optim.SGD(model.parameters(), lr=lr, momentum=0.9,
                                  weight_decay=1e-5)
            with open(args.save, 'rb') as f:
                model.load_state_dict(torch.load(f, map_location=lambda storage, loc: storage))
            counter += 1

        # Early stopping
        if counter == 8:
            break
except KeyboardInterrupt:
    print('-' * 89)
    print('Exiting from training early')

# Load the best saved model.
with open(args.save, 'rb') as f:
    model.load_state_dict(torch.load(f, map_location=lambda storage, loc: storage))
#    if args.model in ['RNN_TANH', 'RNN_RELU', 'LSTM', 'GRU']:
#        model.rnn.flatten_parameters()

#with open("exp/pytorch-Transformer-emb512_hid4096_nly6-ami+fisher-0.2-Gaussian-GP0-preFalse-nosoftmax/model.pt", 'rb') as f:
#    model.load_state_dict(torch.load(f, map_location=lambda storage, loc: storage))

model_dict = model.state_dict()

# Run on test data.
test_loss = evaluate(test_data)
print('=' * 89)
print('| End of training | test loss {:5.2f} | test ppl {:8.2f}'.format(
      test_loss, math.exp(test_loss)))
print('=' * 89)
