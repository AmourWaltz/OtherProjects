from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import os
import argparse
from collections import defaultdict
import math
from xml.dom.expatbuilder import parseString

import torch
import torch.nn as nn


#############################
# Evaluation settings
#############################
parser = argparse.ArgumentParser(description="Evaluate the PPL on"
                                    "the test set with a PyTorch trained "
                                    "neural language model.")
parser.add_argument('--inpfile', type=str, required=True,
                    help="Input file of evaluation set")
parser.add_argument('--outfile', type=str, required=True,
                    help="Output file of evaluation set")
parser.add_argument('--vocabulary', type=str, required=True,
                    help="Vocabulary used for training")
parser.add_argument('--cuda', action='store_true', help='use CUDA')
parser.add_argument('--model_dir', type=str, required=True,
                    help="Path to a pretrained neural model.")
parser.add_argument('--model', type=str, default='LSTM',
                    help='Network type. can be RNN, LSTM or Transformer.')
parser.add_argument('--emsize', type=int, default=1024,
                    help='size of word embeddings')
parser.add_argument('--nhid', type=int, default=1024,
                    help='number of hidden units per layer')
parser.add_argument('--nlayers', type=int, default=2,
                    help='number of layers')
parser.add_argument('--nhead', type=int, default=8,
                    help='the number of heads in the encoder/decoder of the '
                    'transformer model')
parser.add_argument('--model_var', type=str, default='none',
                    help='model variants type: [none] | Transformer: XL | LSTM: Bi-LSTM')
parser.add_argument('--cross_utt', type=int, default=0,
                    help='Using cross-utterance representation or not')
parser.add_argument('--seq_len', type=int, default=128,
                    help='sequence length')
parser.add_argument('--nnlm_itdir', type=str, required=True,
                    help="Path to a pretrained neural language model for interpolation")
parser.add_argument('--nnlm_weight', type=float, default=0.8,
                    help="Interpolation weight for the NNLM")

args = parser.parse_args()
print(args)
assert os.path.exists(args.inpfile), "Eval set path does not exists."
assert os.path.exists(args.vocabulary), "Vocabulary path does not exists."
assert os.path.exists(args.model_dir), "Model path does not exists."

device = torch.device("cuda" if args.cuda else "cpu")


def load_data(path):
    r"""Read evaluation set by each sentence.

    Assume the input file format is as following:
        everybody has a master is out here
        oh yeah
        well it seems like it
        ...

    Args:
        path (str): A file of eval set with the above format.

    Returns:
        The eval set represented by a dictionary. The key is a sentence.
    """
    eval_set = []
    with open(path, 'r', encoding='utf-8') as f:
        for line in f:
            line = line.strip()
            eval_set.append(line)
            pass
        pass

    return eval_set


def read_vocab(path):
    r"""Read vocabulary.

    Args:
        path (str): A file with a word and its integer index per line.

    Returns:
        A vocabulary represented by a dictionary from string to int (starting
        from 0).
    """
    word2idx = {}
    idx2word = []
    with open(path, 'r', encoding='utf-8') as f:
        for line in f:
            word = line.split()
            assert len(word) == 2
            word = word[0]
            if word not in word2idx:
                idx2word.append(word)
                word2idx[word] = len(idx2word) - 1
                pass
            pass
        pass

    return word2idx


def get_input_and_target(sent, vocab, cross_utt=False, seq_len=128):
    r"""Convert a sentence to lists of integers, with input and target separately.

    Args:
        sent:       Sentence, with words separated by spaces, e.g. 'hello there'
        vocab:      A dictionary from string to int, e.g. {'<s>':0, 'hello':1,
                    'there':2, 'apple':3, ...}
        cross_utt:  Using cross-utterance representation or not.
        seq_len:    Length of history context.

    Returns:
        A pair of lists, one with the integerized input sequence, one with the
        integerized output/target sequence: in this case ([0, 1, 2], [1 2 0]),
        because the input sequence has '<s>' added at the start and the output
        sequence has '<s>' added at the end.
        Words that are not in the vocabulary will be converted to '<unk>', which
        is expected to be in the vocabulary if there are out-of-vocabulary words.
        Number of out-of-vocabulary word.
    """
    input_string = '<s> ' + sent
    output_string = sent + ' <s>'

    oov_num = 0
    input_ids, output_ids = [], []

    for word in input_string.split():
        try:
            input_ids.append(vocab[word]) # word.lower()
            pass
        except KeyError:
            # Count the number of out-of-vocabulary word
            oov_num += 1
            input_ids.append(vocab['<unk>'])
            pass
        pass
    
    for word in output_string.split():
        try:
            output_ids.append(vocab[word]) # word.lower()
        except KeyError:
            output_ids.append(vocab['<unk>'])
            pass
        pass

    #############################
    # Using cross-utterance or not
    #############################
    if cross_utt == 1 and os.path.exists(os.path.join(args.outfile, 'context_id.txt')):
        with open(os.path.join(args.outfile, 'context_id.txt'), "r") as f:
            for line in f:
                line = line.split()
                line = list(map(int, line))
                input_ids = line + input_ids
                if len(input_ids) >= seq_len:
                    break
                pass
            pass
        pass

    return input_ids, output_ids, oov_num


def compute_sentence_ppl(nnlm_1, criterion, ntokens, input, target, model_type='LSTM', seq_len=128, 
                        nnlm_2=None, weight=0.0, cross_utt=False, hid_1=None, hid_2=None):
    r"""Compute neural language model score of a sentence.

    Args:
        nnlm_1:     The main neural language model.
        criterion:  Training criterion of a neural language model, e.g. cross entropy.
        ntokens:    Vocabulary size.
        inp/tgt:    A pair of lists, integerized input/target sequence.
        model_type: Model type, e.g. LSTM or Transformer or others.
        seq_len:    Length of history context
        nnlm_2:     The interpolated neural language model.
        weight:     Weight of interpolated neural language model.
        cross_utt:  Using cross-utterance representation or not.
        model_var:  Using the variants of standard neural language model or not.
        hidden:     Initial hidden state for getting the score of the input
                    sentence with a recurrent-typed neural language model
                    (optional).

    Returns:
        The score (negative log-likelihood) of the input sequence from a neural
        language model. If the model is recurrent-typed, the function has an
        extra output: the last hidden state after computing the score of the
        input sequence.
    """

    length = len(target)
    input = torch.LongTensor(input).view(-1, 1).contiguous().to(device)
    target = torch.LongTensor(target).view(-1).contiguous().to(device)

    # # Bi-LSTM: A backward LSTM as main language model and interpolated with a forward standard LSTM model.
    if args.model_var == 'bid':
        target_r = torch.flip(input.squeeze(-1), [0])
        input_r = torch.flip(target.unsqueeze(-1), [0])
        pass

    with torch.no_grad():
        # Setp 1: Compute the probability of the main LM.
        # Noted that the input of the backward LSTM LM (model_var == 'bid') is the reversed word sequence input_r.
        if model_type == 'Transformer':
            if args.model_var == 'none':
                output_1 = nnlm_1(input)
                pass
            pass
        else:
            if args.model_var == 'bid':
                output_1, hid_1 = nnlm_1(input_r, hid_1)
                pass
            else:
                output_1, hid_1 = nnlm_1(input[-length:], hid_1)
                pass
            pass

        # Step 2: Compute the probability of the interpolated LM if required.
        # loss_1 represents the score of the main LM and loss_2 represents the score of interpolated LM.
        if 0 < weight <= 1:
            # For backward LSTM LMs as the main LM, 
            # we prefer to compute the interpolated sentence score only combined with the forward LSTM LM.
            # Specifically, the loss_1 needs to be computed using the reversed backward sequence target_r. 
            if args.model_var == 'bid':
                output_2, hid_2 = nnlm_2(input[-length:], hid_2)
                loss_1 = criterion(output_1.view(-1, ntokens), target_r[:length])
                loss_2 = criterion(output_2.view(-1, ntokens), target)
                loss = weight * loss_1 + (1. - weight) * loss_2
                pass
            else:
                output_2, hid_2 = nnlm_2(input[-length:], hid_2)
                # Interpolated in probability domain.
                output = (1. - weight) * output_1[-length:] + weight * output_2
                loss = criterion(output.view(-1, ntokens), target)

                # Interpolated in log domain. 
                # loss_1 = criterion(output_1[-length:].view(-1, ntokens), target)
                # loss_2 = criterion(output_2.view(-1, ntokens), target)
                # loss = weight * loss_1 + (1. - weight) * loss_2
                pass
            pass
        else:
            output = output_1[-length:]
            loss = criterion(output.view(-1, ntokens), target)
            hid_2 = None
            pass
        pass

    sent_ppl = (length) * loss.item()
    print("ppl:", sent_ppl)

    return sent_ppl, hid_1, hid_2


def compute_ppl(eval_set, nnlm_1, criterion, ntokens, vocab, model_type, seq_len, 
                nnlm_2=None, weight=0.0, cross_utt=0, model_var='none'):
    r"""Compute sentence scores of evaluation set from a neural language model.

    Args:
        eval_set:   The sentences represented by a dictionary.
        nnlm_1:     The main neural language model.
        criterion:  Training criterion of a neural language model, e.g. cross entropy.
        ntokens:    Vocabulary size.
        model_type: Model type, e.g. LSTM or Transformer or others.
        seq_len:    Length of history context.
        nnlm_2:     The interpolated neural language model.
        weight:     Weight of interpolated neural language model.
        cross_utt:  Using cross-utterance representation or not.
        model_var:  Using the variants of standard neural language model or not.

    Returns:
        The ppl of each sentence in evaluation set and the averaged ppl.
    """
    nnlm_1.eval()
    if 0 < weight <= 1:
        nnlm_2.eval()
        pass

    total_ppl = 0
    total_len = 0
    total_oov = 0
    num_sent = 0
    sent_and_ppl = defaultdict(float)

    #############################
    # Hidden state defination
    #############################
    # LSTM as main language model.
    if model_type != 'Transformer':
        hid_1 = nnlm_1.init_hidden(1)
        pass
    else:
        hid_1 = None
        pass

    # Transformer as main language model or using bi-LSTM. 
    # Then interpolated language model is a standard LSTM language model.
    if 0 < weight <= 1:
        hid_2 = nnlm_2.init_hidden(1)
        pass
    else:
        hid_2 = None
        pass

    #############################
    # Compute ppl of each sentence
    #############################
    for sent in eval_set:
        print("input sentence:", sent)
        input, target, num_oov = get_input_and_target(sent, vocab, cross_utt, seq_len)
        length = len(target)

        if cross_utt == 1:
            ppl, hid_1, hid_2 = compute_sentence_ppl(nnlm_1, criterion, ntokens, input, target, 
                                model_type, seq_len, nnlm_2, weight, cross_utt, hid_1, hid_2)

            # Write the history context
            if os.path.exists(os.path.join(args.outfile, 'context_id.txt')) is False:
                open(os.path.join(args.outfile, 'context_id.txt'), "w")
                open(os.path.join(args.outfile, 'context_text.txt'), "w")
                print("flag")
                pass

            with open(os.path.join(args.outfile, 'context_id.txt'), "r+", encoding='utf-8') as f:
                content = f.read()
                f.seek(0, 0)
                f.write(' '.join(map(str, input[-length:])) + '\n' + content)
                pass

            with open(os.path.join(args.outfile, 'context_text.txt'), "r+", encoding='utf-8') as f:
                content = f.read()
                f.seek(0, 0)
                f.write(sent + '\n'+ content)
                pass
            pass
        else:
            ppl, _, _ = compute_sentence_ppl(nnlm_1, criterion, ntokens, input, target, 
                                model_type, seq_len, nnlm_2, weight, cross_utt, hid_1, hid_2)
            pass

        sent_and_ppl[sent] = ppl
        num_sent += 1
        total_len += length
        total_ppl += ppl
        total_oov += num_oov
        pass

    print("Number of sentences in the eval set: %d " % num_sent)
    print("Number of words in the eval set: %d " % total_len)
    print("PPL on the eval set: %.4f " % math.exp(total_ppl / total_len))

    return sent_and_ppl


def write_prob(sent_and_ppl, path):
    r"""Write out sentence probability of sentence of evaluation set.
        ...

    Args:
        sent_and_ppl:     The sentences and their ppl represented by a dictionary.
        path (str):       A output file of sentences' probability.
    """

    with open(os.path.join(path, 'prob'), 'w', encoding='utf-8') as f:
        for ppl in sent_and_ppl.values():
            f.write('%.4f\n' % (1 / ppl))
            pass
        pass
    pass


#############################
# Load data
#############################
print("Model path: ", args.model_dir)
print("Load vocabulary: ", args.vocabulary)
vocab = read_vocab(args.vocabulary)
ntokens = len(vocab)
print("Load evaluation data: ", args.inpfile)
eval_set = load_data(args.inpfile)
print("Compute sentence scores with a ", args.model, " model")

#############################
# Neural language model
#############################
print("Load model and criterion")
import model
if args.model == 'Transformer':
    # The activation function can be 'relu' (default) or 'gelu'
    if args.model_var == 'none':
        nnlm_1 = model.TransformerModel(ntokens, args.emsize, args.nhead, args.nhid,
                                        args.nlayers, 0.5, "gelu", True).to(device)
        print("Main neural language model:\n", nnlm_1)
        pass

    if 0 < args.nnlm_weight <= 1:
        nnlm_2 = model.RNNModel('LSTM', ntokens, 1024, 1024, 2, 0.5, True).to(device)
        print("Interpolated neural language model used:\n", nnlm_2)
        pass
    else:
        nnlm_2 = None
        print("No interpolated neural language model used:\n", nnlm_2)
        pass
    pass
elif args.model == 'LSTM':
    # Bi-LSTM: A backward LSTM as main language model and interpolated with a forward standard LSTM model.
    if args.model_var == 'none' or args.model_var == 'bid':
        nnlm_1 = model.RNNModel(args.model, ntokens, args.emsize, args.nhid,
                                args.nlayers, 0.5, True).to(device)
        print("Main neural language model:\n", nnlm_1)
        pass
    if 0 < args.nnlm_weight <= 1:
        nnlm_2 = model.RNNModel(args.model, ntokens, args.emsize, args.nhid,
                                args.nlayers, 0.5, True).to(device)
        print("Interpolated neural language model used:\n", nnlm_2)
        print("Interpolated language model weight: %.2f" % args.nnlm_weight)
        pass
    else:
        nnlm_2 = None
        print("No interpolated neural language model used:\n", nnlm_2)
        pass
    pass

#############################
# Load model and criterion
#############################
with open(os.path.join(args.model_dir, 'model.pt'), 'rb') as f:
    nnlm1_para = torch.load(f, map_location=lambda storage, loc: storage)
    pass

nnlm1_dict = nnlm_1.state_dict()
nnlm1_para = {k: v for k, v in nnlm1_para.items() if k in nnlm1_dict}
nnlm1_dict.update(nnlm1_para)
nnlm_1.load_state_dict(nnlm1_dict)

if 0 < args.nnlm_weight <= 1:
    with open(os.path.join(args.nnlm_itdir, 'model.pt'), 'rb') as f:
        nnlm2_para = torch.load(f, map_location=lambda storage, loc: storage)
        pass

    nnlm2_dict = nnlm_2.state_dict()
    nnlm2_para =  {k: v for k, v in nnlm2_para.items() if k in nnlm2_dict}
    nnlm2_dict.update(nnlm2_para)
    nnlm_2.load_state_dict(nnlm2_dict)
    pass

criterion = nn.CrossEntropyLoss()

#############################
# Compute and write the ppl
#############################
eval_ppl = compute_ppl(eval_set, nnlm_1, criterion, ntokens, vocab,
                    args.model, args.seq_len, nnlm_2, args.nnlm_weight, 
                    args.cross_utt, args.model_var)
# print("Write sentence ppl out\n", eval_ppl)
write_prob(eval_ppl, args.outfile)

