#!/usr/bin/env bash

# This script trains an RNN (including LSTM and GRU) or Transformer-based language model with PyTorch and performs N-best rescoring
stage=0
gpu=2
model_type=Transformer # LSTM, GRU or Transformer
embedding_dim=512 # 512 for Transformer (to reproduce the perplexities and WERs above)
hidden_dim=4096 # 512 for Transformer
nlayers=6 # 6 for Transformer
nhead=8 # for Transformer
learning_rate=0.1 # 0.1 for Transformer
seq_len=100
model_var=none # none for baseline
prior_path=steps/pytorchnn/prior/transformer # load pretrained prior model
prior=False # using pretrained model or not
mark=marks # save_path disctinct to uncover
inter_flag=0
inter_alpha=0.8
batch=32
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
decode_dir_suffix=pytorch-${model_type}-emb${embedding_dim}_hid${hidden_dim}_nly${nlayers}-${dropout}-${model_var}-${seq_len}-pre${prior}-${mark}-itpr${ratio}-ib${inter_flag}-${inter_alpha}-seg${seg_wise}-nbest${nbest}

#mkdir -p $data_dir
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
