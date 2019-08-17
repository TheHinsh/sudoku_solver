all:
	g++ -I ./src ./src/*.cpp -o sudoku_solver
	@echo Completed $@
	./sudoku_solver

easy:
	g++ -DEASY -I ./src ./src/*.cpp -o sudoku_solver
	@echo Completed $@
	./sudoku_solver

clean:
	rm -rf sudoku_solver *.txt
