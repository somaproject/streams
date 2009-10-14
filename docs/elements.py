from elementpipeline import *
import bisect

"""

The real question here is "what is the data structure that is returned by
the query function?"

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


    

