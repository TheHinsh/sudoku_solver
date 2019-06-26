#include "stdafx.h"
#include "Group.h"
#include <iostream>
#include <fstream>


Group::Group(size_t size) 
{
    cells.clear();
}

Group::~Group()
{
}

void Group::AddCell(Cell* cell)
{
    cells.push_back(cell);
}

void Group::Clear(void)
{
    cells.clear();
}
Cell* Group::GroupCell(size_t cellNo)
{
    if (cellNo >= cells.size()) return NULL;
    return cells.at(cellNo);
}

size_t Group::GroupSize(void) 
{ 
    return cells.size();
}

Cell* Group::ProcessGroup(ofstream& ofs, size_t group)
{
    size_t num_cells = cells.size();
    Cell* ret = NULL;
    const char* valid = cells.at(0)->ValidChars();
    uint32_t* pos = new uint32_t[num_cells];
    memset(pos, 0, sizeof(uint32_t)*num_cells);
    for (size_t cellNo = 0; cellNo < num_cells; cellNo++)
    {
        uint32_t possible = (cells.at(cellNo)->IsSolved()) ? 0 : cells.at(cellNo)->PossibleTable();
        for (size_t charSel = 0; charSel < num_cells; charSel++)
            pos[charSel] += (possible & (1 << charSel)) ? 1 : 0;
    }

    for (size_t cellNo = 0; cellNo < num_cells; cellNo++)
    {
        for (size_t charSel = 0; charSel < num_cells; charSel++)
        {
            if ((pos[charSel] == 1) && (cells.at(cellNo)->PossibleTable() & (1 << charSel)))
            {
                cells.at(cellNo)->CellChar(valid[charSel]);
                ret = cells.at(cellNo);
                cellNo = num_cells;
                break;
            }
        }
    }
    delete[] pos;
    return ret;
}

uint32_t Group::GroupBits(char value)
{
    uint32_t ret = 0;
    for (size_t cellNo = 0; cellNo < cells.size(); cellNo++)
    {
        if (cells.at(cellNo)->IsSolved()) continue;
        if( cells.at(cellNo)->IsPossible(value) )
            ret |= (1 << (cells.size()-cellNo-1));
    }
    return ret;
}

void Group::Print(ofstream& ofs, size_t group)
{
    streamsize wd = ofs.width();
    size_t num_cells = cells.size();
    const char* valid = cells.at(0)->ValidChars();
    uint32_t* pos = new uint32_t[num_cells];
    memset(pos, 0, sizeof(uint32_t)*num_cells);
    ofs << " " << hex << group + 1 << " |";
    for (size_t cellNo = 0; cellNo < num_cells; cellNo++)
    {
        uint32_t possible = cells.at(cellNo)->IsSolved()?0:cells.at(cellNo)->PossibleTable();
        ofs.width(4);
        ofs.fill(' ');
        ofs << hex << possible;
        ofs.width(wd);
        for (size_t charSel = 0; charSel < num_cells; charSel++)
            pos[charSel] += (possible & (1 << charSel)) ? 1 : 0;
    }
    ofs << endl;
    ofs << " " << group + 1 << " |";
    for (size_t charSel = 0; charSel < num_cells; charSel++)
    {
        ofs.width(4);
        ofs.fill(' ');
        ofs << hex << pos[charSel];
        ofs.width(wd);
    }
    ofs << endl;
    ofs << "Numbers: ";
    for (size_t charSel = 0; charSel < num_cells; charSel++)
    {
        if (pos[charSel] == 1)
        {
            ofs.width(4);
            ofs.fill(' ');
            ofs << dec << valid[charSel];
            ofs.width(wd);
        }
    }
    ofs << endl;
    delete[] pos;
}
