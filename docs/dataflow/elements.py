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

class TestSource(Source):
    """
    The test source has sequential data appended

    """
    
    def __init__(self, name):
        Source.__init__(self, name)
        self.src_timetree = timetree.default()
        
        self.source = self.createSource("default", self.getSourceTree)

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

        return results


class TestVis(Vis):
    """
    The test vis element keeps a cache of the data
    that it has seen locally, as well as a matching
    time-series cache.

    """

    def __init__(self, name, renderengine):
        Vis.__init__(self,name)
        self.sink = self.createSink("default")
        self.src_timetree = timetree.default()

        self.renderCache = {}

        self.renderengine = renderengine

    def reset(self):
        pass
        

    def renderWindow(self, t1, t2):
        """
        1. do we have data for t1, t2 in our cache?
        2. if not, look upstream

        """

        # find all the gaps in our current rendering

        gaplists = self.findGaps(t1, t2)

        for g in gaplists:
            r = self.sink.get(g)

            self.add_sequence(r)

        data = self.findData(t1, t2)

        for d in data:
            self.renderengine.render(d)


    def findGaps(self, t1, t2):
        """

        Return all the gaps in our current cache between t1 and
        t2; that is, all the places we don't currently think
        we have data, but would like to

        """


    

