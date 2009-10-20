Data Flow in the Soma Streams Application
===========================================

Data in Soma comes from the Soma backplane, which keeps a canonical
time source of monotonically-increasing timestamps.  We would like to
visualize 32 channels of data, each of which is roughly 4 ksps of
32-bit samples. This is rougly 60 MB/hour/channel, or rougly 11 GB of
data over six hours. We assume the workstation we're using is the best available
machine for rougly $5000, which result in 16 GB of ram and 200 MB/sec
of disk IO.

The data from a given channel is generally arranged into packets, 
these packets typically consist of a start time (timestamped) and
some number (say 128) of samples at a fixed sampling rate. Thus,
each packet defines data acquired over an interval [t1, t2]. Because
of sampling jitter and timestamp jitter, buffers can slightly overlap
or there can be gaps -- that is, any of the following are possible (times 
are fictional):

Ideal :  A: [11, 20], B: [21:30]
Gap:     A: [11, 20], B: [23, 32]
Overlap: A: [11, 20], B: [19, 28]
Simultaneous : A: [10, 15], B: [10, 17]

Sometimes packets can be discontinuous from the source, so that A
might be [11, 20] and B could be [51, 60] -- that is, there could have
been multiple buffers in that gap, but there are not. We will know
(through other mechanisms related to the Soma network protocol) if
this is the case.


GUI features
--------------

We would like to visualize this data over the course of a six hour experiment, 
including

- Looking continually at the newest data
- going back in time and looking at data from earlier in the experiment
- Zooming out and in smoothly, up to and including observing all six hours of data. 


We would also like to simultaneously perform transforms of some of
this data, for visualization. This brings us to the pipeline
architecture / interface.

Pipelines
--------------

A pipeline represents / constitutes roughly one channel's worth of data
and the transforms and visualizations that are occuring related to that channel. 
A pipeline can consist of four classes of elements interconnected in a DAG: Sources (S),
Filters (F), Triggers (T), and Visualizers (V). The typical connection for simply
observing a channel of data looks like: 

S -> V

If we would instead like to look at a filtered version of this data, you 
could imagine a connection like :

S -> F -> V

Of course, we might want to look at both the raw data and the filtered
data, prompting:

S --> F --> V
  |
  --> F --> V 

the visualizers are told by the main window to render their data
between a time window [t1, t2] onto the openGL context. There is
generally one pipeline per source channel of data, so we end up with
~32 pipelines.  The resulting pipeline DAGs generally have branching
factors of, at worst, 2 or 3.

Sorting of filtering operations
--------------------------------

An additional feature we desperately want to support is the ability
to manipulate a setting on the filter element in a pipeline. Consider a pipeline
like: 

S --> F --> V

This might be, for example, a visualization of a FIR low-pass-filtered version
of the source data. Assume that we are currently looking at data between 4:00 and
4:10 (that is, ten minutes of data four hours into the experiment). We are 
curious what the data look like filtered with some -different- settings of 
the filters. That is, we change a "property" on F. Somehow, now, 
F must refilter all of the data between 0:00 and 4:10 -- this may take some
time -- perhaps 60 seconds. We would like for the data between 4:00 and 4:10
to be filtered (processed) **first** to provide immediate visual feedback.


Additional implementation notes
--------------------------------

Computation cheap, memory expensive, disk prohibitively so. 

Models for Filters
=========================================
We are going to consider the generic "wave buffer", WaveBuffer_t, which looks
as follows:

struct WaveBuffer_t {
       timeid_t start; 
       float samplingrate; 
       std::vector<float> data;
}

These buffers in general arrive one after another, with slight overlap
or gaps.  We can of course compute the end time by using start +
data.size() * 1/samplingrate. 

FIR filter (moving average) 
----------------------------
We might want to use a Finite Impulse Response (FIR), with an arbitrary kernel. For the
purposes of this discussion, however, let's just assume that we're computing
a moving average of the last N samples. 

Initially, we have to deal with the notion that we're examining slightly discontinuous
data -- that is, if our time series is x[n], we will need to use a heuristic
to determine that that the last sample in Wave Buffer 1 and the first sample in Wave Buffer
Two are x[n] and x[n+1], respectively. 

For simplicity's sake, let's ignore this part of the problem, and
simply assume that each wavebuffer is exactly 128 samples long, and
that their adjacency is perfect (no gaps/overlap).

Spectrogram
----------------------------



Triggering
-----------------------------

For each incoming WaveBuffer we check if two adjacent samples exceed a threshold, and
if so place that into a list of triggered events. 



More formal or general solution?
=========================================

Split time into periods indexed by t \in Z, the result of partitioning
time up into T. 

Assume we just compute a hash over T intervals that consists
of an associative function of the data, h(d). Alternatively, we don't need
to be associative, but can keep track of the to-be-hashed data in a 
data structure. 

H(T_i) =  h({d_i}) for d_i \in T_i


Assume we keep a map of start_time, end_time, fingerprint, etc. 
And we keep the same of the data we receive.



What does the Standard operation look like
-------------------------------------------

1. Vis: render time (t1, t2): 

In a recursive manner,  vis attempts to figure out the region with "new data":
   2. Vis: compute internal hash for (t1, t2) -- value equals Q1 
   3. Vis: ask upstream to compute hash for (t1, t2): value equals Q2

   ultimately, vis determines that there's probably new data in [t1a, t2a]

Vis asks upstream for data in [t1a, t2a]:
  1. this data is not currently in upstream's cache, so it adds it to 
  the work queue, and will "get to it soon", under the expectation we're going to ask for
  some of this again soon. We return what we currently can, which is a subset
  of the data

  2. We return the subset of the data that is in our cache. 

How do we handle updates? That is, could we have a filter
that returned the _number_ of samples in a window T1, t2? We can 
if we also hash the results of the data...

-------------------------------------------------------
What does the pipeline look like? 

