
import bisect

class TMap(object):

    def __init__(self):
        self.times = []
        self.cache = {}

    def find(self, t):
        i = bisect.bisect(self.times, t)
        if t not in self.cache:
            return None
        return self.cache[t]

    def insert(self, t, d):
        bisect.insort(self.times, t)
        self.cache[t] = d

    def clear():
        pass
    
    def range(self, t1, t2):
        i = bisect.bisect_left(self.times, t1)

        out = []
        while i < len(self.times) and self.times[i] >= t1 and self.times[i] <= t2:
            out.append(self.cache[self.times[i]])

            i += 1
        return out


    
    
