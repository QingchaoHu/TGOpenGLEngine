build : main.out

start : build
	./main.out

clean :
	rm *.o main.out

HEADERS := $(wildcard *.h)
CPPS := $(wildcard *.cpp)
OBJS := $(CPPS:.cpp=.o)

main.out : $(OBJS) TGProgram.o
	@# g++ -std=c++17 -static -lglfw -lGLEW -lglm -lassimp -framework OpenGL -o main.out $(OBJS)
	g++ -lglfw -lGLEW -lglm -lassimp -framework OpenGL -o main.out $(OBJS)

$(OBJS) : %.o : %.cpp
	g++ -c -g -std=c++17 -o $@ $^
TGProgram.o : TGProgram.h
	g++ -c -g -std=c++17 -o $@ $^

.PHONY : build start clean