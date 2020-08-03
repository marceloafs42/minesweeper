#ifndef BINARY_H
#define BINARY_H

#include <iostream>
#include <vector>

class TBinary{
    public:

    int _length;
    std::vector<std::vector<int>> _finalVector;

    void GenerateBinary(std::vector<int> mylist, int pos);
};

#endif // BINARY_H
