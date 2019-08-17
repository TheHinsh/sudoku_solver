#pragma once
#include <vector>
#include "Cell.h"
#include "Group.h"
#include <iostream>
#include <fstream>

using namespace std;
class Table
{
public:
    Table(const char* output, const char empty, const char* valid_chars = "123456789", size_t block_w_h = 3);
    ~Table();
    void SetupCells();
    void FillCells(const char* values = NULL);
    void RecurseTable();
    bool ProcessTable();
    bool IsSolved(size_t solve_count, ofstream& ofs) { if(solve_count == (num_rows*num_cols)) { ofs << "COMPLETE" << endl ; return true; } return false; }
    void Print(bool stdout = false);
    void PrintLine(char value);
    void PrintCharBitMap(size_t index);

    void SetChar(size_t row, size_t col, char value, bool verbose = false);
    void SetIcon(size_t row, size_t col, void* icon);
    char GetChar(size_t row, size_t col);
    void* GetIcon(size_t row, size_t col);
private:
    const char* filename;
    const char empty;
    const char* chars;
    size_t num_chars;
    size_t block_size;
    size_t num_rows, num_cols;
    size_t previous;
    Cell   **cells;
    Group  *Blocks;
    Group  *Rows;
    Group  *Cols;
    ofstream ofs;

    size_t BlockNum(size_t row, size_t col);
    void ResetGroups();
    void ClearBlk(size_t row, size_t col, char value);
    void ClearRow(size_t row, size_t col, char value);
    void ClearCol(size_t row, size_t col, char value);
};

