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
    Cell* CellAt(size_t cellNo);
    size_t Size(void);

    Cell* Process(ofstream& ofs, size_t group);
    uint32_t Bits(char value);

    void Print(ofstream& ofs, size_t group);
    size_t Total();

private:
    vector<Cell*> cells;
};

