#include "stdafx.h"
#include "Table.h"

using namespace std;

Table::Table(const char* output, const char empty, const char* valid_chars, size_t block_w_h) :
    filename(output), 
    empty(empty),
    chars(valid_chars), num_chars(strlen(chars)),
    block_size(block_w_h),
    num_rows(num_chars), num_cols(num_chars),
    previous(0)
{
    // clear out the output file. :)
    ofs.open(filename, ofstream::out | ofstream::trunc);
    ofs.close();
    ofs.open(filename, ofstream::out | ofstream::app);

    cells = new Cell *[num_rows];
    for (size_t row = 0; row < num_rows; row++)
        cells[row] = new Cell[num_cols];
    Blocks = new Group[block_size*block_size];
    Rows = new Group[num_rows];
    Cols = new Group[num_cols];
    ResetGroups();
    SetupCells();
}

Table::~Table()
{
    for (size_t row = 0; row < num_rows; row++)
        delete[] cells[row];
    delete[] cells;
	delete[] Blocks;
	delete[] Rows;
	delete[] Cols;
    ofs.close();
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
            cells[row][col].Setup(row, col, chars);
        }
    }
}

void Table::FillCells(const char* values)
{
    size_t offset = 0;
    if (!values) return;
    for (size_t row = 0; row < num_rows; row++)
    {
        for (size_t col = 0; col < num_cols; col++)
        {
            if(values[offset] != empty)
            {
                SetChar(row, col, values[offset]);
                if (cells[row][col].NewlySolved())
                {
                    // cells[row][col].Print(ofs, __LINE__, __FUNCTION__);
                    RecurseTable();
                }
            }
            offset++;
        }
    }
    ofs << __FUNCTION__ << " complete" << endl;
}

void Table::RecurseTable()
{
    for (size_t row = 0; row < num_rows; row++)
    {
        for (size_t col = 0; col < num_cols; col++)
        {
            if(cells[row][col].IsSolved())
            {
                SetChar(row, col, cells[row][col].CellChar());
                if(cells[row][col].NewlySolved())
                {
                    // cells[row][col].Print(ofs, __LINE__, __FUNCTION__);
                    RecurseTable();
                }
            }
        }
    }
}

bool Table::ProcessTable( )
{
    size_t solve_count = 0;
    size_t num_cells = num_rows * num_cols;
    size_t max_total = 0;
    for(size_t i = 0; i < num_chars; i++)
    {
        max_total += chars[i] - chars[0] + 1;
    }
    ofs << "Starting " << __FUNCTION__ << endl;
    for (size_t row = 0; row < num_rows; row++)
    {
        for (size_t col = 0; col < num_cols; col++)
        {
            if (cells[row][col].IsSolved())
            {
                solve_count++;
                SetChar(row, col, cells[row][col].CellChar());
                if(cells[row][col].NewlySolved())
                {
                    // cells[row][col].Print(ofs, __LINE__, __FUNCTION__);
                    RecurseTable();
                }
            }
        }
    }
    if (IsSolved(solve_count, ofs)) return true;

    ofs << "solve_count " << solve_count << " total " << num_cells << endl;
    Cell* isSolved = NULL;
    solve_count = 0;
    for (size_t row = 0; row < num_rows; row++) 
    {
        // ofs << "Row " << row+1 << " total " << Rows[row].Total() << endl;
        isSolved = Rows[row].Process(ofs, row);
        solve_count += (isSolved)?1:0;
        if (isSolved )
        {
            SetChar(isSolved->CellRow(), isSolved->CellCol(), isSolved->CellChar());
            if(isSolved->NewlySolved())
            {
                // isSolved->Print(ofs, __LINE__, __FUNCTION__);
                RecurseTable();
            }
        }
    }
    if (IsSolved(solve_count, ofs)) return true;
    solve_count = 0;
    for (size_t col = 0; col < num_cols; col++)
    {
        // ofs << "Col " << col+1 << " total " << Cols[col].Total() << endl;
        isSolved = Cols[col].Process(ofs, col);
        solve_count += (isSolved)?1:0;
        if (isSolved )
        {
            SetChar(isSolved->CellRow(), isSolved->CellCol(), isSolved->CellChar());
            if(isSolved->NewlySolved())
            {
                // isSolved->Print(ofs, __LINE__, __FUNCTION__);
                RecurseTable();
            }
        }
    }
    if (IsSolved(solve_count, ofs)) return true;
    solve_count = 0;
    for (size_t block = 0; block < block_size*block_size; block++)
    {
        // ofs << "Blk " << block+1 << " total " << Blocks[block].Total() << endl;
        isSolved = Blocks[block].Process( ofs, block);
        solve_count += (isSolved)?1:0;
        if (isSolved)
        {
            SetChar(isSolved->CellRow(), isSolved->CellCol(), isSolved->CellChar());
            if(isSolved->NewlySolved())
            {
                // isSolved->Print(ofs, __LINE__, __FUNCTION__);
                RecurseTable();
            }
        }
    }
    if(previous == solve_count)
    {
        ofs << "Unable to solve any more" << endl;
        for (size_t c = 0; c < num_chars ; c++)
        {
            // bitmap for this char
            // PrintCharBitMap(c);
            streamsize wd = ofs.width();
            for (size_t row = 0; row < block_size*block_size; row+=block_size)
            {
                for (size_t col = 0; col < block_size*block_size; col+=block_size)
                {
                    size_t myblk = BlockNum(row,col);
                    size_t blkBits = Blocks[myblk].Bits(chars[c]);
                    for(size_t oneBit = 0; oneBit < num_chars; oneBit++)
                    {
                        Cell* theOne = Blocks[myblk].CellAt(oneBit);
                        if(blkBits == (1 << (num_chars - oneBit - 1)) )
                        {
                            SetChar(theOne->CellRow(), theOne->CellCol(), chars[c]);
                            if(theOne->NewlySolved())
                            {
                                theOne->Print(ofs, __LINE__, __FUNCTION__);
                                RecurseTable();
                            }
                        }
                    }
                    ofs.width(4);
                    ofs.fill(' ');
                    // ofs << hex << blkBits << ' ';
                    size_t sBits = blkBits;
                    size_t off = 0;
                    size_t ctst1 = (0 << (2*block_size) | 1 << block_size | 1);
                    size_t ctst2 = (1 << (2*block_size) | 0 << block_size | 1);
                    size_t ctst3 = (1 << (2*block_size) | 1 << block_size | 1);
                    // size_t ctst4 = (1 << (2*block_size) | 1 << block_size | 0);
                    while((sBits & 1) == 0)
                    {
                        sBits >>= 1;
                        off++;
                    }
                    if(sBits == ctst1 || sBits == ctst2 || sBits == ctst3)
                    {
                        size_t colClear = col + block_size - (off%block_size) - 1;
                        for (size_t r = 0; r < num_rows; r++)
                        {
                            if(BlockNum(r,colClear) != myblk)
                            {
                                cells[r][colClear].SelectPossible(chars[c], false); 
                            }
                        }
                        RecurseTable();
                        // PrintCharBitMap(c);
                    }
                    off = 0;
                    sBits = blkBits;
                    size_t rtst1 = (0 << 2 | 1 << 1 | 1);
                    size_t rtst2 = (1 << 2 | 0 << 1 | 1);
                    size_t rtst3 = (1 << 2 | 1 << 1 | 1);
                    // size_t rtst4 = (1 << 2 | 1 << 1 | 0);
                    while((sBits & 1) == 0)
                    {
                        sBits >>= 1;
                        off++;
                    }
                    if(sBits == rtst1 || sBits == rtst2 || sBits == rtst3)
                    {
                        size_t rowClear = row + block_size - (off/block_size) -1;
                        for (size_t column = 0; column < num_cols; column++)
                        {
                            if(BlockNum(rowClear,column) != myblk)
                            {
                                cells[rowClear][column].SelectPossible(chars[c], false); 
                            }
                        }
                        RecurseTable();
                        // PrintCharBitMap(c);
                    }
                    // ofs << endl;
                }
            }
            ofs.width(wd);
        }
    }
    previous = solve_count;
    Print();
    return (solve_count == num_cells);
}

void Table::Print(bool stdout)
{
    if(stdout)
    {
        for (size_t r = 0; r < num_rows; r++)
        {
            for (size_t c = 0; c < num_cols; c++)
            {
                std::cout << " " << cells[r][c].CellChar() ;
            }
            std::cout << endl;
        }
        return;
    }
    ofs << "Table: " << num_chars << " x " << num_chars << endl;
    PrintLine('=');
    for (size_t row = 0; row < num_rows; row++)
    {
        ofs.width(2);
        ofs.fill(' ');
        ofs << row + 1 << " | ";
        for (size_t col = 0; col < num_cols; col++)
        {
            streamsize wd = ofs.width();
            ofs.width(4);
            if (cells[row][col].IsSolved())
            {
                char solved = '*';
                if (cells[row][col].NewlySolved())
                {
                    solved = '+';
                    // SetChar(row, col, cells[row][col].CellChar());
                }
                ofs.fill(solved);
                ofs << cells[row][col].CellChar();
            }
            else
            {
                ofs.fill(' ');
                ofs << right << hex << cells[row][col].PossibleTable(false);
            }
            if (((col + 1) % block_size) == 0)
                ofs << "|";
            else
                ofs << " ";
            ofs.width(wd);
        }
        ofs << endl;
        if (((row + 1) % block_size) == 0)
            PrintLine('-');
    }
    PrintLine('=');
    size_t *colTotal = new size_t[num_rows];
    for (size_t row = 0; row < num_rows; row++)
    {
        colTotal[row] = 0;
    }
    for (size_t row = 0; row < num_rows; row++)
    {
        size_t rowTotal = 0;
        ofs.fill(' ');
        ofs.width(2);
        ofs << row + 1 << " | ";
        for (size_t col = 0; col < num_cols; col++)
        {
            ofs.fill(' ');
            ofs.width(3);
            ofs << cells[row][col].CellChar();
            if(cells[row][col].CellChar() != ' ')
            {
                rowTotal += (cells[row][col].CellChar() - '1' + 1);
                colTotal[col] += (cells[row][col].CellChar() - '1' + 1);
            }
            if ((col+1) % block_size)
                ofs << "  ";
            else
                ofs << " |";
        }
        ofs << ' ' << dec << rowTotal << endl;
        if (((row + 1) % block_size) == 0)
            PrintLine('-');
    }
    ofs << "   ";
    for (size_t row = 0; row < num_rows; row++)
    {
        ofs.width(5);
        ofs.fill(' ');
        ofs << colTotal[row]; // Incorrect value!!
    }

    ofs << endl;
}

void Table::PrintLine( char value)
{
    streamsize wd = ofs.width();
    ofs << "   ";
    ofs.width(5 * num_cols);
    ofs.fill(value);
    ofs << value;
    ofs << endl;
    ofs.width(wd);
}

void Table::PrintCharBitMap(size_t index)
{
    ofs << "Char Bitmap: " << num_chars << " x " << num_chars << " char " << chars[index] << endl;
    PrintLine('=');
    for (size_t row = 0; row < num_rows; row++)
    {
        ofs << row + 1 << " | ";
        for (size_t col = 0; col < num_cols; col++)
        {
            streamsize wd = ofs.width();
            ofs.width(4);
            ofs.fill(' ');
            char value = ((1 << index) & cells[row][col].PossibleTable(false))?chars[index]:'*';
            ofs << right << value;
            if (((col + 1) % block_size) == 0)
                ofs << "|";
            else
                ofs << " ";
            ofs.width(wd);
        }
        ofs << endl;
        if (((row + 1) % block_size) == 0)
            PrintLine('-');
    }
    PrintLine('=');
}

void Table::SetChar(size_t row, size_t col, char value, bool verbose) 
{
    if(verbose)
        ofs << __FUNCTION__ << " Row " << row + 1 << " Col " << col + 1 << " value " << value << endl;
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
