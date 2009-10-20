from nose.tools import *
import numpy as np

from timetree import *


def test_range():
    g = 1000
    l = 4
    m = 8
    tt = TimeTree(g, l, m)

    (t1, t2) = tt.get_bin_range(0, 0)
    assert_equal(t1, 0)
    assert_equal(t2, g-1)

    (t1, t2) = tt.get_bin_range(0, 1)
    assert_equal(t1, g)
    assert_equal(t2, 2*g-1)

    (t1, t2) = tt.get_bin_range(0, -1)
    assert_equal(t1, -g)
    assert_equal(t2, -1)

    (t1, t2) = tt.get_bin_range(1, 0)
    assert_equal(t1, 0)
    assert_equal(t2, 4*g-1)

    (t1, t2) = tt.get_bin_range(1, 1)
    assert_equal(t1, 4*g)
    assert_equal(t2, 8*g-1)

    (t1, t2) = tt.get_bin_range(1, -1)
    assert_equal(t1, -4*g)
    assert_equal(t2, -1)


def test_bin():

    g = 1000
    l = 4
    m = 8
    tt = TimeTree(g, l, m)

    assert_equal(tt.get_bin(0, 0), 0)
    assert_equal(tt.get_bin(0, g-1), 0)
    assert_equal(tt.get_bin(0, -1), -1)

    
    # randomized test
    
    N = 10000
    for i in range(N):
        # pick level
        l = np.random.randint(0, m+1)

        bin = np.random.randint(-10000, 10000)

        t1, t2 = tt.get_bin_range(l, bin)

        for j in range(10):
            t = np.random.randint(t1, t2+1)
            assert_equal(bin, tt.get_bin(l, t))
    

def test_sub():
    g = 1000
    l = 4
    m = 8
    tt = TimeTree(g, l, m)

    assert_equal(tt.get_sub_bins(1, 0), [0, 1, 2, 3])
    assert_equal(tt.get_sub_bins(1, 1), [4, 5, 6, 7])
    
    assert_equal(tt.get_sub_bins(2, 0), [0, 1, 2, 3])

def get_data_in_range():

    g = 1000
    l = 4
    m = 8
    tt = TimeTree(g, l, m, hashfunc)

    tt.insert(10, 0)

    assert_equal(tt.get_times_in_range(8, 9), [])
    assert_equal(tt.get_times_in_range(8, 10), [10])
    assert_equal(tt.get_times_in_range(9, 10), [10])
    assert_equal(tt.get_times_in_range(9, 11), [10])
    assert_equal(tt.get_times_in_range(10, 11), [10])

    tt.insert(20, 0)
    
    assert_equal(tt.get_times_in_range(10, 20), [10, 20])

    

def test_hash_simple():

    def hashfunc(hl):
        return np.sum(hl)

    def item_hash_func(t, d):
        return t # this is a nice wrong way to do this, oh well
    
    g = 1000
    l = 4
    m = 8
    tt = TimeTree(g, l, m, hashfunc, item_hash_func)

    assert_equal(None, tt.get_hash(1, 10))

    assert_equal(None, tt.get_hash(0, 0))

    tt.insert(0, "Hello")

    assert_equal(tt.get_hash(0, 0), 0)
    tt.insert(10, "World")
    assert_equal(tt.get_hash(0, 0), 10)

    for i in range(m+1):
        print i
        assert_equal(tt.get_hash(i, 0), 10)
        
    
    tt.insert(1010, "World")
    assert_equal(tt.get_hash(0, 0), 10)
    assert_equal(tt.get_hash(1, 0), 1020)
    
