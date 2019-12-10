PRO_DIR=../..
-include $(PRO_DIR)/makefile.dep

OBJ_DIR=./obj_dep
BIN_DIR=./bin
$(shell mkdir -p $(OBJ_DIR))
$(shell mkdir -p $(BIN_DIR))

BIN=$(BIN_DIR)/eventsvr
OBJ=$(patsubst %.cpp, $(OBJ_DIR)/%.o, $(wildcard *.cpp))

$(BIN) : $(OBJ) $(LIB_DIR)/libcore.a
	g++ $(CXXFLAGS) $(INC) -o $@ $^ -L$(PRO_DIR)/lib -ljemalloc -lm -lpthread -ldl -lcore -lcoreproto -lproto -lprotobuf -lrt -lresolv -lmongoc -lbson

clean:
	rm -rf $(BIN) $(OBJ_DIR)

-include $(PRO_DIR)/makefile.rule
