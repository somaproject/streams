"""
This is our attempt to make sure our ideas about an element pipeline
are capable of working.


Request from a connected element are of the following form:

request(t1, t2):


returns a list of packets of times:

t1 : start time
t2 : end time


"""

import timetree


class TimeInterval(object):
    def __init__(self):
        self.start = 0
        self.end = 0

    def __len__(self):
        return self.end - self.start + 1

class Buffer(object):
    def __init__(self, starttime, endtime):
        self.starttime = starttime
        self.endtime = endtime

    def getEndTime(self):
        return self.endtime
    
    def __hash__(self):
        return hash(self.starttime) + hash(self.endtime)

    
    
class SourcePad(object):
    def __init__(self, parent, src_tree_func):
        self.src_tree_func = src_tree_func

    def get_time_hash(self, level, bin):
        return self.src_tree_func().get_hash(level, bin)

    def get_data(self, t1, t2):
        return self.src.get_data(t1, t2)
    

class SinkPad(object):
    def __init__(self, parent):

        self.parent = parent
        self.src == None
        
    def connect_to(self, src):
        self.src = src
        

class Element(object):

    def __init__(self):
        self.sourcepads = {}
        self.sinkpads = {}
        
    def createSource(self, name):
        self.sourcepads[name] = SourcePad(self)
        return self.sourcepads[name]
    
    def createSink(self, name):
        self.sinkpads[name] = SinkPad(self)
        return self.sinkpads[name]
    
class Source(Element):
    def __init__(self, name):
        Element.__init__(self)
        self.name = name

    def getData(self, padname, start, end):
        raise NotImplementedError()

    
class Filter(Element):
    def __init__(self, name):
        Element.__init__(self)


class Vis(Element):
    def __init__(self, name):
        Element.__init__(self)

    def renderWindow(self, t1, t2):
        raise NotImplementedError()


def connect(srcpad, sinkpad):
    sinkpad.connect_to(srcpad)
        
