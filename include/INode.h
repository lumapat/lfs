#ifndef INODE_H
#define INODE_H

#include <iostream>
#include <string>
#include <vector>
#include <string>
#include <cstring>

typedef struct {
    char filename[32] = {0};
    int filesize = 0;
    int dataPointers[128] = {-1};
} INodeInfo;

class INode
{
    public:
        INodeInfo info;
        ~INode();
        INode(std::string filenameIn);
        void setSize(int size) {info.filesize = size;}
        void addDataPointer(int blockNum);
        char* convertToString();

    private:
        int nextDataPointer;
};

#endif
