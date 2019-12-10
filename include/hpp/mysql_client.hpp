#pragma once

#include "singleton.hpp"
#include "mysql++.h"
#include "type_def.h"

using namespace mysqlpp;

class MysqlClient
{
public:
    MysqlClient(const char* host, uint32 port, const char* db, const char* user, const char* pwd):
        _connection(false)
    {
        _connection.connect(db, host, user, pwd, port);
    }
    ~MysqlClient()
    {
        _connection.disconnect();
    }
    const Query Commit(string sql)
    {
        return _connection.query(sql);
    }
private:
    Connection _connection;
};

class MysqlClientS: public Singleton<MysqlClient>
{
public:
    bool Connect(const char* host, uint32 port, const char* db, const char* user, const char* pwd)
    {
        _connection.connect(db, host, user, pwd, port);
        return _connection.connected();
    }
    void DisConnect()
    {
        if (_connection.connected())
            _connection.disconnect();
    }
    const Query Commit(string sql)
    {
        return _connection.query(sql);
    }
private:
    Connection _connection;
};

