
MAKE_DIR =	core/proto			\
			core/src			\
			proto				\
			common				\
			svr/_1gm			\
			svr/proxysvr		\
			svr/eventsvr		\
			svr/connectsvr		\
			svr/connectwssvr	\
			svr/dbproxysvr		\
			svr/websvr			\
			svr/machinesvr		\
			svr/lipsticksvr		\
			svr/promotersvr

default: debug

debug:
	for dir in $(MAKE_DIR);								\
	do													\
		CFLAGS="-O0 -D_DEBUG" CXXFLAGS="-O0 -D_DEBUG"	\
		make -j2 -C $$dir;								\
	done

release:
	for dir in $(MAKE_DIR);			\
	do								\
		CFLAGS="-O3" CXXFLAGS="-O3"	\
		make -j2 -C $$dir;			\
	done

clean:
	for dir in $(MAKE_DIR);		\
	do							\
		make clean -C $$dir;	\
	done

proto:  
	make -j1 -C proto

core:
	make -j1 -C core/proto
	make -j1 -C core/src

cs:
	cp	proto/pb_cs_msg.proto	\
		proto/pb_common.proto	\
		proto_cs/
	svn ci	proto_cs/* -m ""
	
web:
	cp	proto/pb_ss_msg.proto	\
		proto/pb_common.proto	\
		proto/pb_db_data.proto	\
		proto_web/
	svn ci	proto_web/* -m ""

.PHONY: debug release clean proto core cs web

