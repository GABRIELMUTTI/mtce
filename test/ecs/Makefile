CXX  		:= gcc
CXX_FLAGS 	:= -Wall -Wextra -fdiagnostics-color -g

BIN		:= bin
MAIN		:= main.c
SRC		:= ../../code/collection/*.c ../../code/ecs/*.c
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
