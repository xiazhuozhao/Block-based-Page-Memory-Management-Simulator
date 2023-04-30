# Block-based-Page-Memory-Management-Simulator
西南石油大学《操作系统》课程实验，选题为基于块的页式内存管理模拟器。实验使用C++ with Classes实现了一套模拟动态分区分配方案，模拟页式内存管理，总内存大小为1M。使用按块分配的方案，块大小取4K，总共256个块用于分配，用2位16进制数字表示块地址，忽略块内地址。内存空间使用空闲分区链表表示，链表使用C++ STL List模板方案实现。项目使用交互式方案设计。本人还构建了一套图形界面用于生动地展示每一个块的使用情况，未分配的块用空心方块表示，已分配的块用实心方块表示。

# 操作命令
1) firstadapt size：使用首次适应法分配size个内存块，size为16进制数字 
2) bestadapt size：使用最佳适应法分配size个内存块，size为16进制数字 
3) worstadapt size：使用最差适应法分配size个内存块，size为16进制数字 
4) free start size：释放掉以start为首块编号、size大小的内存块，参数均为16进制数字 
5) exit：退出 
