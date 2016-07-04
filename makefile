CC := g++-5

ARCH := core2 

CFLAGS := -march=$(ARCH) -O3 -fopenmp -m64 -std=c++11

COMPILE_COMMAND := $(CC) $(CFLAGS)

OUTPUT := app.out

all: 
	$(COMPILE_COMMAND) -o $(OUTPUT) *.cpp

clean:
	rm -f *.o $(OUTPUT).*