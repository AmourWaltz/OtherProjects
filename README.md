# Cross-Utterance-LMs

## Introduction:

Language models have been conventionally evaluated on the sentence level, which derives from the typical decoding procedure
which is carried out for each utterance independently. The utterance assumed to be a sentence is a natural unit for parallel decoding in an off-line speech recognition scenario.

Practically, multiple consecutive utterances can be part of the same discourse, which are potentially related to each other. In recent years, performance improvements have been reported by using neural network language models that can represent longer span history contexts across a range of ASR tasks. To this end, it is natural to consider language modeling with more powerful and robust cross-utterance representations.

The code is based on the Kaldi recipe and mainly implemented using PyTorch. A single GPU card is used for training. The detailed configurations and the training process for Cross-Utterance LMs are presented as follows

## Prerequisite:

Please run the following command and install the packages.
```shell
 pip install -r requirements.txt
```

## Implementation:

### Baseline Language Model Settings

|             | embedding_dim | hidden_dim | nlayers | learning_rate   | dropout  |
| ----------- | ------------- | ---------- | ------- | --------------- | -------- |
| LSTM        | 1024          | 1024       | 2       | 5               | 0.2      |
| Transformer | 512           | 4096       | 6       | 0.1             | 0.2      |

### Neural Language Model Training Steps

```shell
 # level: using word-level or bpe-level data set.
 # stage: 0 - Data preparation; 1 - LM training; 2 - PPL evaluation; 3 - N-best rescoring.

 # Train a baseline Transformer LM in bpe-level:
 bash local/pytorchnn/run_nnlm_trans_swbd.sh --stage 1 --gpu 1 --level bpe

 # Train a baseline LSTM LM in word-level:
 bash local/pytorchnn/run_nnlm_lstm_swbd.sh --stage 1 --gpu 2 --level word
```
