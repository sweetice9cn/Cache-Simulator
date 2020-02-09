# Cache-Simulator
Nov, 2018
<h3>Cache Design</h3>
<p>Read Miss: on a read miss, the cache issues a read request for the data from the lower level of the cache. Once the data is returned, it is placed in an empty way, if one exists, or data in one of the ways is evicted to create room for the new data.</p>
 The ways of the cache are numbered from {0,1,2..W-1} for a W-way cache. If an empty way exists, data is placed in lowest numbered empty way.
 Eviction is performed based on LRU policy. [Note, in this lab you are expected to emulate the exact LRU policy, not hierarchical LRU]
<p>Write Hit: both the L1 and L2 caches are write-through caches.</p>
<p>Write Miss: both the L1 and L2 caches are write no-allocate caches. On a write miss, the write request is forwarded to the lower level of the cache.</p>
<p>Non-Inclusive: the L1 and L2 caches are non-inclusive.</p>

Configuration File (config.txt):
The parameters of the L1 and L2 caches are specified in a configuration file. The format of the configuration file is as follows.
 Block size: Specifies the block size for the cache in bytes. This should always be a non-negative power of 2 (i.e., 1, 2, 4, 8, etc).
 Associativity: Specifies the associativity of the cache. A value of "1" implies a direct-mapped cache, while a "0" value implies fully-associative. Should always be a non-negative power of 2.
 Cache size: Specifies the total size of the cache data array in KB.

Trace File (trace.txt):
Your simulator will need to take as input a trace file that will be used to compute the output statistics. The trace file will specify all the data memory accesses that occur in the sample program. Each line in the trace file will specify a new memory reference. Each line in the trace cache will therefore have the following two fields:
 Access Type: A single character indicating whether the access is a read (‘R') or a write ('W').
 Address: A 32-bit integer (in unsigned hexidecimal format) specifying the memory address that is being accessed.
Fields on the same line are separated by a single space.
The skeleton code provided reads the trace file one line at a time in order. After each access, your code should emulate the impact of the access on the cache hierarchy.

Simulator Output:
For each cache access, your simulator must output whether the access caused a read or write hit or miss in the L1 and L2 caches, or, in the L2 cache, it it was not accessed. Each event is coded with a number, as shown below.
0: No Access
1: Read Hit
2: Read Miss
3: Write Hit
4: Write Miss