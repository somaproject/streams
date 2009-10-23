from elementpipeline import *
import bisect

"""

The real question here is "what is the data structure that is returned by
the query function?"

What makes a data stream a data stream? What does our data look like?

1. can packets overlap? Yes, slightly
2. can two packets ever start at exactly the same time? no
   so we could use packet start time as a GUID
3. packets have time span, but can be of different lengths.

A stream can have a series of packets at [0, 10], [15, 20], and
that can be the totality of the available data for a packet range.

Query supports range? We would like to have data such that we know
we've seen all the data that's in a particular range.


How do we detect boundaries? How do we glue received sequences together?

1. perfectly-adjacent sequences are valid, that is, a sequence covering [0, 10] and [11, 20]
2. overlap is also valid? That is, responders can send sequences that they have sent
before.

Query(t1, t2) --> returns available sequences that overlap t1, t2. 

How do we handle buffers that are updating in time? For example, what if we are
very slowly updating a packet?

When would we want to do that?

-----
What happens when the source gets a packet with an overlapping ... something?

buffer1: [0, 10]
buffer2: [10, 20]

but buffer2 could also be [1, 10]

buffers have to be able to overlap

"""
import numpy as np

class TestSource(Source):
    """
    The test source has sequential data appended

    """
    
    def __init__(self, name):
        Source.__init__(self, name)
        self.src_timetree = timetree.default()
        
        self.source = self.createSource("default", self.get_data,
                                        self.getSourceTree)

        self.data_times = []
        self.data = {}

        self.stepsize = 10

    def getSourceTree(self):
        return self.src_timetree


    def generateData(self, t1, t2):
        """
        generate canonical, fake data from t1 to t2
        """
        d = np.arange(t1, t2, self.stepsize)

        b = Buffer(t1, t2)
        b.data = d

        bisect.insort_left(self.data_times, t1)
        
        self.data[t1] = b

        self.src_timetree.insert(t1, b)

    def reset(self):
        self.data_times = []
        self.data = {}

        self.src_timetree.clear()


    def get_data(self, t1, t2):
        """
        Returns the available list of data
        between t1 and t2

        """
        results = []
        i = bisect.bisect_left(self.data_times, t1)
        while i < len(self.data_times) and self.data_times[i] <=t2 :
            if self.data_times[i] >= t1:
                results.append(self.data[self.data_times[i]])
            i += 1
        return results


class TestWaveSource(Source):
    pass


class VisCache(object):

    def __init__(self):
        self.data = {}
        self.times = []

    def addData(self, t, data):
        bisect.insort_left(self.times, t)
        self.data[t] = data

    def findData(self, t1, t2):
        i = bisect.bisect(self.times, t1)

        res = []
        
        while i < len(self.times) and self.times[i] < t2:
            res.append(self.data[self.times[i]])

            i += 1
        return res

    
class TestVis(Vis):
    """
    The test vis element keeps a cache of the data
    that it has seen locally, as well as a matching
    time-series cache.

    Test vis does the following:
    1. render (t1, t2)
    2. this looks in the internal render cache, and then
    also places a request into the process() method
    to update the render cache if necessary

    

    """

    def __init__(self, name, renderengine):
        Vis.__init__(self,name)
        self.sink = self.createSink("default")
        self.timetree = timetree.default()

        self.renderCache = {}

        self.renderengine = renderengine

        self.viscache = VisCache()
        

    def reset(self):
        pass
        

    def renderWindow(self, t1, t2):
        """
        1. do we have data for t1, t2 in our cache?
        2. if not, look upstream

        """

        # find all the gaps in our current rendering

        data = self.viscache.findData(t1, t2)

        for d in data:
            self.renderengine.render(d)
        self.renderregion = (t1, t2)
        
    def process(self):
        """
        The primary render region loop

        1. find the top-level bin the region , and over each, compute
        the hash, and compare.
        
        """

        t1, t2 = self.renderregion

        # this should be recursive, but remember our stack is very limited

        startbin = self.timetree.get_bin(self.timetree.maxlevel, t1)
        endbin = self.timetree.get_bin(self.timetree.maxlevel, t2)

        difference = False
        diffbins = []


        for level in range(self.timetree.maxlevel, -1, -1):
            difference = False
            print "level =", level
            
            for bin_i in range(startbin, endbin +1):
                myhash = self.timetree.get_hash(level, bin_i)
                srchash = self.sink.src.get_time_hash(level, bin_i)

                if myhash != srchash:
                    if level == 0:
                        diffbins.append(bin_i)
                        difference = True
                    else:
                        sbs = self.timetree.get_sub_bins(level, bin_i)
                        startbin = sbs[0]
                        endbin = sbs[-1]
                        difference = True
                        
                        break
            if difference == False:
                # there are no differences, break!
                break
        # startbin, endbin should now point

        if difference:
            t1, t2 = self.timetree.get_bin_range(0, diffbins[0])

            print "requesting data", t1, t2
            data = self.sink.src.get_data(t1, t2)
            for d in data:
                self.timetree.insert(d.starttime, d)
                self.viscache.addData(d.starttime, d)
                
        


class FIRFilter(Filter):

    def __init__(self, length):
        """
        Compute the moving-window average of the filter for length N.

        Since our architecture is primarially "pull", we don't do this
        until we've received a query from upstream.
        

        we know that if we want LENGTH packets then we will need
        to go back a certain amount of time in history

        """
        
        

        self.sink = self.createSink("default")
        self.sink_timetree = timetree.default()
        
        self.src_timetree = timetree.default()
        self.source = self.createSource("default", self.get_data,
                                        self.get_source_tree)

    def get_source_tree(self):
        return self.src_timetree

    def get_data(self, t1, t2):
        """
        Returns the available list of data
        between t1 and t2

        """
        results = []
        i = bisect.bisect_left(self.data_times, t1)
        while i < len(self.data_times) and self.data_times[i] <=t2 :
            if self.data_times[i] >= t1:
                results.append(self.data[self.data_times[i]])
            i += 1
        return results


    def process(self):
        """
        1. the input was a request for (t1, t2)
        
        2. do we have some existing subset of t1, t2 in our input queue ? 

        """
        
