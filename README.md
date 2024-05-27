通过C++编写的Cache模拟器，输入trace文件，输出Cache执行过程中的统计信息，包括读Cache次数、写Cache次数、命中次数、失效次数、Cache块替换次数、写回内存次数。本模拟器采用LRU和组相联算法，写失效策略为写分配，支持对Cache大小、块大小、相联度进行自定义。
编译命令：g++ main.cpp -o CacheSim -std=c++11
使用命令：./CacheSim <trace_input_file> <cache_size> <cache_block_size> <cache_associativity>
其中<trace_input_file>为trace文件路径，<cache_size>为Cache大小，<cache_block_size>为Cache块的大小，<cache_associativity>为相联度。所有大小相关的参数单位均为Byte。
使用示例：./CacheSim ./tracefiles/085.gcc.din 16384 8 4