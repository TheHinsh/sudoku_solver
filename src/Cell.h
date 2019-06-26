#pragma once
/* Contains:
** - information about a specific cell
** - 
*/
#include <map>
using namespace std;

class Cell
{
public:
    Cell();
    ~Cell();
    const char* ValidChars(const char* valid = NULL);
    char   CellChar(const char value = '\0');
    size_t CellRow(size_t row = 0);
    size_t CellCol(size_t col = 0);
    void*  CellIcon(void* value = NULL);
    void   SelectPossible(char value, bool isPossible);
    bool   IsPossible(char value);
    uint32_t PossibleTable(bool calculate = true);
    bool   IsSolved();


private:
    size_t cellRow, cellCol;
    const char* validChars;
    char cellChar; 
    void* cellIcon;
    uint32_t bitTable;
    map<char, bool> possible;
};

