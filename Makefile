
CXX = g++
CXX_FLAGS = -c -std=c++17 $F
LD_FLAGS = $F

all: matrix

matrix: matrix.o
	$(CXX) $(LD_FLAGS) matrix.o -o matrix

matrix.o: matrix.cpp precompile.h.gch
	$(CXX) $(CXX_FLAGS) matrix.cpp -o matrix.o

precompile.h.gch: precompile.h vector.h matrix.h frac.h
	$(CXX) $(CXX_FLAGS) precompile.h -o precompile.h.gch

clean:
	rm -f *.o *.h.gch matrix
