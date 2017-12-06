all: progMPI

progMPI: prog.c funcs.c funcs.h
	mpicc prog.c funcs.c -lm -lnetpbm -o progMPI

clean:
	rm -f progMPI
