PRO_DIR=../..
-include $(PRO_DIR)/makefile.dep

OBJ_DIR=./obj_dep
LIB_DIR=$(PRO_DIR)/lib
$(shell mkdir -p $(OBJ_DIR))
$(shell mkdir -p $(LIB_DIR))

BIN=$(LIB_DIR)/libcoreproto.a
OBJ=$(patsubst %.proto, $(OBJ_DIR)/%.pb.o, $(wildcard *.proto))

$(BIN) : $(OBJ)
	ar rcs $@ $^

$(OBJ_DIR)/%.pb.o : %.proto
	$(PROTOC) --cpp_out=./ $<
	g++ -c $(CXXFLAGS) $(INC) $(patsubst %.proto, %.pb.cc, $<) -o $@

clean:
	rm -rf $(BIN) $(OBJ_DIR) *.pb.cc *.pb.h

