#!/usr/bin/env bash
# Copyright 2012  Johns Hopkins University (author: Daniel Povey)
#           2020  Ke Li

# This script trains an RNN (including LSTM and GRU) or Transformer-based language model with PyTorch and performs N-best rescoring

# Dev/eval2000 perplexity of a 2-layer LSTM model is: 47.1/41.9. WERs with N-best rescoring (with hidden states carried over sentences):
# %WER 10.9 | 4459 42989 | 90.5 6.4 3.1 1.4 10.9 42.7 | exp/chain/tdnn7q_sp/decode_eval2000_sw1_fsh_fg_pytorch_lstm_nbest//score_10_0.0/eval2000_hires.ctm.filt.sys
# %WER 7.1 | 1831 21395 | 93.8 4.1 2.1 0.9 7.1 36.4 | exp/chain/tdnn7q_sp/decode_eval2000_sw1_fsh_fg_pytorch_lstm_nbest//score_11_0.0/eval2000_hires.ctm.swbd.filt.sys
# %WER 14.6 | 2628 21594 | 87.3 8.5 4.1 1.9 14.6 46.7 | exp/chain/tdnn7q_sp/decode_eval2000_sw1_fsh_fg_pytorch_lstm_nbest//score_10_0.0/eval2000_hires.ctm.callhm.filt.sys
# Without hidden-state-carry-over, the WER on eval2000 from N-best rescoring with the LSTM model is 11.2

# Dev/eval2000 perplexity of a Transformer LM is: 47.0/41.6. WERs with N-best rescoring:
# %WER 10.8 | 4459 42989 | 90.6 6.3 3.1 1.5 10.8 42.1 | exp/chain/tdnn7q_sp/decode_eval2000_sw1_fsh_fg_pytorch_transformer_nbest//score_10_0.0/eval2000_hires.ctm.filt.sys
# %WER 7.2 | 1831 21395 | 93.7 4.2 2.1 1.0 7.2 37.3 | exp/chain/tdnn7q_sp/decode_eval2000_sw1_fsh_fg_pytorch_transformer_nbest//score_10_0.0/eval2000_hires.ctm.swbd.filt.sys
# %WER 14.4 | 2628 21594 | 87.6 8.3 4.1 2.0 14.4 45.5 | exp/chain/tdnn7q_sp/decode_eval2000_sw1_fsh_fg_pytorch_transformer_nbest//score_10_0.0/eval2000_hires.ctm.callhm.filt.sys

# Begin configuration section.
source /opt/share/etc/gcc-5.4.0.sh

# This script trains an RNN (including LSTM and GRU) or Transformer-based language model with PyTorch and performs N-best rescoring
stage=0
gpu=1
model_type=LSTM # LSTM, GRU or Transformer
embedding_dim=1024 # 512 for Transformer (to reproduce the perplexities and WERs above)
hidden_dim=1024 # 512 for Transformer
nlayers=2 # 6 for Transformer
nhead=8 # for Transformer
learning_rate=5 # 0.1 for Transformer
seq_len=100
model_var=none # none for baseline
prior_path=steps/pytorchnn/prior/lstm # load pretrained prior model
prior=False # load a pretrained model
mark=marks # save_path disctinct to uncover
inter_flag=0
inter_alpha=0.8
seg_wise=1

##################################################################################################
dropout=0.2 # baseline 0.2 | bayesian initial 0.0
##################################################################################################
ratio=1.0
. ./cmd.sh
. ./path.sh
. ./utils/parse_options.sh

set -e
export CUDA_VISIBLE_DEVICES=$gpu
data_dir=data/pytorchnn
ac_model_dir=exp/chain/cnn_tdnn1a_sp_ep6_specmaskonline

pytorch_path=swbd/pytorch-${model_type}-emb${embedding_dim}_hid${hidden_dim}_nly${nlayers}-${dropout}-${model_var}-${seq_len}-pre${prior}-${mark}
nn_model=$pytorch_path/model.pt
decode_dir_suffix=pytorch-${model_type}-emb${embedding_dim}_hid${hidden_dim}_nly${nlayers}-${dropout}-${model_var}-${seq_len}-pre${prior}-${mark}-itpr${itpr}-ib${inter_flag}-${inter_alpha}-seg${seg_wise}-nbest${nbest}

mkdir -p $pytorch_path

# Check if PyTorch is installed to use with python
if python steps/pytorchnn/check_py.py 2>/dev/null; then
  echo PyTorch is ready to use on the python side. This is good.
else
  echo PyTorch not found on the python side.
  echo Please install PyTorch first. For example, you can install it with conda:
  echo "conda install pytorch torchvision cudatoolkit=10.2 -c pytorch", or
  echo with pip: "pip install torch torchvision". If you already have PyTorch
  echo installed somewhere else, you need to add it to your PATH.
  echo Note: you need to install higher version than PyTorch 1.1 to train Transformer models
  exit 1
fi

#if [ $stage -le 0 ]; then
#  local/pytorchnn/data_prep.sh $data_dir
#fi


LM=sw1_fsh_fg # Using the 4-gram const arpa file as old lm
if [ $stage -le 1 ]; then
  echo "$0: Perform nbest-rescoring on $ac_model_dir with a PyTorch trained $model_type LM."
  for decode_set in eval2000; do
      decode_dir=${ac_model_dir}/decode_${decode_set}_${LM}
        for d in ${decode_dir}_${decode_dir_suffix}; do grep Sum $d/scor*/*swbd*ys | utils/best_wer.sh; done
        for d in ${decode_dir}_${decode_dir_suffix}; do grep Sum $d/scor*/*callhm*ys | utils/best_wer.sh; done
  done
  for decode_set in rt02; do
      decode_dir=${ac_model_dir}/decode_${decode_set}_${LM}
        for d in ${decode_dir}_${decode_dir_suffix}; do grep Sum $d/scor*/*swbd1*ys | utils/best_wer.sh; done
        for d in ${decode_dir}_${decode_dir_suffix}; do grep Sum $d/scor*/*swbd2*ys | utils/best_wer.sh; done
        for d in ${decode_dir}_${decode_dir_suffix}; do grep Sum $d/scor*/*swbd3*ys | utils/best_wer.sh; done
  done
  for decode_set in rt03; do
      decode_dir=${ac_model_dir}/decode_${decode_set}_${LM}
        for d in ${decode_dir}_${decode_dir_suffix}; do grep Sum $d/scor*/*fsh*ys | utils/best_wer.sh; done
        for d in ${decode_dir}_${decode_dir_suffix}; do grep Sum $d/scor*/*swbd*ys | utils/best_wer.sh; done
  done


fi
exit 0
