with open('ca.txt') as fin, open('ca_ready.txt', 'w') as fout:
    for line in fin:
        fout.write(line.replace('\t', ' '))
