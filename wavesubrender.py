#!/usr/bin/python
import numpy as n
import pylab


def lowest(data, i, j):
    # returns the index of the lowest point between i and j
    return n.argmin(data[i:j])

def highest(data, i, j):
    # returns the index of the lowest point between i and j
    return n.argmax(data[i:j])


N = 1000
var = 01.0
data = n.random.rand(N)*var + n.sin(n.arange(0, N).astype(float) / 100)

minpointlists = []
maxpointlists = [] 


M = 50
for i in range(N-M):
    minpointlists.append( i + lowest(data, i, i+M))
    maxpointlists.append( i + highest(data, i, i+M))

N1 =  len(n.unique(n.array(minpointlists)))
print "the 'downsampled' data has ", float(N1)/ N*100, "% of the points of the original"

pylab.plot(data)
pylab.plot(minpointlists, data[minpointlists], 'r')
pylab.plot(maxpointlists, data[maxpointlists], 'g')
pylab.show()


    
