#pragma once

#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "base.h"
#include "mongoc.h"	
#include "bson.h"

class EventServer
{
public:
    int Initialize(int port, char* argv []);
    void Run();
	
private:
    void ParseArgs(int argc, char* argv[], char* conf, bool& daemon);
    bool LoadConfig(const char* conf);
    bool InitNetWork();
    bool Connect();
    void DisConnect();

    bool CheckRunFlag();
    void CheckChangeDay();
    void DealNetWork();
    bool WriteMongoDB(const char* buf, uint32 buf_len);
    mongoc_collection_t* GetMongoCollection(const string& event_name);

    EventSvrConf _conf;
    int _lsfd;
    char _buf[MAX_EVENT_BUF_LEN];
    struct sockaddr_in _addr;
    socklen_t _addr_len;

    time_t _connect_time;
    mongoc_client_t* _mongo_client;
    mongoc_database_t* _mongo_db;
    unordered_map<string, mongoc_collection_t*> _coll_map;
};

