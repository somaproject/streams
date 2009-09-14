Requirements: 
=============

Data sizes
----------

Experiments are ~6 hours

at 32 ksps, this means: 
32 k points/sec      128 kB/sec
1.9M points/min      8 MB / min
115M points/hr       460 MB/ hr

700M points total    3.5 GB total

GUI things we'd like to do
----------------------------

   * Support "maximum data size" (e.g. circular buffer) behavior, with
     user-selecting buffer size
   * Be able to support scrolling left and right in time
   * be able to support trigger jumping
   * render different things at different levels
   * 


Allowed resources
------------------
Each stream renderer can take / consume: 
128 MB RAM
32 MB on the GPU
arbitrary disk space
512 kB/sec disk IO

Input data guarantees
----------------------
Input data is in wave buffers
The time on each wave buffer is _monotonic_, but _not_ continuous
Sampling rates may vary across buffers


This suggests that: 
  1. each successive "scale" will need 4x fewer points
  2. we will need ~10 "scales" 

