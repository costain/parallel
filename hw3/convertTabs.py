with open('er.txt') as fin, open('enrol.txt', 'w') as fout:
    for line in fin:
        fout.write(line.replace('\t', ' '))
