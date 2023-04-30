/*
OS Experiment 3
Date: 21/11/11
Author: 202031061299
*/
#include "stdlib.h"
#include "list"
#include "iostream"
#include "algorithm"
#include "climits"
using namespace std;
#define SIZE 1048576
#define SIZE_BLOCK 4096
#define NUM_BLOCK 256
#define SUCCEED 0
#define ERROR -1

class FreeSpace
{
private:
    /* data */
public:
    int startBlock;                          //起始块地址
    int endBlock;                            //终点块地址
    int size();                              //计算并返回块的大小
    FreeSpace(int startBlock, int endBlock); //构造函数
    bool FreeSpace::operator==(const FreeSpace &s)      //运算符'=='的重载
    {
        if (s.startBlock == this->startBlock && s.endBlock == this->endBlock)
            return true;
        return false;
    }
};
FreeSpace::FreeSpace(int _startBlock, int _endBlock)
{
    startBlock = _startBlock;
    endBlock = _endBlock;
}
int FreeSpace::size()
{
    return this->endBlock - this->startBlock + 1;
}

class SpaceAllocater
{
private:
    void allocate(FreeSpace f, int size); //本来想用此函数实现具体的分配操作，而下方的几个xxxAdapt函数仅仅用来查找一个合适的空间，以便进一步提高程序的内聚性；结果写着写着不小心忘了它的存在，留下一个小小的残念
    std::list<FreeSpace> freeSpaceList;   //使用C++ STL模板构建的空闲分区链表，装载有FreeSpace对象
public:
    SpaceAllocater();              //对象的构造函数
    int firstAdapt(int size);      //首次适应法
    int bestAdapt(int size);       //最佳适应法
    int worstAdapt(int size);      //最差适应法
    int free(int start, int size); //释放内存
    void showMemoryUseage();       //显示内存使用情况
    void showLinkList();           //打印空闲分区链表，用于调试和学习
};
SpaceAllocater::SpaceAllocater()
{
    freeSpaceList.push_front(FreeSpace(0, NUM_BLOCK - 1));
}
int SpaceAllocater::firstAdapt(int size)
{
    int flag = ERROR;            //存放分配结果，默认为失败
    if (size <= 0 || size > 256) //对分配命令进行检错
    {
        printf("ERROR: Invalid size %d.\n", size);
        return ERROR;
    }
    list<FreeSpace>::iterator ListIterator = freeSpaceList.begin(); //使用迭代器遍历链表
    while (ListIterator != freeSpaceList.end())
    {
        if ((*ListIterator).size() >= size) //如果首次遇到合适的内存空间
        {
            if ((*ListIterator).size() - size > 1) //如果分配后剩余空间大于一个块，则一分为二再分配
            {
                printf("Block %X to %X will be allocated.\n", (*ListIterator).startBlock, (*ListIterator).startBlock + size - 1);
                (*ListIterator).startBlock += size;
            }
            else //如果分配后剩余空间小于等于一个块，则全部分配
            {
                printf("Block %X to %X will be allocated.\n", (*ListIterator).startBlock, (*ListIterator).endBlock);
                ListIterator = this->freeSpaceList.erase(ListIterator);
            }
            flag = SUCCEED; // 分配成功
            break;
        }
        ListIterator++;
    }
    if (!!flag)
        printf("ERROR: No free space.\n");
    return flag;
}
int SpaceAllocater::bestAdapt(int size)
{
    if (size <= 0 || size > 256)
    {
        printf("ERROR: Invalid size %d.\n", size);
        return ERROR;
    }
    list<FreeSpace>::iterator ListIterator = freeSpaceList.begin();
    FreeSpace *bestAdaptPointer = nullptr;
    while (ListIterator != freeSpaceList.end())
    {
        if (size <= (*ListIterator).size())
        {
            if (bestAdaptPointer == nullptr)
            {
                bestAdaptPointer = &*ListIterator;
            }
            else if ((*ListIterator).size() < (*bestAdaptPointer).size())
            {
                bestAdaptPointer = &*ListIterator;
            }
        }
        ListIterator++;
    }
    if (bestAdaptPointer == nullptr)
    {
        printf("ERROR: No free space.\n");
        return ERROR;
    }
    if ((*bestAdaptPointer).size() - size > 1)
    {
        printf("Block %X to %X will be allocated.\n", (*bestAdaptPointer).startBlock, (*bestAdaptPointer).startBlock + size - 1);
        (*bestAdaptPointer).startBlock += size;
    }
    else
    {
        printf("Block %X to %X will be allocated.\n", (*bestAdaptPointer).startBlock, (*bestAdaptPointer).endBlock);
        this->freeSpaceList.remove(*bestAdaptPointer);
    }
    return SUCCEED;
}
int SpaceAllocater::worstAdapt(int size)
{
    if (size <= 0 || size > 256)
    {
        printf("ERROR: Invalid size %d.\n", size);
        return ERROR;
    }
    list<FreeSpace>::iterator ListIterator = freeSpaceList.begin();
    FreeSpace *bestAdaptPointer = nullptr;
    while (ListIterator != freeSpaceList.end())
    {
        if (size <= (*ListIterator).size())
        {
            if (bestAdaptPointer == nullptr)
            {
                bestAdaptPointer = &*ListIterator;
            }
            else if ((*ListIterator).size() > (*bestAdaptPointer).size())
            {
                bestAdaptPointer = &*ListIterator;
            }
        }
        ListIterator++;
    }
    if (bestAdaptPointer == nullptr)
    {
        printf("ERROR: No free space.\n");
        return ERROR;
    }
    if ((*bestAdaptPointer).size() - size > 1)
    {
        printf("Block %X to %X will be allocated.\n", (*bestAdaptPointer).startBlock, (*bestAdaptPointer).startBlock + size - 1);
        (*bestAdaptPointer).startBlock += size;
    }
    else
    {
        printf("Block %X to %X will be allocated.\n", (*bestAdaptPointer).startBlock, (*bestAdaptPointer).endBlock);
        this->freeSpaceList.remove(*bestAdaptPointer);
    }
    return SUCCEED;
}
int SpaceAllocater::free(int start, int size)
{
    int end = start + size - 1;
    if (size <= 0 || start < 0 || start >= SIZE_BLOCK || end <= 0 || end >= SIZE_BLOCK)
    {
        printf("ERROR: out of range.\n");
        return ERROR;
    }
    list<FreeSpace>::iterator ListIterator = freeSpaceList.begin();
    if (ListIterator == freeSpaceList.end())
    {
        FreeSpace newNode = FreeSpace(start, end);
        ListIterator = freeSpaceList.insert(ListIterator, 1, newNode);
    }
    while (ListIterator != freeSpaceList.end())
    {
        if ((start >= (*ListIterator).startBlock && start <= (*ListIterator).endBlock) ||
            (end >= (*ListIterator).startBlock && end <= (*ListIterator).endBlock) ||
            (start < (*ListIterator).startBlock && end > (*ListIterator).endBlock))
        {
            printf("ERROR: The free instruction contains unallocated space.\n");
            return ERROR;
        }
        if (start > (*ListIterator).startBlock && end > (*ListIterator).endBlock)
            break;
        ListIterator++;
    }
    ListIterator = freeSpaceList.begin();
    while (ListIterator != freeSpaceList.end())
    {
        if ((*ListIterator).endBlock < start)
        {
            if (start == (*ListIterator).endBlock + 1)
            {
                (*ListIterator).endBlock = end;
                start = (*ListIterator).startBlock;
                ListIterator++;
                if ((ListIterator != freeSpaceList.end()) &&
                    ((*ListIterator).startBlock == end + 1))
                {
                    (*ListIterator).startBlock = start;
                    ListIterator--;
                    freeSpaceList.erase(ListIterator);
                }
            }
            else
            {
                ListIterator++;
                FreeSpace newNode = FreeSpace(start, end);
                ListIterator = freeSpaceList.insert(ListIterator, 1, newNode);
                ListIterator++;
                if ((ListIterator != freeSpaceList.end()) &&
                    ((*ListIterator).startBlock == end + 1))
                {
                    (*ListIterator).startBlock = start;
                    ListIterator--;
                    freeSpaceList.erase(ListIterator);
                }
            }
            printf("%X to %X will be freed.\n", start, end);
            break;
        }
        else if ((*ListIterator).startBlock > end)
        {
            if ((*ListIterator).startBlock == end + 1)
            {
                (*ListIterator).startBlock = start;
            }
            else
            {
                FreeSpace newNode = FreeSpace(start, end);
                ListIterator = freeSpaceList.insert(ListIterator, 1, newNode);
            }
            printf("%X to %X will be freed.\n", start, end);
            break;
        }
        ListIterator++;
    }
    return SUCCEED;
}
void SpaceAllocater::showMemoryUseage()
{
    int spaceUsage[NUM_BLOCK];
    int unUsedBlocks = 0;
    float useage;
    for (int i = 0; i < NUM_BLOCK; i++)
    {
        spaceUsage[i] = 1;
    }
    list<FreeSpace>::iterator ListIterator = freeSpaceList.begin();
    while (ListIterator != freeSpaceList.end())
    {
        FreeSpace f = *ListIterator;
        for (int i = f.startBlock; i <= f.endBlock; i++) //生成空闲分区表
        {
            spaceUsage[i] = 0;
            unUsedBlocks++;
            //printf("%d unused \n", i);
        }
        ListIterator++;
    }
    useage = 1.0 - (1.0 * unUsedBlocks / NUM_BLOCK); //计算内存占用比
    printf("Useage:\n  ");
    for (int i = 0; i < 16; i++)
    {
        printf("%s", i < 16 * useage ? "■ " : "□ "); //形象地展示内存使用率
    }
    printf("  %.2lf%% Used\n", useage * 100);
    printf("Useage Map:\n  "); //形象地展示内存使用状况
    for (int i = 0; i < 16; i++)
    {
        printf("%X ", i);
    }
    printf("\n");
    for (int i = 0; i < 16; i++)
    {
        printf("%X ", i);
        for (int j = 0; j < 16; j++)
        {
            printf("%s", spaceUsage[i * 16 + j] == 1 ? "■ " : "□ ");
        }
        printf("\n", i);
    }
    return;
}
void SpaceAllocater::showLinkList()
{
    list<FreeSpace>::iterator ListIterator = freeSpaceList.begin();
    while (ListIterator != freeSpaceList.end())
    {
        FreeSpace f = *ListIterator;
        printf("A free Node starts at %X, ends at %X, size is %X;\n", f.startBlock, f.endBlock, f.size());
        ListIterator++;
    }
}

int main()
{
    SpaceAllocater s = SpaceAllocater();
    string op;
    int data1, data2;
    s.showMemoryUseage();
    while (true)
    {
        cin >> op;
        if (op == "exit")
            break;
        else if (op == "free")
        {
            scanf("%x %x", &data1, &data2);
            if (!data1 && !data2)
            {
                cout << "INVALID OPERATION" << endl;
            }
            else
            {
                s.free(data1, data2);
            }
        }
        else if (op == "firstadapt")
        {
            scanf("%x", &data1);
            if (!data1)
            {
                cout << "INVALID OPERATION" << endl;
            }
            else
            {
                s.firstAdapt(data1);
            }
        }
        else if (op == "bestadapt")
        {
            scanf("%x", &data1);
            if (!data1)
            {
                cout << "INVALID OPERATION" << endl;
            }
            else
            {
                s.bestAdapt(data1);
            }
        }
        else if (op == "worstadapt")
        {
            scanf("%x", &data1);
            if (!data1)
            {
                cout << "INVALID OPERATION" << endl;
            }
            else
            {
                s.worstAdapt(data1);
            }
        }
        else
        {
            cout << "INVALID OPERATION" << endl;
        }
        s.showLinkList();
        s.showMemoryUseage();
        cin.clear();
    }
    return 0;
}