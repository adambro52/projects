CSci4061 Fall 2018 Project 3
Name: Gavin Celichowski, Adam Smith, Peter Van Dyke
X500: celic005, smit8796, vandy123
Group ID: 35

How to Compile: Use the included makefile and type 'make' into the shell.
% make

How to Run:
% ./web_server <port> <path> <num_dispatch> <num_workers> <dynamic_flag> <qlen> <cache_entries>
The server will be configurable in the following ways:
- port number can be specified (you may only use ports 1025 - 65535 by default)
- path is the path to your web root location from where all the files will be served
- num_dispatcher is how many dispatcher threads to start up (must be 1-100)
- num_workers is how many worker threads to start up (must be 1-100)
- dynamic_flag indicates whether the worker thread pool size should be static or dynamic. By
default, it should be 0.
- qlen is the fixed, bounded length of the request queue (must be 1-100)
- cache_entries is the number of entries available in the cache (must be 1-100)
**IMPORTANT NOTE: The file webserver_log MUST be located in the root directory passed as
argv[2], otherwise the program will not be able to open it, and hence the server will not log to a file.
The file is opened with the O_CREAT, but err on the side of caution and please include this file in the
testing folder ***

How the Program Works:
-The program begins in main by getting the arguments from the user, error checking them, and then
initiating a connection by calling init(port)
-Main then launches num_dispatch dispatch threads and num_workers worker threads, detaches the threads,
and then sits in a while(1) loop while the threads serve the requests
-The dispatch threads calls accept_connection in a while(1) loop, and then inserts any received requests
into the bounded buffer request queue using insert_item. The bounded buffer's insert and remove functions
are protected via lock and synchronized via locks and condition variables.
-The worker threads get items from the bounded buffer request queue, check the cache to see if the request
has been recently served and is saved in the heap, and then
  --Cache HIT (request in the cache): returns the result using the cached data
  --Cache MISS (request not in the cache): searches disk for the file the request is asking for, and then
  either finds the file, adds it to the cache, and then returns the result to the client, or it does not find
  then file and returns an error. Upon finding a file in a disk access, the program uses the stat struct to determine
  the file size and then mallocs a character array of that size which is then filled with the file's contents.
-After serving the request, the worker thread performs some cleanup and logs the request by printing log entries to
the terminal and the log file using the following format:
[threadId][reqNum][fd][Request string][bytes/error][time][Cache HIT/MISS]
-A signal handler is set up to handle ^C interrupts. The handler performs cleanup and then exits the process

Caching Policy:
-Our cache is an array of cache_entry_t structs, and uses a Least Recently Used (LRU) Eviction policy
-LRU is implemented by moving a cached entry to the front of the cache on a cache hit, and evicting the
entry at the last index of the array on a miss. The new entries are added to the front of the array.

Dynamically Changing Pool Size Policy:
-N/A

Member Contributions:
-Most of the work was done together using the Atom Teletype feature, so everyone helped design and write all aspects
of the program, but each of us had areas that we dedicated a little more time to
  -Adam: Caching and memory management
  -Gavin: Bounded buffer, disk accesses, and synchronization
  -Peter: Worker threads, timing, logging, and testing
