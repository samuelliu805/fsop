#In Situ File Access Profiling
## Goal 
The goal for this tool is to track and vistualize file operations in a multi-threaded environment

## Current Progress
I achieved this using strace and tested this using sysbench. By parsing the output I can visualize the file operations.
* Using **sysbench**: 
An example command for running sysbench is: sysbench --test=fileio --file-total-size=1MB --file-test-mode=rndwr --max-time=2 --max-requests=0 --file-block-size=4K --file-num=64 --num-threads=6 run
I also tried using fio, but I found sysbench is a better tool in producing file operations.

* Running **Strace**: 
An example for running strace with time is:  strace -T -o thread.txt sysbench --test=fileio --file-total-size=1MB --file-test-mode=rndwr --max-time=5 --max-requests=0 --file-block-size=4K --file-num=4 --num-threads=3 run
The output file contains the information for all operations. The way I approach it is searching for open system call, and record all followings file operations (ie, read/write)with the same file descriptor number until close on that file descriptor is called.

## Problem need to solve
The problem with this approach is that strace contains large operation overhead. 

## Next step:
fix the bugs to recursively print the directory path so that we can vistualize it
