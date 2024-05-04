import csv
import math

filename = 'sampleData.csv'

def nthColumn(n):
    values = []
    with open(filename, 'r') as file:
        reader = csv.reader(file)
        for row in reader:
            values.append(row[n])
    return values


def percentError():
    rowExample = nthColumn(1)
    rowUser = nthColumn(3)
    percentError = []
    for i in range(len(rowExample)):
        percentError.append(int(abs((int(rowExample[i])-int(rowUser[i]))/int(rowUser[i])) * 100))
    return percentError

print(percentError())