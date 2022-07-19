#!/usr/bin/env bash
# Copyright 2012  Johns Hopkins University (author: Daniel Povey)
#           2020  Ke Li
#           2022  Boyang XUE


##################################################################################################
# This script trains an LSTM or Transformer based language model with PyTorch and performs N-best rescoring.
##################################################################################################
stage=0 # 0: Data preparation; 1: LM training; 2: PPL evaluation; 3: N-best rescoring.

##################################################################################################
# Model settings
##################################################################################################
model_type=LSTM # LSTM or Transformer
embedding_dim=1024 # 1024 for LSTM
hidden_dim=1024 # 1024 for LSTM
nlayers=2 # 2 for Transformer
nhead=8 # for Transformer
model_var=none # model variants: none for standard LSTM, bid for bi-LSTM

##################################################################################################
# Training settings
##################################################################################################
seq_len=128 # length of history context
gpu=2 # single gpu card No. 
batch=32 # number of batch size
learning_rate=5 # 5 for LSTM
epoch=64 # maximum epochs for training
dropout=0.2 # Default: 0.2
pretrain=False # using pretrained model or not
suffix=base # differentiate the language model with marginal difference

##################################################################################################
# Evaluation settings
##################################################################################################
ng_weight=1.0 # interpolation weight for the n-gram LM
nnlm_weight=0.6 # interpolation weight for the NNLM
cross_utt=0 # using cross-utterance representation or not
nbest=20 # number of nbest list. Default: 20
ng=sw1_fsh_fg # using the 4-gram const arpa file as old lm

. ./cmd.sh
. ./path.sh
. ./utils/parse_options.sh

set -e
export CUDA_VISIBLE_DEVICES=$gpu


##################################################################################################
# Path sets
##################################################################################################
# training data directory. data/swbd_word/ for word-level
level=word
data_dir=data/swbd_$level
# load pretrained prior model when pretrain=True is used
pretrain_dir=steps/pytorchnn/pretrain/transformer
# acoustic model decode set directory
ac_model_dir=exp/chain/cnn_tdnn1a_sp_ep6_specmaskonline
# language model directory to save
lm_model_dir=nnlms/pytorch-${model_type}-emb${embedding_dim}_hid${hidden_dim}_nly${nlayers}-drop${dropout}-${model_var}-len${seq_len}-pre${pretrain}-${level}_level-${suffix}
nn_model=$lm_model_dir/model.pt
# interpolated language model directory
lm_itpr_dir=nnlms/pytorch-LSTM-emb1024_hid1024_nly2-drop0.2-none-len128-preFalse-base
# evaluated ppl results to save
eval_dir=nnlms/pytorch-${model_type}-emb${embedding_dim}_hid${hidden_dim}_nly${nlayers}-drop${dropout}-${model_var}-len${seq_len}-pre${pretrain}-${level}_level-${suffix}/nnlm${nnlm_weight}-cross_utt${cross_utt}
# decoding directory to save
decode_dir_suffix=pytorch-${model_type}-emb${embedding_dim}_hid${hidden_dim}_nly${nlayers}-drop${dropout}-${model_var}-len${seq_len}-pre${pretrain}-${level}_level-${suffix}-ng_weight${ng_weight}-nnlm1_weight${nnlm_weight}-cross_utt${cross_utt}-${nbest}best

mkdir -p $eval_dir


##################################################################################################
# Check if PyTorch is installed to use with python
##################################################################################################

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


##################################################################################################
# Stage 0: Training data preparation.
##################################################################################################

# if [ $stage -le 0 ]; then
#  local/pytorchnn/data_prep.sh $data_dir
# fi


##################################################################################################
# Stage 1: Train a PyTorch neural network language model.
##################################################################################################

if [ $stage -le 1 ]; then
  echo "$0: Start $model_type language model training."
    python steps/pytorchnn/train.py --data $data_dir \
            --model $model_type \
            --emsize $embedding_dim \
            --nhid $hidden_dim \
            --nlayers $nlayers \
            --nhead $nhead \
            --lr $learning_rate \
            --dropout $dropout \
            --seq_len $seq_len \
            --clip 1.0 \
            --batch-size $batch \
            --epoch $epoch \
            --save $nn_model \
            --prior $pretrain \
            --prior_path $pretrain_dir \
            --model_var $model_var \
            --tied \
            --cuda > $lm_model_dir/train.log
fi


##################################################################################################
# Stage 2: Evaluate the PPL on the swbd test set with a PyTorch trained neural LM.
##################################################################################################

if [ $stage -le 2 ]; then
  echo "$0: Evaluate the PPL on the swbd test set with a PyTorch trained $model_type LM in $lm_model_dir/model.pt."
    python steps/pytorchnn/evaluate.py --inpfile $data_dir/test.txt \
            --outfile $eval_dir \
            --model_dir $lm_model_dir \
            --vocabulary $data_dir/words.txt \
            --model $model_type \
            --emsize $embedding_dim \
            --nhid $hidden_dim \
            --nlayers $nlayers \
            --nhead $nhead \
            --seq_len $seq_len \
            --model_var $model_var \
            --cross_utt $cross_utt \
            --nnlm_weight $nnlm_weight \
            --nnlm_itdir $lm_itpr_dir \
            --cuda > $eval_dir/eval.log
fi


##################################################################################################
# Stage 3: Perform nbest-rescoring on decoding files with a PyTorch trained neural LM.
#         The recipe is used for TDNN hybrid system based on Kaldi.
#         When applied on other acoustic conditions with different decoding systems,
#         set stage=6 and modify the nbest-list path in steps/pytorchnn/compute_nbest_scores.sh.
#         The scores of nbest-list can be found in ${decode_dir}_${decode_dir_suffix}/archives.1/lmwt.nn
##################################################################################################

if [ $stage -le 3 ]; then
  echo "$0: Perform nbest-rescoring on $ac_model_dir with a PyTorch trained $model_type LM."
  for decode_set in eval2000 rt02 rt03; do
      decode_dir=${ac_model_dir}/decode_${decode_set}_${ng}
      steps/pytorchnn/lmrescore_nbest_pytorchnn.sh \
        --stage 1 \
        --cmd "$decode_cmd --mem 4G" \
        --N $nbest \
        --model_dir $lm_model_dir \
        --model_type $model_type \
        --emsize $embedding_dim \
        --nhid $hidden_dim \
        --nlayers $nlayers \
        --nhead $nhead \
        --seq_len $seq_len \
        --model_var $model_var \
        --cross_utt $cross_utt \
        --nnlm_weight $nnlm_weight \
        --nnlm_itdir $lm_itpr_dir \
        $ng_weight data/lang_$ng $nn_model $data_dir/words.txt \
        data/${decode_set} ${decode_dir} \
        ${decode_dir}_${decode_dir_suffix}
  done
fi
exit 0

