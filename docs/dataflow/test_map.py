from nose.tools import *
from map import *

def test_basic():
    tm = TMap()

    tm.insert(0, "H")
    assert_equal(tm.find(10), None)

    assert_equal(tm.find(0), 'H')

    assert_equal(tm.range(0, 4), ['H'])

    tm.insert(4, "E")

    assert_equal(tm.range(0, 4), ['H', 'E'])
    
    
    
