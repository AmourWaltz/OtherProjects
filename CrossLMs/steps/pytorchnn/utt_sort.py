import numpy as np
from collections import defaultdict

def resort_utterance(path):
    r"""Resort the utterances in time sequence.

    Assume the input file format is as following:
        en_4156-A_030185-030248 oh yeah
        en_4156-A_030470-030672 well i'm going to have mine and two more classes
        en_4156-B_029874-030166 everybody has a masters out here 
        en_4156-B_030297-030472 well it seems like it 
        ...

    Args:
        path (str): A file of nbest lists with the above format.

    Returns:
        Resorted utterance in time sequence.
        en_4156-A_030185-030248 oh yeah
        en_4156-B_029874-030166 everybody has a masters out here 
        en_4156-A_030470-030672 well i'm going to have mine and two more classes
        en_4156-B_030297-030472 well it seems like it 
        ...
        The utterance represented by a dictionary from string to a list of
        strings. The key is utterance id and the value is the sentence.
    """

    utt_seq = defaultdict()
    with open(path + '/text', 'r', encoding='utf-8') as f:
        for line in f:
            line = line.strip()
            try:
                utt_id, sent = line.split(' ', 1)
            except ValueError:
                utt_id = line
                sent = ' '
            key = utt_id.split('-', 1)[0]
            frame = utt_id.rsplit('_', 1)[1]
            time_s, time_e = frame.split('-', 1)
            time = int(time_s)
#             time = (int(time_s) + int(time_e)) / 2
            if key not in utt_seq:
                utt_seq[key] = [(time, utt_id, sent)]
                pass
            else:
                utt_seq[key].append((time, utt_id, sent))
                pass
            pass
        pass

    for key in utt_seq.keys():
        utt_sort = sorted(utt_seq[key])
        for (time, utt_id, sent) in utt_sort:
            with open(path + '/text_sort', 'a', encoding='utf-8') as f:
                f.write('%s %s\n' % (utt_id, sent))
                pass
            pass
        pass


resort_utterance('data/rt02')
