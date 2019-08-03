CXX  		:= gcc
CXX_FLAGS 	:= -Wall -Wextra -fdiagnostics-color -g

BIN			:= bin
SRC			:= ../../code/collection/*.c
INCLUDE			:= -I../../code -I../


run: linked_list_run hash_set_run

# linked_list
linked_list: $(BIN)/test_linked_list

$(BIN)/test_linked_list: $(SRC) test_linked_list.c
	$(CXX) $(CXX_FLAGS) $(INCLUDE) $^ -o $@

linked_list_run: clean linked_list
	./$(BIN)/test_linked_list

# hash_set
hash_set: $(BIN)/test_hash_set

$(BIN)/test_hash_set: $(SRC) test_hash_set.c
	$(CXX) $(CXX_FLAGS) $(INCLUDE) $^ -o $@

hash_set_run: clean hash_set
	./$(BIN)/test_hash_set

clean:
	-rm $(BIN)/*

