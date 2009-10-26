"""

"""
import zlib
import bisect
import struct

import numpy as np

def default():
    """
    create default
    """

    return TimeTree(10000, 4, 8)


def crchashfunc(hl):
    d = np.array(hl, dtype=np.uint32)
    
    hash = zlib.crc32(d.tostring())
    
    return hash

def itemhash_func(t, val):
    
    s = struct.pack("ll", t, val.__hash__())
    
    hash = zlib.crc32(s)

    return hash

class TimeTree(object):

    def __init__(self, granularity, levelsize, maxlevel,
                 list_hash_func = crchashfunc,
                 item_hash_func = itemhash_func):
        """
        granularity: how large are bins at the lowest level
        levelsize: each level contains this many bins of the level below it
        maxlevel: what is the highest level?

        """

        self.granularity = granularity
        self.levelsize = levelsize
        self.maxlevel = maxlevel

        self.clear()
        
        self.list_hash_func = list_hash_func
        self.item_hash_func = item_hash_func

    def clear(self):

        self.times = []
        self.cache = {}
        self.hashtree = {}
        for i in range(0, self.maxlevel+1):
            self.hashtree[i] = {}


    def get_bin_range(self, level, binnum):
        """
        returns the bin endpoints, inclusive, for the binnum
        at this level

        """
        if level > self.maxlevel:
            raise Exception()
        

        if level == 0:
            g = self.granularity
        else:
            g = self.granularity * (level) * self.levelsize
        
        t1 = binnum * g
        t2 = (binnum + 1) * g - 1
        
        assert (t2-t1) == (g -1)
        return (t1, t2)
        
            
    def get_bin(self, level, t):
        """
        Returns the bin ID that


        """


        if level > self.maxlevel:
            raise Exception()

            
        if level == 0:
            g = self.granularity
        else:
            g = self.granularity * (level) * self.levelsize
        
        ti = t / g

        if t < 0:
            return ti
        else:
            return ti

    def get_sub_bins(self, level, bin):
        if level > self.maxlevel or level == 0:
            raise Exception()
        
        # get current bin time range
        t1, t2 = self.get_bin_range(level, bin)

        bi = self.get_bin(level - 1, t1)
        
        return [bi + x for x in range(self.levelsize)]

        

    def get_hash(self, level, bin):
        """
        Return the current hash at this level
        """
        assert level >= 0
        
        if bin not in self.hashtree[level]:
            return None
        else:
            return self.hashtree[level][bin]
        

    def compute_level_hash(self, hl):
        """
        compute the hash over a given list
        """
        # right now this is a stupid crc32

        return self.list_hash_func(hl)


    def get_data_times_in_range(self, t1, t2):
        """
        Return a list of all of the times of data


        """
        i = bisect.bisect_left(self.times, t1)

        res = []
        while i < len(self.times) and  self.times[i] >= t1:
            res.append(self.times[i])
            i += 1
        return res
    
        
    def insert(self, t, value):
        """
        right now, we assume that each value has a single start
        time.
        """
        if t in self.cache:
            if value in self.cache[t]:
                # re-putting a value we already have, so nothing changes
                return

        if t not in self.cache:
            bisect.insort_left(self.times, t)
            self.cache[t] = []


        bisect.insort_left(self.cache[t], value)


        # get all of the data points at level 0, and hash them
        b = self.get_bin(0, t)
        t1, t2 = self.get_bin_range(0, b)
        
        data_times = self.get_data_times_in_range(t1, t2)
        data = []
        for dt in data_times:
            for item in self.cache[dt]:
                data.append((item, dt))

        hashes = [self.item_hash_func(dt, d) for d, dt in data]

        self.hashtree[0][b] = self.compute_level_hash(hashes)
        
        # now, for each level, update the hash
        # walk up the tree...

        for level in range(1, self.maxlevel+1):
            bin = self.get_bin(level, t)
            # compute the hash at the level below
            hashes = [self.get_hash(level-1, bi) for bi in self.get_sub_bins(level, bin)]

            # now update the caches
            hash_to_zero = [0 if h == None else h for h in hashes]
            h = self.compute_level_hash(hash_to_zero)

            self.hashtree[level][bin] = h
        

