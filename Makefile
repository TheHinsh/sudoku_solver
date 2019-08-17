all: sudoku_solver
	./sudoku_solver ./table1.txt '*' 123456789 3 expert.tbl

easy: sudoku_solver
	./sudoku_solver ./table1.txt '*' 123456789 3 easy.tbl

hex1: sudoku_solver
	./sudoku_solver ./table1.txt '*' 0123456789ABCDEF 4 hexadoku.tbl

sudoku_solver: ./src/*.cpp ./src/*.h
	g++ -I ./src ./src/*.cpp -o sudoku_solver
	@echo Completed $@

clean:
	rm -rf sudoku_solver *.txt
