# My third makefile

# Name of the project
PROJ_NAME=robomus

# .c files
C_SOURCE=$(wildcard ./src/*.cpp)

# .h files
H_SOURCE=$(wildcard ./src/*.h)

# Object files
OBJ=$(subst .cpp,.o,$(subst src,objects,$(C_SOURCE)))

# Compiler and linker
CC=g++

# Flags for compiler
CC_FLAGS=-c         \
         -W         \
         -Wall      \
         -ansi      \
         -pedantic

# Command used at clean target
RM = rm -rf

#
# Compilation and linking
#
all: objFolder $(PROJ_NAME)

$(PROJ_NAME): $(OBJ)
	@ echo 'Building binary using GCC linker: $@'
	$(CC) $^ -o $@  -L/usr/local/lib -l:liboscpack.so -std=c++11 -pthread -lwiringPi
	@ echo 'Finished building binary: $@'
	@ echo ' '
#-L/usr/local/lib -lliboscpack.so.1.1.0
./objects/%.o: ./src/%.cpp ./src/%.h
	@ echo 'Building target using GCC compiler: $<'
	$(CC) $< $(CC_FLAGS) -o $@ -I /usr/local/include/oscpack/ -L/usr/local/lib -l:liboscpack -std=c++11 -lwiringPi -pthread
	@ echo ' '

./objects/main.o: ./src/main.cpp $(H_SOURCE)
	@ echo 'Building target using G++ compiler: $<'
	$(CC) $< $(CC_FLAGS) -o $@ -I /usr/local/include/oscpack/ -L/usr/local/lib -l:liboscpack -std=c++11 -lwiringPi -pthread
	@ echo ' '

objFolder:
	@ mkdir -p objects

clean:
	@ $(RM) ./objects/*.o $(PROJ_NAME) *~
	@ rmdir objects

.PHONY: all clean