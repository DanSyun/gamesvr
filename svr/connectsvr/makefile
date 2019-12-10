PRO_DIR=../..
-include $(PRO_DIR)/makefile.dep

OBJ_DIR=./obj_dep
BIN_DIR=./bin
$(shell mkdir -p $(OBJ_DIR))
$(shell mkdir -p $(BIN_DIR))

BIN=$(BIN_DIR)/connectsvr
OBJ=$(patsubst %.cpp, $(OBJ_DIR)/%.o, $(wildcard *.cpp))

$(BIN) : $(OBJ) $(LIB_DIR)/libcore.a
	g++ $(CXXFLAGS) $(INC) -o $@ $^ $(LIB)

clean:
	rm -rf $(BIN) $(OBJ_DIR)

-include $(PRO_DIR)/makefile.rule

