
SRC = main.cpp Naive/Naive.cpp MoinsNaive/MoinsNaive.cpp

OBJ = main.o Naive/Naive.o MoinsNaive/MoinsNaive.o

CFLAG = -O3 -std=c++14

LFLAG = -pthread

all: $(OBJ)
	mkdir -p Resultats
	g++ $(OBJ) -o a.out $(LFLAG)


%.o: %.cpp %.hpp
	g++ -o $@ -c $< $(CFLAG)

%.o: %.cpp
	g++ -o $@ -c $< $(CFLAG)

clean:
	rm $(OBJ)

cleanall:
	rm $(OBJ) a.out