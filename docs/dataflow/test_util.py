"""
Situations to test:

"""
from timetree import *
from nose.tools import *
from util import * 
import numpy as np
    
def test_tt1():
    tt1 = TimeTree(100, 4, 8)
    tt2 = TimeTree(100, 4, 8)

    tt1.insert(0, "H")

    s = find_not_match(tt1, tt2, 0, 1000, 10)

    assert_equal(s, [0])

    s = find_not_match(tt1, tt2, 100, 1000, 10)

    assert_equal(s, [])


    print s

def test_tt2():
    """
    Randomly pick times, add datapoints, and
    check that we're getting the right bin
    """

    
    BINSIZE = 100
    tt1 = TimeTree(BINSIZE, 4, 8)
    tt2 = TimeTree(BINSIZE, 4, 8)

    np.random.seed(0)

    K = 10000
    s = np.random.randint(0, BINSIZE*100, K)

    for i in s:
        tt1.insert(i, "H")
        tt2.insert(i, "H")

        assert_equal(find_not_match(tt1, tt2, 0, BINSIZE*100, 10), [])

    # now, randomly pick a  time, add a datapoint,
    for k in range(K):
        t = np.random.randint(0, 1000000)

        # pick who we are going to insert into
        if np.random.randint(0, 2) == 0:
            first = True
        else:
            first = False

        val = k*1000
        if first:
            tt1.insert(t, val)
        else:
            tt2.insert(t, val)

        # figure out which bin this would have been
        b = t / BINSIZE

        assert_equal(find_not_match(tt1, tt2, 0, 1000000, 10), [b])

        if first:
            tt2.insert(t, val)
        else:
            tt1.insert(t, val)

