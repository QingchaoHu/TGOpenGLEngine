build : main.out

start : build
	./main.out

clean :
	rm *.o main.out

CPPS := $(wildcard *TG*.cpp)
OBJS := $(CPPS:.cpp=.o)

BREW := $(shell brew --prefix)
INC := -I$(BREW)/include
LIB := -L$(BREW)/lib

main.out : $(OBJS) main.o
	@# g++ $(LIB) -std=c++17 -static -lglfw -lGLEW -lglm -lassimp -framework OpenGL -o main.out $(OBJS) main.o
	g++ $(LIB) -std=c++17 -lglfw -lGLEW -lglm -lassimp -framework OpenGL -o main.out $(OBJS) main.o

$(OBJS) : %.o : %.cpp
	g++ -c -g -std=c++17 $(INC) -o $@ $^
main.o : TGProgram.h main.cpp
	g++ -c -g -std=c++17 $(INC) -o $@ main.cpp

.PHONY : build start clean