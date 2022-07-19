from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import math
import torch
import torch.nn as nn
import torch.nn.functional as F
import torch.nn.init as init
import numpy as np
from torch.nn.utils.rnn import PackedSequence

from embed_regularize import embedded_dropout
from locked_dropout import LockedDropout

_VF = torch._C._VariableFunctions
_rnn_impls = {
    'LSTM': _VF.lstm,
    'GRU': _VF.gru,
    'RNN_TANH': _VF.rnn_tanh,
    'RNN_RELU': _VF.rnn_relu,
}


class RNNModel(nn.Module):
    """Container module with an encoder, a recurrent module, and a decoder."""
    def __init__(self, rnn_type, ntoken, ninp, nhid, nlayers, dropout=0.5,
                 tie_weights=False, bid=False):
        super(RNNModel, self).__init__()

        self.rnn_type = rnn_type
        self.nhid = nhid
        self.nlayers = nlayers
        self.bid = bid
        self.drop = nn.Dropout(dropout)
        self.encoder = nn.Embedding(ntoken, ninp)
        if rnn_type in ['LSTM', 'GRU']:
            self.rnn = getattr(nn, rnn_type)(ninp, nhid, nlayers, dropout=dropout)
            #self.rnn = LSTM(ninp, nhid, nlayers, dropout=dropout)
        else:
            try:
                nonlinearity = {'RNN_TANH': 'tanh', 'RNN_RELU': 'relu'}[rnn_type]
            except KeyError:
                raise ValueError("""An invalid option for `--model` was supplied,
                      options are ['LSTM', 'GRU', 'RNN_TANH' or 'RNN_RELU']""")
            self.rnn = nn.RNN(ninp, nhid, nlayers, nonlinearity=nonlinearity,
                              dropout=dropout)
        self.decoder = nn.Linear(nhid, ntoken)

        if tie_weights:
            if nhid != ninp:
                raise ValueError('When using the tied flag, nhid must be equal '
                                 'to emsize.')
            self.decoder.weight = self.encoder.weight

        self.init_weights()

    def init_weights(self):
        initrange = 0.1
        nn.init.uniform_(self.encoder.weight, -initrange, initrange)
        nn.init.zeros_(self.decoder.bias)
        nn.init.uniform_(self.decoder.weight, -initrange, initrange)

    def forward(self, x, hidden):
        emb = self.drop(self.encoder(x))
        output, hidden = self.rnn(emb, hidden)
        output = self.drop(output)
        if self.bid == True:
            output_1, output_2 = output.chunk(2, dim=-1)
            decoded_1 = self.decoder(output_1)
            decoded_2 = self.decoder(output_2)
            return decoded_1, decoded_2, hidden
        else:
            decoded = self.decoder(output)
            return decoded, hidden

    def init_hidden(self, bsz):
        weight = next(self.parameters())
        if self.rnn_type == 'LSTM':
            return (weight.new_zeros(self.nlayers, bsz, self.nhid),
                    weight.new_zeros(self.nlayers, bsz, self.nhid))
        return weight.new_zeros(self.nlayers, bsz, self.nhid)


'''
Self build Transformer-XL
XBY 2022.2.5: Transformer-XL
'''

class BiRNNModel(nn.Module):
    """Container module with an encoder, a recurrent module, and a decoder."""
    def __init__(self, rnn_type, ntoken, ninp, nhid, nlayers, dropout=0.5,
                 tie_weights=False, bid=True):
        super(BiRNNModel, self).__init__()

        self.rnn_type = rnn_type
        self.nhid = nhid
        self.nlayers = nlayers
        self.bid = bid
        self.drop = nn.Dropout(dropout)
        self.encoder = nn.Embedding(ntoken, ninp)
        if rnn_type in ['LSTM', 'GRU']:
            self.rnn = getattr(nn, rnn_type)(ninp, nhid, nlayers, dropout=dropout)
            self.rnn_back = getattr(nn, rnn_type)(ninp, nhid, nlayers, dropout=dropout)
            #self.rnn = LSTM(ninp, nhid, nlayers, dropout=dropout)
        else:
            try:
                nonlinearity = {'RNN_TANH': 'tanh', 'RNN_RELU': 'relu'}[rnn_type]
            except KeyError:
                raise ValueError("""An invalid option for `--model` was supplied,
                      options are ['LSTM', 'GRU', 'RNN_TANH' or 'RNN_RELU']""")
            self.rnn = nn.RNN(ninp, nhid, nlayers, nonlinearity=nonlinearity,
                              dropout=dropout)
        self.decoder = nn.Linear(nhid*2, ntoken)

        # if tie_weights:
        #     if nhid != ninp:
        #         raise ValueError('When using the tied flag, nhid must be equal '
        #                          'to emsize.')
        #     self.decoder.weight = self.encoder.weight

        self.init_weights()

    def init_weights(self):
        initrange = 0.1
        nn.init.uniform_(self.encoder.weight, -initrange, initrange)
        nn.init.zeros_(self.decoder.bias)
        nn.init.uniform_(self.decoder.weight, -initrange, initrange)

    def forward(self, x, hidden):
        emb = self.drop(self.encoder(x))
        # Compute word probability in two directions
        output1, hidden = self.rnn(emb, hidden)
        hidden2 = self.init_hidden(x.size()[1])
        # print(emb.size())
        output2, _ = self.rnn_back(torch.flip(emb, [0]), hidden2)
        output1 = self.drop(output1)
        output2 = self.drop(output2)

        # Reverse the backward sequence and shift two tokens before concatenate
        output2 = torch.flip(output2, [0])
        # output1[:-2, :, :] += output2[2:, :, :]
        shift = 2
        if output1.size()[0] == 1:
            out_shift = torch.zeros(1, output2.size()[1], output2.size()[2]).cuda()
            pass
        else:
            add = torch.zeros(shift, output2.size()[1], output2.size()[2]).cuda()
            out_shift = torch.cat((output2[shift:, :, :], add), 0)
            pass
        
        # print(add.size(), out_shift.size())
        output = torch.cat((output1, out_shift), -1)
        decoded = self.decoder(output)
        return decoded, hidden

    def init_hidden(self, bsz):
        weight = next(self.parameters())
        if self.rnn_type == 'LSTM':
            return (weight.new_zeros(self.nlayers, bsz, self.nhid),
                    weight.new_zeros(self.nlayers, bsz, self.nhid))
        return weight.new_zeros(self.nlayers, bsz, self.nhid)


class PositionalEncoding(nn.Module):
    r"""Inject some information about the relative or absolute position of the
        tokens in the sequence. The positional encodings have the same dimension
        as the embeddings, so that the two can be summed. Here, we use sine and
        cosine functions of different frequencies.
    .. math::
        \text{PosEncoder}(pos, 2i) = sin(pos/10000^(2i/d_model))
        \text{PosEncoder}(pos, 2i+1) = cos(pos/10000^(2i/d_model))
        \text{where pos is the word position and i is the embed idx)
    Args:
        d_model: the embed dim (required).
        dropout: the dropout value (default=0.1).
        max_len: the max. length of the incoming sequence (default=5000).
    Examples:
        >>> pos_encoder = PositionalEncoding(d_model)
    """

    def __init__(self, d_model, dropout=0.1, max_len=5000):
        super(PositionalEncoding, self).__init__()
        self.dropout = nn.Dropout(p=dropout)

        pe = torch.zeros(max_len, d_model)
        position = torch.arange(0, max_len, dtype=torch.float).unsqueeze(1)
        div_term = torch.exp(torch.arange(0, d_model, 2).float() * (-math.log(10000.0) / d_model))
        pe[:, 0::2] = torch.sin(position * div_term)
        pe[:, 1::2] = torch.cos(position * div_term)
        pe = pe.unsqueeze(0).transpose(0, 1)
        self.register_buffer('pe', pe)

    def forward(self, x):
        r"""Inputs of forward function
        Args:
            x: the sequence fed to the positional encoder model (required).
        Shape:
            x: [sequence length, batch size, embed dim]
            output: [sequence length, batch size, embed dim]
        Examples:
            >>> output = pos_encoder(x)
        """

        x = x + self.pe[:x.size(0), :]
        return self.dropout(x)


class TransformerModel(nn.Module):
    """Container module with an encoder, a transformer module, and a decoder."""

    def __init__(self, ntoken, ninp, nhead, nhid, nlayers, dropout=0.5,
                 activation="relu", tie_weights=False):
        super(TransformerModel, self).__init__()
        try:
            from torch.nn import TransformerEncoder, TransformerEncoderLayer
        except ImportError:
            raise ImportError('TransformerEncoder module does not exist in '
                              'PyTorch 1.1 or lower.')
        self.model_type = 'Transformer'
        self.src_mask = None
        self.pos_encoder = PositionalEncoding(ninp, dropout)
        encoder_layers = TransformerEncoderLayer(ninp, nhead, nhid, dropout,
                                                 activation)
        self.transformerlayers = TransformerEncoder(encoder_layers, nlayers)
        self.encoder = nn.Embedding(ntoken, ninp)
        self.ninp = ninp
        self.decoder = nn.Linear(ninp, ntoken)
        if tie_weights:
            # if nhid != ninp:
            #     raise ValueError('When using the tied flag, nhid must be equal '
            #                      'to emsize.')
            self.decoder.weight = self.encoder.weight
        self.init_weights()

    def _generate_square_subsequent_mask(self, sz):
        mask = (torch.triu(torch.ones(sz, sz)) == 1).transpose(0, 1)
        mask = mask.float().masked_fill(mask == 0, float('-inf')).masked_fill(
                mask == 1, float(0.0))
        return mask

    def init_weights(self):
        initrange = 0.1
        nn.init.uniform_(self.encoder.weight, -initrange, initrange)
        nn.init.zeros_(self.decoder.bias)
        nn.init.uniform_(self.decoder.weight, -initrange, initrange)

    def forward(self, src, has_mask=True):
        if has_mask:
            device = src.device
            if self.src_mask is None or self.src_mask.size(0) != len(src):
                mask = self._generate_square_subsequent_mask(len(src)).to(device)
                self.src_mask = mask
        else:
            self.src_mask = None
        src = self.encoder(src) * math.sqrt(self.ninp)
        src = self.pos_encoder(src)
        output = self.transformerlayers(src, self.src_mask)
        output = self.decoder(output)
        return output


'''
Self build LSTM
XBY 2.20: LSTM
'''

class LSTM(nn.Module):
    def __init__(self, input_size, hidden_size, num_layers=1, bias=True, dropout=0.):
        super(LSTM, self).__init__()
        self.input_size = input_size
        self.hidden_size = hidden_size
        self.bias = bias
        self.num_layers = num_layers
        self.dropout = float(dropout)

        # LSTM: input gate, forget gate, cell gate, output gate.
        gate_size = 4 * hidden_size

        self.weight_ih_mean_1 = nn.Parameter(torch.Tensor(gate_size, input_size))
        self.weight_hh_mean_1 = nn.Parameter(torch.Tensor(gate_size, hidden_size))
        self.bias_ih_mean_1 = nn.Parameter(torch.Tensor(gate_size))
        # Second bias vector included for CuDNN compatibility. Only one
        # bias vector is needed in standard definition.
        self.bias_hh_mean_1 = nn.Parameter(torch.Tensor(gate_size))
        self.weight_ih_mean_2 = nn.Parameter(torch.Tensor(gate_size, hidden_size))
        self.weight_hh_mean_2 = nn.Parameter(torch.Tensor(gate_size, hidden_size))
        self.bias_ih_mean_2 = nn.Parameter(torch.Tensor(gate_size))
        # Second bias vector included for CuDNN compatibility. Only one
        # bias vector is needed in standard definition.
        self.bias_hh_mean_2 = nn.Parameter(torch.Tensor(gate_size))

        self._all_weights = [k for k, v in self.__dict__.items() if '_ih' in k or '_hh' in k]

        self.reset_parameters()

    def extra_repr(self):
        s = '{input_size}, {hidden_size}'
        return s.format(**self.__dict__)

    def reset_parameters(self):
        stdv = 1.0 / math.sqrt(self.hidden_size)
        init.uniform_(self.weight_ih_mean_1, -stdv, stdv)
        init.uniform_(self.weight_hh_mean_1, -stdv, stdv)
        init.uniform_(self.bias_hh_mean_1, -stdv, stdv)
        init.uniform_(self.bias_ih_mean_1, -stdv, stdv)

        init.uniform_(self.weight_ih_mean_2, -stdv, stdv)
        init.uniform_(self.weight_hh_mean_2, -stdv, stdv)
        init.uniform_(self.bias_hh_mean_2, -stdv, stdv)
        init.uniform_(self.bias_ih_mean_2, -stdv, stdv)

    def flat_parameters(self):
        w_hh_1 = self.weight_hh_mean_1 * 1.
        w_ih_1 = self.weight_ih_mean_1 * 1.
        b_hh_1 = self.bias_hh_mean_1 * 1.
        b_ih_1 = self.bias_ih_mean_1 * 1.

        w_hh_2 = self.weight_hh_mean_2 * 1.
        w_ih_2 = self.weight_ih_mean_2 * 1.
        b_hh_2 = self.bias_hh_mean_2 * 1.
        b_ih_2 = self.bias_ih_mean_2 * 1.

        return [w_ih_1[:, :].contiguous(), w_hh_1[:, :].contiguous(),
                b_ih_1[:].contiguous(), b_hh_1[:].contiguous(),
                w_ih_2[:, :].contiguous(), w_hh_2[:, :].contiguous(),
                b_ih_2[:].contiguous(), b_hh_2[:].contiguous()
                ]

    @staticmethod
    def permute_hidden(hx, permutation):
        if permutation is None:
            return hx
        return hx[0].index_select(1, permutation), hx[1].index_select(1, permutation)

    def forward(self, inputs, hx=None):  # noqa: F811
        orig_input = inputs
        # xxx: isinstance check needs to be in conditional for TorchScript to compile
        if isinstance(orig_input, PackedSequence):
            inputs, batch_sizes, sorted_indices, unsorted_indices = inputs
            max_batch_size = batch_sizes[0]
            max_batch_size = int(max_batch_size)
        else:
            batch_sizes = None
            max_batch_size = inputs.size(1)
            sorted_indices = None
            unsorted_indices = None

        if hx is None:
            zeros = torch.zeros(self.num_layers,
                                max_batch_size, self.hidden_size,
                                dtype=inputs.dtype, device=inputs.device)
            hx = (zeros, zeros)
            pass
        else:
            # Each batch of the hidden state should match the input sequence that
            # the user believes he/she is passing in.
            hx = self.permute_hidden(hx, sorted_indices)
            pass
        pass

        # self.flatten_parameters()
        # print(self.flat_parameters()[0].size())
        if batch_sizes is None:
            result = _rnn_impls['LSTM'](inputs, hx, self.flat_parameters(), self.bias, self.num_layers,
                                        0., self.training, False, False)
            pass
        else:
            result = _rnn_impls['LSTM'](inputs, batch_sizes, hx, self.flat_parameters(), self.bias,
                                        self.num_layers, 0., self.training, False)
            pass
        pass

        output = result[0]
        hidden = result[1:]
        # xxx: isinstance check needs to be in conditional for TorchScript to compile
        if isinstance(orig_input, PackedSequence):
            output_packed = PackedSequence(output, batch_sizes, sorted_indices, unsorted_indices)
            return output_packed, self.permute_hidden(hidden, unsorted_indices)
        else:
            return output, self.permute_hidden(hidden, unsorted_indices)

'''
Self build Transformer
XBY 2.20: Transformer
'''

class MultiheadAttention(nn.Module):
    def __init__(self, embed_dim, num_heads, dropout=0., bias=True, add_bias_kv=False, add_zero_attn=False, kdim=None, vdim=None):
        super(MultiheadAttention, self).__init__()
        self.embed_dim = embed_dim
        self.kdim = kdim if kdim is not None else embed_dim
        self.vdim = vdim if vdim is not None else embed_dim
        self._qkv_same_embed_dim = self.kdim == embed_dim and self.vdim == embed_dim

        self.num_heads = num_heads
        self.dropout = dropout
        self.head_dim = embed_dim // num_heads
        assert self.head_dim * num_heads == self.embed_dim, "embed_dim must be divisible by num_heads"

        # self.in_proj_weight = Parameter(torch.empty(3 * embed_dim, embed_dim))
        self.qkv_net = nn.Linear(embed_dim, 3 * embed_dim)

        self.drop = nn.Dropout(dropout)

        # self.out_proj = _LinearWithBias(embed_dim, embed_dim)
        self.o_net = nn.Linear(embed_dim, embed_dim)

        # self.bias_k = self.bias_v = None

        self.add_zero_attn = add_zero_attn

        self._reset_parameters()

    def _reset_parameters(self):
        if self._qkv_same_embed_dim:
            nn.init.xavier_uniform_(self.qkv_net.weight)

        #if self.in_proj_bias is not None:
        nn.init.constant_(self.qkv_net.bias, 0.)
        nn.init.constant_(self.o_net.bias, 0.)

    def forward(self, query, key, value, key_padding_mask=None, need_weights=True, attn_mask=None):
        scaling = float(self.head_dim) ** -0.5
        tgt_len, bsz, embed_dim = query.size()

        # q, k, v size(): (seq_length, batch_size, dim_model)  e.g (100, 32, 512)
        q, k, v = self.qkv_net(query).chunk(3, dim=-1)
        q = q * scaling
        if attn_mask is not None:
            if attn_mask.dim() == 2:
                attn_mask = attn_mask.unsqueeze(0)
                if list(attn_mask.size()) != [1, query.size(0), key.size(0)]:
                    raise RuntimeError('The size of the 2D attn_mask is not correct.')
            elif attn_mask.dim() == 3:
                if list(attn_mask.size()) != [bsz * self.num_heads, query.size(0), key.size(0)]:
                    raise RuntimeError('The size of the 3D attn_mask is not correct.')
            else:
                raise RuntimeError("attn_mask's dimension {} is not supported".format(attn_mask.dim()))

        q = q.contiguous().view(tgt_len, bsz * self.num_heads, self.head_dim).transpose(0, 1)
        if k is not None:
            k = k.contiguous().view(-1, bsz * self.num_heads, self.head_dim).transpose(0, 1)
        if v is not None:
            v = v.contiguous().view(-1, bsz * self.num_heads, self.head_dim).transpose(0, 1)

        src_len = k.size(1)
        # q, k size(): (batch_size * num_heads, seq_length, head_dim)  e.g (256, 100, 64)
        # print("q.size(), k.size(): ", q.size(), k.size())
        attn_output_weights = torch.bmm(q, k.transpose(1, 2))

        # attn_output_weights.size(): (batch_size * num_heads, seq_length, seq_length)  e.g (256, 100, 100)
        # print("attn_output_weights.size(): ", attn_output_weights.size())
        # if not self.training:
        #     print("attn_output_weights.size(): ", attn_output_weights.size())

        assert list(attn_output_weights.size()) == [bsz * self.num_heads, tgt_len, src_len]
        if attn_mask is not None:
            if attn_mask.dtype == torch.bool:
                attn_output_weights.masked_fill_(attn_mask, float('-inf'))
            else:
                attn_output_weights += attn_mask

        attn_output_weights = F.softmax(attn_output_weights, dim=-1)
        attn_output_weights = self.drop(attn_output_weights)
        #print("attn_output_weights_2.size(): ", attn_output_weights.size())

        attn_output = torch.bmm(attn_output_weights, v)
        #print("attn_output_weights_2.size(): ", attn_output_weights.size())

        assert list(attn_output.size()) == [bsz * self.num_heads, tgt_len, self.head_dim]
        attn_output = attn_output.transpose(0, 1).contiguous().view(tgt_len, bsz, embed_dim)
        attn_output = self.o_net(attn_output)

        if need_weights:
            # average attention weights over heads
            attn_output_weights = attn_output_weights.view(bsz, self.num_heads, tgt_len, src_len)
            return attn_output, attn_output_weights.sum(dim=1) / self.num_heads
        else:
            return attn_output, None


class StandardTransformerEncoderLayer(nn.Module):
    def __init__(self, d_model, nhead, dim_feedforward=2048, dropout=0.1):
        super(StandardTransformerEncoderLayer, self).__init__()
        self.self_attn = MultiheadAttention(d_model, nhead, dropout=dropout)
        self.linear1 = nn.Linear(d_model, dim_feedforward)
        self.dropout = nn.Dropout(dropout)
        self.linear2 = nn.Linear(dim_feedforward, d_model)

        self.norm1 = nn.LayerNorm(d_model)
        self.norm2 = nn.LayerNorm(d_model)
        self.dropout1 = nn.Dropout(dropout)
        self.dropout2 = nn.Dropout(dropout)

        self.activation = nn.GELU()

    def forward(self, src, src_mask=None):
        # src.size(): (seq_length, batch_size, dim_model)  e.g (100, 32, 512)
        # print("src.size(): ", src.size())
        src2 = self.self_attn(src, src, src, attn_mask=src_mask)[0]
        src = src + self.dropout1(src2)
        src = self.norm1(src)
        src2 = self.linear2(self.dropout(self.activation(self.linear1(src))))
        src = src + self.dropout2(src2)
        src = self.norm2(src)
        return src


class LSTMTransformerModel(nn.Module):
    """Container module with an LSTM module, an encoder, a transformer module, and a decoder."""

    def __init__(self, ntoken, ninp, nhead, nhid, nlayers, ninp_l, nhid_l, nlayers_l, dropout=0.5, tie_weights=False):
        super(LSTMTransformerModel, self).__init__()
        # try:
        #     from torch.nn import TransformerEncoder, TransformerEncoderLayer
        # except ImportError:
        #     raise ImportError('TransformerEncoder module does not exist in '
        #                       'PyTorch 1.1 or lower.')
        self.model_type = 'Transformer'
        self.src_mask = None
        self.pos_encoder = PositionalEncoding(ninp, dropout)
        self.transformerlayers = nn.ModuleList()
        self.drop = nn.Dropout(dropout)

        # LSTM module        
        self.nhid_l = nhid_l
        self.nlayers_l = nlayers_l
        self.rnn = getattr(nn, 'LSTM')(ninp_l, nhid_l, nlayers_l, dropout=dropout)
        for _ in range(nlayers):
            self.transformerlayers.append(StandardTransformerEncoderLayer(ninp, nhead, nhid, dropout))
        pass

        # encoder_layers = TransformerEncoderLayer(ninp, nhead, nhid, dropout,
        #                                          activation)
        # self.transformer_encoder = TransformerEncoder(encoder_layers, nlayers)
        self.encoder = nn.Embedding(ntoken, ninp)
        self.ninp = ninp
        self.decoder = nn.Linear(ninp, ntoken)

        if tie_weights:
            #if nhid != ninp:
            #    raise ValueError('When using the tied flag, nhid must be equal '
            #                     'to emsize.')
            self.decoder.weight = self.encoder.weight
        self.init_weights()

    def _generate_square_subsequent_mask(self, sz):
        mask = (torch.triu(torch.ones(sz, sz)) == 1).transpose(0, 1)
        mask = mask.float().masked_fill(mask == 0, float('-inf')).masked_fill(
                mask == 1, float(0.0))
        return mask

    def init_weights(self):
        initrange = 0.1
        nn.init.uniform_(self.encoder.weight, -initrange, initrange)
        nn.init.zeros_(self.decoder.bias)
        nn.init.uniform_(self.decoder.weight, -initrange, initrange)

    def init_hidden(self, bsz):
        weight = next(self.parameters())

        return (weight.new_zeros(self.nlayers_l, bsz, self.nhid_l),
                weight.new_zeros(self.nlayers_l, bsz, self.nhid_l))


    def forward(self, src, hidden, has_mask=True):
        # src.size(): (seq_length, batch_size)  e.g (100, 32)
        # print("src.size(): ", src.size())
        emb = self.encoder(src)
        emb_d = self.drop(emb)
        out_l, hidden = self.rnn(emb_d, hidden)

        if has_mask:
            device = src.device
            if self.src_mask is None or self.src_mask.size(0) != len(src):
                mask = self._generate_square_subsequent_mask(len(src)).to(device)
                self.src_mask = mask
        else:
            self.src_mask = None
        # src = self.encoder(src) * math.sqrt(self.ninp)
        src = emb * math.sqrt(self.ninp)

        src = self.pos_encoder(src)
        # output = self.transformerlayers(src, self.src_mask)
        output = src + out_l

        # output.size(): (seq_length, batch_size, dim_model) e.g (100, 32, 512)
        # print("output.size(): ", output.size())
        for mod in self.transformerlayers:
            output = mod(output, src_mask=self.src_mask)

        output = self.decoder(output)

        return output, hidden


class LSTMTransformerModel2(nn.Module):
    """Container module with an LSTM module, an encoder, a transformer module, and a decoder."""

    def __init__(self, ntoken, ninp, nhead, nhid, nlayers, ninp_l, nhid_l, nlayers_l, dropout=0.5, tie_weights=False):
        super(LSTMTransformerModel2, self).__init__()
        # try:
        #     from torch.nn import TransformerEncoder, TransformerEncoderLayer
        # except ImportError:
        #     raise ImportError('TransformerEncoder module does not exist in '
        #                       'PyTorch 1.1 or lower.')
        self.model_type = 'Transformer'
        self.src_mask = None
        self.nlayers = nlayers
        self.nlayers_l = nlayers_l
        self.pos_encoder = PositionalEncoding(ninp, dropout)
        self.transformerlayers = nn.ModuleList()
        self.rnn = nn.ModuleList()
        self.drop = nn.Dropout(dropout)

        # LSTM module        
        self.nhid_l = nhid_l
        # self.rnn = getattr(nn, 'LSTM')(ninp_l, nhid_l, nlayers, dropout=dropout)
        for _ in range(nlayers):
            self.rnn.append(getattr(nn, 'LSTM')(ninp_l, nhid_l, nlayers_l, dropout=0.2))
            self.transformerlayers.append(StandardTransformerEncoderLayer(ninp, nhead, nhid, dropout))
            pass

        # encoder_layers = TransformerEncoderLayer(ninp, nhead, nhid, dropout,
        #                                          activation)
        # self.transformer_encoder = TransformerEncoder(encoder_layers, nlayers)
        self.encoder = nn.Embedding(ntoken, ninp)
        self.ninp = ninp
        self.decoder = nn.Linear(ninp, ntoken)

        if tie_weights:
            #if nhid != ninp:
            #    raise ValueError('When using the tied flag, nhid must be equal '
            #                     'to emsize.')
            self.decoder.weight = self.encoder.weight
            pass

        self.init_weights()

    def _generate_square_subsequent_mask(self, sz):
        mask = (torch.triu(torch.ones(sz, sz)) == 1).transpose(0, 1)
        mask = mask.float().masked_fill(mask == 0, float('-inf')).masked_fill(
                mask == 1, float(0.0))
        return mask

    def init_weights(self):
        initrange = 0.1
        nn.init.uniform_(self.encoder.weight, -initrange, initrange)
        nn.init.zeros_(self.decoder.bias)
        nn.init.uniform_(self.decoder.weight, -initrange, initrange)

    def init_hidden(self, bsz):
        weight = next(self.parameters())

        hiddens = []
        for _ in range(self.nlayers):
            hiddens.append((weight.new_zeros(self.nlayers_l, bsz, self.nhid_l),
                weight.new_zeros(self.nlayers_l, bsz, self.nhid_l)))

        return hiddens

    def forward(self, src, hidden, has_mask=True):
        # src.size(): (seq_length, batch_size)  e.g (100, 32)
        # print("src.size(): ", src.size())
        # emb = self.encoder(src)
        # emb_d = self.drop(emb)
        # out_l, hidden = self.rnn(emb_d, hidden)

        if has_mask:
            device = src.device
            if self.src_mask is None or self.src_mask.size(0) != len(src):
                mask = self._generate_square_subsequent_mask(len(src)).to(device)
                self.src_mask = mask
                pass
            pass
        else:
            self.src_mask = None
            pass

        src = self.encoder(src) * math.sqrt(self.ninp)

        src = self.pos_encoder(src)
        # output = self.transformerlayers(src, self.src_mask)
        output = src

        hiddens = []
        # output.size(): (seq_length, batch_size, dim_model) e.g (100, 32, 512)
        # print("output.size(): ", output.size())
        for i in range(self.nlayers):
            # lstm_hids = [hidden[0][i, :, :].unsqueeze(0), hidden[1][i, :, :].unsqueeze(0)]
            out_l, hids = self.rnn[i](output, hidden[i])
            hiddens.append(hids)
            output = self.transformerlayers[i](output + out_l, src_mask=self.src_mask)
            pass

        output = self.decoder(output)

        return output, hiddens

'''
Self build Transformer-XL
XBY 2022.2.5: Transformer-XL
'''

def _rel_shift(x, zero_triu=False):
    x_padded = x.reshape(x.size(1), x.size(0), *x.size()[2:])
    x = x_padded[1:].reshape(x.size(0), x.size(1) - 1, *x.size()[2:])

    if zero_triu:
        ones = torch.ones((x.size(0), x.size(1)))
        x = x * torch.tril(ones, x.size(1) - x.size(0))[:,:,None,None]

    return x


class PositionalEmbedding(nn.Module):
    def __init__(self, demb):
        super(PositionalEmbedding, self).__init__()

        self.demb = demb

        inv_freq = 1 / (10000 ** (torch.arange(0.0, demb, 2.0) / demb))
        self.register_buffer('inv_freq', inv_freq)

    def forward(self, pos_seq, bsz=None):
        sinusoid_inp = torch.ger(pos_seq, self.inv_freq)
        pos_emb = torch.cat([sinusoid_inp.sin(), sinusoid_inp.cos()], dim=-1)

        if bsz is not None:
            return pos_emb[:,None,:].expand(-1, bsz, -1)
        else:
            return pos_emb[:,None,:]


class MultiHeadAttn(nn.Module):
    def __init__(self, n_head, d_model, dropout):
        super(MultiHeadAttn, self).__init__()

        self.n_head = n_head
        self.d_model = d_model
        self.d_head = int(d_model / n_head)

        self.dropout = dropout

        self.qkv_net = nn.Sequential(
            nn.Linear(d_model, 3 * n_head * self.d_head, bias=False),
            LockedDropout(dropout)
        )

        self.drop = nn.Dropout(dropout)
        self.locked_drop = LockedDropout(dropout)

        self.o_net = nn.Linear(n_head * self.d_head, d_model, bias=False)

        self.layer_norm = nn.LayerNorm(d_model)

        self.scale = 1 / (self.d_head ** 0.5)

    def forward(self, h, attn_mask=None, mems=None):
        ##### multihead attention
        # [hlen x bsz x n_head x d_head]
        head_q, head_k, head_v = torch.chunk(self.qkv_net(h), 3, -1)
        head_q = head_q.view(h.size(0), h.size(1), self.n_head, self.d_head)
        head_k = head_k.view(h.size(0), h.size(1), self.n_head, self.d_head)
        head_v = head_v.view(h.size(0), h.size(1), self.n_head, self.d_head)

        # [qlen x klen x bsz x n_head]
        attn_score = torch.einsum('ibnd,jbnd->ijbn', (head_q, head_k))
        attn_score.mul_(self.scale)
        if attn_mask is not None and attn_mask.any().item():
            if attn_mask.dim() == 2:
                attn_score.masked_fill_(attn_mask[None,:,:,None], -float('inf'))
            elif attn_mask.dim() == 3:
                attn_score.masked_fill_(attn_mask[:,:,:,None], -float('inf'))

        # [qlen x klen x bsz x n_head]
        attn_prob = F.softmax(attn_score, dim=1)
        attn_prob = self.drop(attn_prob)

        # [qlen x klen x bsz x n_head] + [klen x bsz x n_head x d_head]
        # -> [qlen x bsz x n_head x d_head]
        attn_vec = torch.einsum('ijbn,jbnd->ibnd', (attn_prob, head_v))
        attn_vec = attn_vec.contiguous().view(
            attn_vec.size(0), attn_vec.size(1), self.n_head * self.d_head)

        ##### linear projection
        attn_out = self.o_net(attn_vec)
        attn_out = self.locked_drop(attn_out)

        ##### residual connection + layer normalization
        output = self.layer_norm(h + attn_out)

        return output


class RelMultiHeadAttn(MultiHeadAttn):
    def __init__(self, n_head, d_model, dropout):
        super(RelMultiHeadAttn, self).__init__(n_head, d_model, dropout)

    def forward(self, w, r, attn_mask=None, mems=None):
        qlen, rlen, bsz = w.size(0), r.size(0), w.size(1)

        if mems is not None:
            w_heads = self.qkv_net(torch.cat([mems, w], 0))
            r_heads = self.qkv_net(r)

            w_head_q, w_head_k, w_head_v = torch.chunk(w_heads, 3, dim=-1)
            r_head_q, r_head_k, r_head_v = torch.chunk(r_heads, 3, dim=-1)

            w_head_q = w_head_q[-qlen:]
            pass
        else:
            w_heads = self.qkv_net(w)
            r_heads = self.qkv_net(r)

            w_head_q, w_head_k, w_head_v = torch.chunk(w_heads, 3, dim=-1)
            r_head_q, r_head_k, r_head_v = torch.chunk(r_heads, 3, dim=-1)
            pass

        klen = w_head_k.size(0)

        w_head_q = w_head_q.view(qlen, bsz, self.n_head, self.d_head)
        w_head_k = w_head_k.view(klen, bsz, self.n_head, self.d_head)
        w_head_v = w_head_v.view(klen, bsz, self.n_head, self.d_head)

        r_head_q = r_head_q.view(rlen, self.n_head, self.d_head)
        r_head_k = r_head_k.view(rlen, self.n_head, self.d_head)

        #### compute attention score
        rw_head_q = w_head_q + r_head_q[-1]
        AC = torch.einsum('ibnd,jbnd->ijbn', (rw_head_q, w_head_k))

        rr_head_q = w_head_q + r_head_q[-1]
        BD = torch.einsum('ibnd,jnd->ijbn', (rr_head_q, r_head_k))
        BD = _rel_shift(BD)

        # [qlen x klen x bsz x n_head]
        attn_score = AC + BD
        attn_score.mul_(self.scale)

        # print(attn_mask)
        # #### compute attention probability
        if attn_mask is not None and attn_mask.any().item():
            if attn_mask.dim() == 2:
                attn_score.masked_fill_(attn_mask[None, :, :, None], -float('inf'))
            elif attn_mask.dim() == 3:
                attn_score.masked_fill_(attn_mask[:, :, :, None], -float('inf'))

        # [qlen x klen x bsz x n_head]
        attn_prob = F.softmax(attn_score, dim=1)
        attn_prob = self.drop(attn_prob)
        # attn_prob = self.locked_drop(attn_prob)

        #### compute attention vector
        attn_vec = torch.einsum('ijbn,jbnd->ibnd', (attn_prob, w_head_v))

        # [qlen x bsz x n_head x d_head]
        attn_vec = attn_vec.contiguous().view(
            attn_vec.size(0), attn_vec.size(1), self.n_head * self.d_head)

        ##### linear projection
        attn_out = self.o_net(attn_vec)
        attn_out = self.locked_drop(attn_out)

        ##### residual connection + layer normalization
        output = self.layer_norm(w + attn_out)

        return output


class TransformerXLDecoderLayer(nn.Module):
    def __init__(self, d_model, nhead, dim_feedforward, dropoutf, dropouta, **kwargs):
        super(TransformerXLDecoderLayer, self).__init__()
        self.self_attn = RelMultiHeadAttn(nhead, d_model, dropouta, **kwargs)
        self.linear1 = nn.Linear(d_model, dim_feedforward)
        self.dropout = nn.Dropout(dropoutf)
        self.linear2 = nn.Linear(dim_feedforward, d_model)

        self.norm1 = nn.LayerNorm(d_model)
        self.norm2 = nn.LayerNorm(d_model)
        self.dropout1 = nn.Dropout(dropouta)
        self.dropout2 = nn.Dropout(dropoutf)

        self.activation = nn.GELU()

    def forward(self, src, pos_emb, src_mask=None, mems=None):
        # src.size(): (seq_length, batch_size, dim_model)  e.g (100, 32, 512)
        # print("src.size(): ", src.size())
        src2 = self.self_attn(src, pos_emb, attn_mask=src_mask, mems=mems)
        src = src + self.dropout1(src2)
        src = self.norm1(src)
        src2 = self.linear2(self.dropout(self.activation(self.linear1(src))))
        src = src + self.dropout2(src2)
        src = self.norm2(src)
        return src


class TransformerXLModel(nn.Module):
    """Container module with an encoder, a transformer module, and a decoder."""

    def __init__(self, ntoken, ninp, nhead, nhid, nlayers, dropoute, dropouti, dropouta, dropoutf, dropouth, dropouto,
                tie_weight=True, tgt_len=None, ext_len=0, mem_len=0, clamp_len=-1):
        super(TransformerXLModel, self).__init__()

        self.model_type = 'Transformer'
        self.src_mask = None
        self.pos_emb = PositionalEmbedding(ninp)
        # self.pos_encoder = PositionalEncoding(ninp, dropout)
        self.transformerlayers = nn.ModuleList()

        self.n_token = ntoken
        self.d_model = ninp
        self.n_head = nhead

        self.dropoute = dropoute
        self.dropouti = dropouti
        self.dropouth = dropouth
        self.dropouto = dropouto

        self.drop_i = nn.Dropout(dropouti)
        self.locked_drop_i = LockedDropout(dropouti)
        self.locked_drop_h = LockedDropout(dropouth)
        self.locked_drop_o = LockedDropout(dropouto)

        self.n_layer = nlayers

        self.tgt_len = tgt_len
        self.ext_len = ext_len
        self.mem_len = mem_len
        self.clamp_len = clamp_len

        for i in range(nlayers):
            self.transformerlayers.append(TransformerXLDecoderLayer(ninp, nhead, nhid, dropoutf, dropouta))
            pass

        self.encoder = nn.Embedding(ntoken, ninp)
        self.ninp = ninp
        self.decoder = nn.Linear(ninp, ntoken)

        if tie_weight:
            #if nhid != ninp:
            #    raise ValueError('When using the tied flag, nhid must be equal '
            #                     'to emsize.')
            self.decoder.weight = self.encoder.weight
            pass

        self.init_weights()

    def init_weights(self):
        initrange = 0.1
        nn.init.uniform_(self.encoder.weight, -initrange, initrange)
        nn.init.zeros_(self.decoder.bias)
        nn.init.uniform_(self.decoder.weight, -initrange, initrange)

    def reset_length(self, tgt_len, ext_len, mem_len):
        self.tgt_len = tgt_len
        self.mem_len = mem_len
        self.ext_len = ext_len

    def init_mems(self):
        if self.mem_len > 0:
            mems = []

            for _ in range(self.n_layer):
                empty = torch.empty(0, dtype=self.encoder.weight.dtype,
                                    device=self.encoder.weight.device)
                mems.append(empty)
                pass

            return mems
        else:
            return None

    def _update_mems(self, hids, mems, qlen, mlen):
        # does not deal with None
        if mems is None: return None

        # mems is not None
        assert len(hids) == len(mems), 'len(hids) != len(mems)'

        # There are `mlen + qlen` steps that can be cached into mems
        # For the next step, the last `ext_len` of the `qlen` tokens
        # will be used as the extended context. Hence, we only cache
        # the tokens from `mlen + qlen - self.ext_len - self.mem_len`
        # to `mlen + qlen - self.ext_len`.
        with torch.no_grad():
            new_mems = []
            end_idx = mlen + max(0, qlen - 0 - self.ext_len)
            beg_idx = max(0, end_idx - self.mem_len)

            for i in range(len(hids)):
                cat = torch.cat([mems[i], hids[i]], dim=0)
                new_mems.append(cat[beg_idx:end_idx].detach())
                pass
            pass

        return new_mems

    def _generate_square_subsequent_mask(self, sz):
        mask = (torch.triu(torch.ones(sz, sz)) == 1).transpose(0, 1)
        mask = mask.float().masked_fill(mask == 0, float('-inf')).masked_fill(
                mask == 1, float(0.0))
        return mask

    def forward(self, src, *mems):
        # src.size(): (seq_length, batch_size)  e.g (100, 32)
        # print("src.size(): ", src.size())
        if not mems: mems = self.init_mems()

        qlen, bsz = src.size()

        # if has_mask:
        #     device = src.device
        #     if self.src_mask is None or self.src_mask.size(0) != len(src):
        #         mask = self._generate_square_subsequent_mask(len(src)).to(device)
        #         self.src_mask = mask
        #         pass
        #     pass
        # else:
        #     self.src_mask = None
        #     pass

        word_emb = self.encoder(src) * math.sqrt(self.ninp)
        mlen = mems[0].size(0) if mems is not None else 0

        klen = mlen + qlen
        self.src_mask = torch.triu(
            word_emb.new_ones(qlen, klen), diagonal=1+mlen).bool()[:,:,None]

        hids = []

        # relative pos embedding
        pos_seq = torch.arange(klen, -1, -1.0, device=word_emb.device)
        if self.clamp_len > 0:
            pos_seq.clamp_(max=self.clamp_len)
        pos_emb = self.pos_emb(pos_seq)

        # src = self.pos_encoder(src)
        # output = self.transformerlayers(src, self.src_mask)
        output = src

        # initial inputs
        core_out = self.locked_drop_i(word_emb)
        pos_emb = self.locked_drop_i(pos_emb)

        # compute hids
        # output.size(): (seq_length, batch_size, dim_model) e.g (100, 32, 512)
        # print("output.size(): ", output.size())
        for i, layer in enumerate(self.transformerlayers):
            # save the input to each layer for memory
            hids.append(core_out)

            # current memory
            mems_i = mems[i] if mems is not None else None

            core_out = layer(core_out, pos_emb, src_mask=self.src_mask, mems=mems_i)

            # apply dropouth, if it is not the last layer
            if i < len(self.transformerlayers) - 1:
                core_out = self.locked_drop_h(core_out)
                pass
            pass

        # update memory
        new_mems = self._update_mems(hids, mems, mlen, qlen)

        # compute loss
        hidden = core_out[-qlen:]
        pred_hid = self.locked_drop_o(hidden)

        output = self.decoder(pred_hid)

        return output, new_mems


