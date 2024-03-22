build : main.out

start : build
	./main.out

clean :
	rm *.o main.out

HEADERS := $(wildcard *.h)
CPPS := $(wildcard *.cpp)
OBJS := $(CPPS:.cpp=.o)

main.out : $(OBJS) TGProgram.o
	g++ -std=c++17 -L/opt/homebrew/lib -I/opt/homebrew/include -lglfw -lglew -lglm -lassimp -framework OpenGL -o main.out $(OBJS)

$(OBJS) : %.o : %.cpp
	g++ -c -g -std=c++17 -I/opt/homebrew/include  -o $@ $^
TGProgram.o : TGProgram.h
	g++ -c -g -std=c++17 -I/opt/homebrew/include  -o $@ $^

.PHONY : build start clean