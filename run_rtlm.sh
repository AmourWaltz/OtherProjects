rm steps/pytorchnn/eval2000*
rm steps/pytorchnn/rt0*
bash local/pytorchnn/run_nnlm_swbd_rtm_baseline.sh --gpu 2 --seq_len 128 --mark base16 --seg_wise 1 --nlayers 16
rm steps/pytorchnn/eval2000*
rm steps/pytorchnn/rt0*
bash local/pytorchnn/run_nnlm_swbd_rtm_baseline.sh --gpu 2 --seq_len 100 --mark base16 --seg_wise 1 --nlayers 16
#rm steps/pytorchnn/eval2000*
#rm steps/pytorchnn/rt0*
