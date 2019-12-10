#pragma once

#include <fcntl.h>
#include <unordered_map>

#include "base.h"
#include "svr_config.pb.h"
#include "protobuf/io/zero_copy_stream_impl.h"
#include "protobuf/text_format.h"


class LogicSvrConf: public LogicSvrConfig
{
typedef LogicSvrConfig  base;
public:
    bool Init(const char* path)
    {
        PBSvrConfig config;
        int fd = open(path, O_RDONLY);
        if (fd < 0) return false;

        google::protobuf::io::FileInputStream input(fd);
        input.SetCloseOnDelete(true);
        if (!google::protobuf::TextFormat::Parse(&input, &config))
            return false;

        base::CopyFrom(config.logic());
        return true;
    }
};

class ConnectSvrConf: public ConnectSvrConfig
{
typedef ConnectSvrConfig  base;
public:
    bool Init(const char* path)
    {
        PBSvrConfig config;
        int fd = open(path, O_RDONLY);
        if (fd < 0) return false;

        google::protobuf::io::FileInputStream input(fd);
        input.SetCloseOnDelete(true);
        if (!google::protobuf::TextFormat::Parse(&input, &config))
            return false;

        base::CopyFrom(config.connect());
        return true;
    }
};

class ProxySvrConf: public ProxySvrConfig
{
typedef ProxySvrConfig  base;
public:
    bool Init(const char* path)
    {
        PBSvrConfig config;
        int fd = open(path, O_RDONLY);
        if (fd < 0) return false;

        google::protobuf::io::FileInputStream input(fd);
        input.SetCloseOnDelete(true);
        if (!google::protobuf::TextFormat::Parse(&input, &config))
            return false;

        base::CopyFrom(config.proxy());

        _index.clear();
        _logic_svr_num = 0;
        for (int32 i = 0; i < base::svrs_size(); ++i)
        {
            _index[base::svrs(i).svr_type()] = i;
            _logic_svr_num += base::svrs(i).svr_num();
        }
        return true;
    }
    const LogicSvr* GetLogicSvr(uint32 svr_type) const
    {
        if (_index.find(svr_type) == _index.end())
            return NULL;
        else
            return &base::svrs(_index.at(svr_type));
    }
    uint32 GetLogicSvrNum() { return _logic_svr_num; }
private:
    std::unordered_map<uint32, uint32> _index;
    uint32 _logic_svr_num;
};

class EventSvrConf: public EventSvrConfig
{
typedef EventSvrConfig base;
public:
    bool Init(const char* path)
    {
        PBSvrConfig config;
        int fd = open(path, O_RDONLY);
        if (fd < 0) return false;

        google::protobuf::io::FileInputStream input(fd);
        input.SetCloseOnDelete(true);
        if (!google::protobuf::TextFormat::Parse(&input, &config))
            return false;

        base::CopyFrom(config.event());
        return true;
    }
};


