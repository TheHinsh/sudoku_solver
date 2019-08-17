#pragma once
/* Contains:
** - information about a specific cell
** - 
*/
#include <map>
#include <iostream>
#include <fstream>
using namespace std;

class Cell
{
public:
    Cell();
    ~Cell();
    void   Setup(size_t row, size_t col, const char* valid);
    const char* ValidChars(const char* valid = NULL);
    char   CellChar(const char value = '\0');
    size_t CellRow(size_t row = 0);
    size_t CellCol(size_t col = 0);
    void*  CellIcon(void* value = NULL);
    void   SelectPossible(char value, bool isPossible);
    bool   IsPossible(char value);
    uint32_t PossibleTable(bool calculate = true);
    bool   IsSolved();
    bool   NewlySolved();
    void   Print(ofstream& ofs, size_t line, const char* func);


private:
    size_t cellRow, cellCol;
    const char* validChars;
    char cellChar; 
    void* cellIcon;
    uint32_t bitTable;
    bool newlySolved;
    map<char, bool> possible;
};

