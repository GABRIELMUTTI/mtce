CXX  		:= gcc
CXX_FLAGS 	:= -Wall -Wextra -fdiagnostics-color -g

BIN		:= bin
MAIN		:= test_hash_set.c
SRC		:= ../../code/collection/*.c
INCLUDE		:= -I../../code

EXECUTABLE	:= main

all: $(BIN)/$(EXECUTABLE)

reb: clean all

run: clean all
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC) $(MAIN)
	$(CXX) $(CXX_FLAGS) $(INCLUDE) $^ -o $@
clean:
	-rm $(BIN)/*
