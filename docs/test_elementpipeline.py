from nose.tools import *
from elements import *
from elementpipeline import *


def test_TestSource():

    s = TestSource("HelloWorld")

    # add data

    s.addData(0, 10, "Hello World 1")
    s.addData(15, 25, "Hello World 2")
    s.addData(30, 40, "Hello World 3")
    s.addData(45, 60, "Hello World 4")

    # now create a new sequence
    
    
