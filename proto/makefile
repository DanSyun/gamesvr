PRO_DIR=..
-include $(PRO_DIR)/makefile.dep

OBJ_DIR=./obj_dep
LIB_DIR=$(PRO_DIR)/lib
$(shell mkdir -p $(OBJ_DIR))
$(shell mkdir -p $(LIB_DIR))

BIN=$(LIB_DIR)/libproto.a
#按顺序编译
OBJ=	$(OBJ_DIR)/pb_event.pb.o			\
		$(OBJ_DIR)/pb_common.pb.o			\
		$(OBJ_DIR)/pb_db_data.pb.o			\
		$(OBJ_DIR)/pb_cs_msg.pb.o			\
		$(OBJ_DIR)/pb_ss_msg.pb.o			\
		$(OBJ_DIR)/pb_internal_msg.pb.o		\
		$(OBJ_DIR)/pb_mem_data.pb.o			\
		$(OBJ_DIR)/pb_config.pb.o

$(BIN) : $(OBJ)
	ar rcs $@ $^

$(OBJ_DIR)/%.pb.o : %.proto
	$(PROTOC) --cpp_out=./ $<
	g++ -c $(CXXFLAGS) $(INC) $(patsubst %.proto, %.pb.cc, $<) -o $@

clean:
	rm -rf $(BIN) $(OBJ_DIR) *.pb.cc *.pb.h

