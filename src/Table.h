#pragma once
#include <vector>
#include "Cell.h"
#include "Group.h"

using namespace std;
class Table
{
public:
    Table(const char* output, const char* valid_chars = "123456789", size_t block_w_h = 3);
    ~Table();
    void SetupCells();
    bool ProcessTable();
    void Print();
    void PrintLine(ofstream& ofs, char value);

    void SetChar(size_t row, size_t col, char value);
    void SetIcon(size_t row, size_t col, void* icon);
    char GetChar(size_t row, size_t col);
    void* GetIcon(size_t row, size_t col);
private:
    const char* filename;
    const char* chars;
    size_t num_chars;
    size_t block_size;
    size_t num_rows, num_cols;
    Cell   **cells;
    Group  *Blocks;
    Group  *Rows;
    Group  *Cols;

    vector<size_t> masks;

    size_t BlockNum(size_t row, size_t col);
    void ResetGroups();
    void ClearBlk(size_t row, size_t col, char value);
    void ClearRow(size_t row, size_t col, char value);
    void ClearCol(size_t row, size_t col, char value);
};

