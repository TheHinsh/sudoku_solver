#include "stdafx.h"
#include "Table.h"
#include <iostream>
#include <fstream>
using namespace std;

Table::Table(const char* output, const char* valid_chars, size_t block_w_h) :
    filename(output), 
    chars(valid_chars), num_chars(strlen(chars)),
    block_size(block_w_h),
    num_rows(num_chars), num_cols(num_chars)
{
    cells = new Cell *[num_rows];
    for (size_t row = 0; row < num_rows; row++)
        cells[row] = new Cell[num_cols];
    Blocks = new Group[block_size*block_size];
    Rows = new Group[num_rows];
    Cols = new Group[num_cols];
    ResetGroups();
    SetupCells();
    size_t mask = (1 << block_size) - 1;
    for (size_t blk = 0; blk < block_size; blk++)
        masks.push_back(mask << (block_size*blk));
}

Table::~Table()
{
    for (size_t row = 0; row < num_rows; row++)
        delete[] cells[row];
    delete[] cells;
	delete[] Blocks;
	delete[] Rows;
	delete[] Cols;
}

void Table::ResetGroups()
{
    for (size_t row = 0; row < num_rows; row++)
    {
        for (size_t col = 0; col < num_cols; col++)
        {
            Blocks[BlockNum(row, col)].AddCell(&cells[row][col]);
            Rows[row].AddCell(&cells[row][col]);
            Cols[col].AddCell(&cells[row][col]);
        }
    }
}

void Table::SetupCells()
{
    for (size_t row = 0; row < num_rows; row++)
    {
        for (size_t col = 0; col < num_cols; col++)
        {
            cells[row][col].ValidChars(chars);
            cells[row][col].CellRow(row);
            cells[row][col].CellCol(col);
        }
    }
    ofstream ofs;
    ofs.open(filename, ofstream::out | ofstream::trunc);
    ofs.close();
}

bool Table::ProcessTable( )
{
    size_t solve_count = 0;
    for (size_t row = 0; row < num_rows; row++)
    {
        for (size_t col = 0; col < num_cols; col++)
        {
            if (cells[row][col].IsSolved())
            {
                solve_count++;
                ClearRow(row, col, cells[row][col].CellChar());
                ClearCol(row, col, cells[row][col].CellChar());
            }
        }
    }

    ofstream ofs;
    ofs.open(filename, ofstream::out | ofstream::app);
    ofs << "solve_count " << solve_count << " total " << (num_rows*num_cols) << endl;
    for (size_t repeat = 0; repeat < 3; repeat++)
    {
        Cell* isSolved = NULL;
        for (size_t block = 0; block < block_size*block_size; block++)
        {
            isSolved = Blocks[block].ProcessGroup( ofs, block);
            if (isSolved)
                SetChar(isSolved->CellRow(), isSolved->CellCol(), isSolved->CellChar());
        }
        for (size_t row = 0; row < num_rows; row++) 
        {
            isSolved = Rows[row].ProcessGroup(ofs, row);
            if (isSolved )
                SetChar(isSolved->CellRow(), isSolved->CellCol(), isSolved->CellChar());
        }
        for (size_t col = 0; col < num_cols; col++)
        {
            isSolved = Cols[col].ProcessGroup(ofs, col);
            if (isSolved )
                SetChar(isSolved->CellRow(), isSolved->CellCol(), isSolved->CellChar());
        }
            // for each valid[char] check each cell in each block
        //   find !solved, and compare against that block row cells
    }
    streamsize wd = ofs.width();
    //ofs << "Block " << endl;
    //for (size_t value = 0; value < strlen(chars); value++)
    //{
    //    ofs << value+1 << " | ";
    //    for (size_t block = 0; block < block_size*block_size; block++)
    //    {
    //        ofs.width(4);
    //        ofs << hex << Blocks[block].GroupBits(chars[value]) << " ";
    //    }
    //    ofs << endl;
    //}
    //ofs << "Row " << endl;
    //for (size_t value = 0; value < strlen(chars); value++)
    //{
    //    ofs << value + 1 << " | ";
    //    for (size_t row = 0; row < num_rows; row++)
    //    {
    //        ofs.width(4);
    //        ofs << hex << Rows[row].GroupBits(chars[value]) << " ";
    //    }
    //    ofs << endl;
    //    ofs.width(wd);
    //}
    //ofs << endl;

    for (size_t value = 0; value < strlen(chars); value++)
    {
        ofs << value + 1 << endl;
        vector<vector<size_t> > blkRows;
        for (size_t blk = 0; blk < block_size; blk++)
        {
            vector<size_t> blkRow;
            for (size_t r = 0; r < block_size; r++)
            {
                for (size_t b = 0; b < block_size; b++)
                    blkRow.push_back(((Rows[blk*block_size + r].GroupBits(chars[value]) & masks[b]) >> (b*block_size)));
            }
            blkRows.push_back(blkRow);
        }
        for (size_t r = 0; r < blkRows.size(); r++)
        {
            ofs << r << " | ";
            for (size_t b = 0; b < blkRows[r].size(); b++)
            {
                ofs.width(4);
                ofs << blkRows[r][b] << " ";
            }
            ofs << endl;
            ofs.width(wd);
        }

        ofs << endl;
    }
    //ofs << "Col " << endl;
    //for (size_t value = 0; value < strlen(chars); value++)
    //{
    //    ofs << value+1 << " | ";
    //    for (size_t col = 0; col < num_cols; col++)
    //    {
    //        ofs.width(4);
    //        ofs << hex << Cols[col].GroupBits(chars[value]) << " ";
    //    }
    //    ofs << endl;
    //    ofs.width(wd);
    //}
    ofs.close();
    return (solve_count == (num_rows*num_cols));
}

void Table::Print()
{
    ofstream ofs;
    ofs.open(filename, ofstream::out | ofstream::app);

    ofs << "Table: " << num_chars << " x " << num_chars << endl;
    PrintLine(ofs, '=');

    for (size_t row = 0; row < num_rows; row++)
    {
        ofs << row + 1 << " | ";
        for (size_t col = 0; col < num_cols; col++)
        {
            ofs.fill(' ');
            ofs.width(3);
            ofs << cells[row][col].CellChar();
            if ((col+1) % block_size)
                ofs << "  ";
            else
                ofs << " |";
        }
        ofs << endl;
        if (((row + 1) % block_size) == 0)
            PrintLine(ofs, '-');
    }
    PrintLine(ofs, '=');
    for (size_t row = 0; row < num_rows; row++)
    {
        ofs << row + 1 << " | ";
        for (size_t col = 0; col < num_cols; col++)
        {
            streamsize wd = ofs.width();
            ofs.width(4);
            if (cells[row][col].IsSolved())
            {
                ofs.fill('*');
                ofs << cells[row][col].CellChar();
            }
            else
            {
                ofs.fill(' ');
                ofs << right << hex << cells[row][col].PossibleTable();
            }
            if (((col + 1) % block_size) == 0)
                ofs << "|";
            else
                ofs << " ";
            ofs.width(wd);
        }
        ofs << endl;
        if (((row + 1) % block_size) == 0)
            PrintLine(ofs, '-');
    }
    ofs << endl;
    ofs.close();
}

void Table::PrintLine(ofstream& ofs, char value)
{
    streamsize wd = ofs.width();
    ofs << "   ";
    ofs.width(5 * num_cols);
    ofs.fill(value);
    ofs << value;
    ofs << endl;
    ofs.width(wd);
}
void Table::SetChar(size_t row, size_t col, char value) 
{
    ClearRow(row, col, value);
    ClearCol(row, col, value);
    ClearBlk(row, col, value);
    cells[row][col].CellChar(value);
}

void Table::SetIcon(size_t row, size_t col, void* icon) 
{ 
    cells[row][col].CellIcon(icon);
}

char Table::GetChar(size_t row, size_t col)
{
    return cells[row][col].CellChar();
}

void* Table::GetIcon(size_t row, size_t col)
{
    return cells[row][col].CellIcon();
}

size_t Table::BlockNum(size_t row, size_t col)
{
    size_t cols_blk = num_cols / block_size;
    size_t rows_blk = num_rows / block_size;
    size_t blk_num = 0;
    blk_num += (row / rows_blk)*cols_blk;
    blk_num += (col / cols_blk);
    return blk_num;
}

void Table::ClearBlk(size_t row, size_t col, char value)
{
    size_t blk_num = BlockNum(row, col);
    for (size_t r = 0; r < num_rows; r++)
        for (size_t c = 0; c < num_cols; c++)
            if (blk_num == BlockNum(r, c))
                cells[r][c].SelectPossible(value, false);
}

void Table::ClearRow(size_t row, size_t col, char value)
{
    for (size_t r = 0; r < num_rows; r++)
        cells[r][col].SelectPossible(value, false);
}

void Table::ClearCol(size_t row, size_t col, char value)
{
    for (size_t c = 0; c < num_cols; c++)
        cells[row][c].SelectPossible(value, false);
}
