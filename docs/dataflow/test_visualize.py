from matplotlib import pyplot
from matplotlib import collections, axes, transforms


from elementpipeline import * 
from elements import *

class RenderDebug(object):
    def __init__(self, tgtlist):
        self.lst = tgtlist

    def newset(self):
        self.lst.append([])
        
    def render(self, d):
        self.lst[-1].append(d)
    

def render_buffers(ax, ep):
    lc = []
    for e in ep:
        # add vertical ticks
        segs = [ (e[0][0], e[0][1] + 0.5), e[0], e[1], (e[1][0], e[1][1] + 0.5)]
        lc.append(segs)

    ax.add_collection(collections.LineCollection(lc))
    
def test_simple():

    tgtlist = []
    rd = RenderDebug(tgtlist)
    
    ts = TestSource("Test")
    vis = TestVis("Test", rd)

    vis.sink.connect_to(ts.source)
    
    fig = pyplot.figure()
    srca = fig.add_subplot(1,2,1)
    visa = fig.add_subplot(1,2,2)
    
    for i in range(100):

        
        ts.generateData(i * 1000, 1000 * (i+1)-1)
        lc = [[(float(d.starttime), float(i)),
               ( float(d.endtime), float(i))] for k, d in ts.data.iteritems()]
        #print lc
        render_buffers(srca, lc)

        rd.newset()
        vis.renderWindow(-100000, 100000)

        lc = [[(b.starttime, i), (b.endtime, i)] for b in rd.lst[-1]
              ]
        render_buffers(visa, lc)

        # plot the most recent render set
        for k in range(5):
            vis.process()
        
    srca.axis([0, 100000, 100, 0])
    visa.axis([0, 100000, 100, 0])
    
    pyplot.show()
    

test_simple()
