#pragma once
#include <vector>

#include <iostream>
#include <fstream>
#include "Cell.h"

using namespace std;
class Group
{
public:
    Group(size_t size = 9);
    ~Group();

    void  AddCell(Cell* cell);
    void  Clear(void);
    Cell* GroupCell(size_t cellNo);
    size_t GroupSize(void);

    Cell* ProcessGroup(ofstream& ofs, size_t group);
    uint32_t GroupBits(char value);

    void Print(ofstream& ofs, size_t group);

private:
    vector<Cell*> cells;
};

