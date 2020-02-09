# Cache-Simulator
Nov, 2018
<h3>Cache Design</h3>
<p>Read Miss: on a read miss, the cache issues a read request for the data from the lower level of the cache. Once the data is returned, it is placed in an empty way, if one exists, or data in one of the ways is evicted to create room for the new data.</p>
<ul>
	<li>The ways of the cache are numbered from {0,1,2..W-1} for a W-way cache. If an empty way exists, data is placed in lowest numbered empty way.</li>
	<li>Eviction is performed based on LRU policy. [Note, in this lab you are expected to emulate the exact LRU policy, not hierarchical LRU]</li>
</ul>
<p>Write Hit: both the L1 and L2 caches are write-through caches.</p>
<p>Write Miss: both the L1 and L2 caches are write no-allocate caches. On a write miss, the write request is forwarded to the lower level of the cache.</p>
<p>Non-Inclusive: the L1 and L2 caches are non-inclusive.</p>

<h3>Configuration File (config.txt):</h3>
<p>The parameters of the L1 and L2 caches are specified in a configuration file. The format of the configuration file is as follows.</p>
<ul>
	<li>Block size: Specifies the block size for the cache in bytes. This should always be a non-negative power of 2 (i.e., 1, 2, 4, 8, etc).</li>
	<li>Associativity: Specifies the associativity of the cache. A value of "1" implies a direct-mapped cache, while a "0" value implies fully-associative. Should always be a non-negative power of 2.</li>
	<li>Cache size: Specifies the total size of the cache data array in KB.</li>
</ul>

<h3>Trace File (trace.txt):</h3>
<p>The simulator takes as input a trace file that will be used to compute the output statistics. The trace file will specify all the data memory accesses that occur in the sample program. Each line in the trace file will specify a new memory reference. Each line in the trace cache will therefore have the following two fields:</p>
<ul>
	<li>Access Type: A single character indicating whether the access is a read (‘R') or a write ('W').</li>
	<li>Address: A 32-bit integer (in unsigned hexidecimal format) specifying the memory address that is being accessed.</li>
</ul>

<h3>Simulator Output:</h3>
<p>For each cache access, your simulator must output whether the access caused a read or write hit or miss in the L1 and L2 caches, or, in the L2 cache, it it was not accessed. Each event is coded with a number, as shown below.</p>
<ul>
	<li>0: No Access</li>
	<li>1: Read Hit</li>
	<li>2: Read Miss</li>
	<li>3: Write Hit</li>
	<li>4: Write Miss</li>
</ul>	