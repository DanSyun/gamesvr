PRO_DIR=..
-include $(PRO_DIR)/makefile.dep

OBJ_DIR=./obj_dep
LIB_DIR=$(PRO_DIR)/lib
$(shell mkdir -p $(OBJ_DIR))
$(shell mkdir -p $(LIB_DIR))

BIN=$(LIB_DIR)/libcommon.a
OBJ=$(patsubst %.cpp, $(OBJ_DIR)/%.o, $(wildcard *.cpp))

$(BIN) : $(OBJ)
	ar rcs $@ $^

clean:
	rm -rf $(BIN) $(OBJ_DIR)

-include $(PRO_DIR)/makefile.rule

