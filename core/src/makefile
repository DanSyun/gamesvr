PRO_DIR=../..
-include $(PRO_DIR)/makefile.dep

OBJ_DIR=./obj_dep
LIB_DIR=$(PRO_DIR)/lib
$(shell mkdir -p $(OBJ_DIR))
$(shell mkdir -p $(LIB_DIR))

BIN=$(LIB_DIR)/libcore.a
OBJ=	$(patsubst %.proto, $(OBJ_DIR)/%.pb.o, $(wildcard *.proto))	\
		$(patsubst %.cpp, $(OBJ_DIR)/%.o, $(wildcard *.cpp))

$(BIN) : $(OBJ)
	ar rcs $@ $^

clean:
	rm -rf $(BIN) $(OBJ_DIR)

-include $(PRO_DIR)/makefile.rule

