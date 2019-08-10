all:
	g++ -I ./src ./src/*.cpp -o sudoku_solver
	@echo Completed $@

clean:
	rm -rf sudoku_solver *.txt
