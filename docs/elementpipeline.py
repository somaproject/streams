"""
This is our attempt to make sure our ideas about an element pipeline
are capable of working.


Request from a connected element are of the following form:

request(t1, t2):


returns a list of packets of times:

t1 : start time
t2 : end time


"""

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
    

class DataSequence(object):
    """
    Represents a continuous sequence of data from T1 to T2
    
    """
    
    def __init__(self, t1, t2):
        self.t1 = t1
        self.t2 = t2
        self.data = {}

class SourcePad(object):
    def __init__(self, parent):
        pass


class SinkPad(object):
    def __init__(self, parent):
        pass
    

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



