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

        self.source = self.createSource("default")

        self.data_times = []
        self.data = {}

    def addData(self, t1, t2, data):

        b = Buffer(t1, t2)
        b.data = data

        bisect.insort_left(self.data_times, t1)
        self.data[t1] = b
        


    def getData(self, padname, start, end):
        """
        Returns the ordered list of data sequences requested
        or some subset. 
        """

        if padname not in self.sourcepads:
            raise AttributeError()

        MAXSIZE = 20 # maximum number of elements / sequences to return

        results = []
        
        if padname == "default":
            # right now, we search the list to construct a time-sequence
            i = bisect.bisect_right(self.data_times, start)

            while i < len(self.data_times):
                buffer_start = self.data[t1].starttime
                buffer_end = self.data[t1].endtime

                if len(results) > 0:
                    if buffer_start > results[-1].endtime + self.seqhold:
                        # too big of a gap, this is cearly not a contiguous
                        # segment

                        
                key = self.data_times[i]

                if key > end:
                    break


                if results == []:
                    s = DataSequence(buffer_start, 0)
                    results.append(s)

                results[-1].endtime = buffer_end
                if buffer_end > end:
                    break
                    
                
                i += 1
                
                
            
            
class TestVis(Vis):

    def __init__(self, name, renderengine):
        Vis.__init__(self,name)
        self.sink = self.createSink("default")

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


    

