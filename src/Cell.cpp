#include "stdafx.h"
#include "Cell.h"

Cell::Cell() :
    cellRow(0), cellCol(0),
    validChars(NULL), cellChar(' '),
    cellIcon(NULL), bitTable(0)
{
}

Cell::~Cell()
{
}

const char* Cell::ValidChars(const char* value)
{
    if (value)
    {
        for (size_t i = 0; i < strlen(value); i++)
            possible[value[i]] = true;
        validChars = value;
    }
    return validChars;
}

char Cell::CellChar(const char value)
{
    if (value != '\0')
    {
        for (size_t i = 0; i < strlen(validChars); i++)
            possible[validChars[i]] = false;
        possible[value] = true;
        cellChar = value;
    }
    return cellChar;
}

size_t Cell::CellRow(size_t row)
{
    if (row)
        cellRow = row;
    return cellRow;
}

size_t Cell::CellCol(size_t col)
{
    if (col)
        cellCol = col;
    return cellCol;
}

void* Cell::CellIcon(void* value)
{
    if (value)
        cellIcon = value;
    return cellIcon;
}

void Cell::SelectPossible(char value, bool isPossible)
{
    possible[value] = isPossible;
}

bool Cell::IsPossible(char value)
{
    return possible[value];
}

uint32_t Cell::PossibleTable(bool calculate)
{
    uint32_t ret = 0;
    size_t len = strlen(validChars);
    if (calculate || bitTable == 0)
    {
        if (len > 32) len = 32;
        for (size_t i = 0; i < len; i++)
            if (possible[validChars[i]])
                ret |= (1 << i);
        bitTable = ret;
    }
    return bitTable;
}

bool Cell::IsSolved()
{
    if (cellChar != ' ') return true;
    // there is one and only one possible value
    uint32_t ret = 0;
    for (size_t i = 0; i < strlen(validChars); i++)
        if (possible[validChars[i]])
            ret++;
    if ( ret == 1 )
    {
        for (size_t i = 0; i < strlen(validChars); i++)
            if (possible[validChars[i]])
                cellChar = validChars[i];
    }

    return (ret == 1 );
}

