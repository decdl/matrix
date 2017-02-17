
CXX = g++
CXX_FLAGS = -c -std=c++17 $F
LD_FLAGS = $F

all: matrix

matrix: matrix.o
	$(CXX) $(LD_FLAGS) matrix.o -o matrix

matrix.o: matrix.cpp prec.h.gch
	$(CXX) $(CXX_FLAGS) matrix.cpp -o matrix.o

prec.h.gch: prec.h Vector.h Matrix.h Frac.h
	$(CXX) $(CXX_FLAGS) prec.h -o prec.h.gch

clean:
	rm -f *.o *.h.gch matrix
