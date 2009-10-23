from nose.tools import *

from elementpipeline import * 
from elements import *

class RenderDebug(object):
    def __init__(self, tgtlist):
        self.lst = tgtlist

    def newset(self):
        self.lst.append([])
        
    def render(self, d):
        self.lst[-1].append(d)
    

def test_simple():

    tgtlist = []
    rd = RenderDebug(tgtlist)
    
    ts = TestSource("Test")
    vis = TestVis("Test", rd)

    vis.sink.connect_to(ts.source)
    

    rd.newset()
    vis.renderWindow(-1000, 1000)
    assert_equal(tgtlist, [[]])
    
    # now, add some data to the source
    ts.generateData(0, 1000)
    rd.newset()
    vis.renderWindow(-1000, 3000)
    print tgtlist

    for i in range(5):
        vis.process()
    
    rd.newset()
    vis.renderWindow(-1000, 1000)
    print tgtlist

    
    ts.generateData(1000, 2000)
    ts.generateData(10000, 11000)
    rd.newset()
    vis.renderWindow(-1000, 3000)
    print tgtlist

    for i in range(5):
        vis.process()
    
    print tgtlist

    rd.newset()
    vis.renderWindow(-1000, 11000)
    print tgtlist

    for i in range(5):
        vis.process()
    
    print tgtlist

