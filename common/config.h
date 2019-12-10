#pragma once

#include <fcntl.h>
#include <unordered_map>

#include "base.h"
#include "pb_config.pb.h"
#include "protobuf/io/zero_copy_stream_impl.h"
#include "protobuf/text_format.h"

#include "singleton.hpp"
#include "mysql_client.hpp"

class ConfigGameAddr: public PBConfigGameAddr, public Singleton<ConfigGameAddr>
{
typedef PBConfigGameAddr  base;
public:
    bool Init(const char* path)
    {
        PBConfig config;
        int fd = open(path, O_RDONLY);
        if (fd < 0) return false;
        google::protobuf::io::FileInputStream input(fd);
        input.SetCloseOnDelete(true);
        if (!google::protobuf::TextFormat::Parse(&input, &config)) return false;

        base::Clear();
        base::CopyFrom(config.game_addr());
        _index.clear();
        for (auto i = 0; i < base::addrs_size(); ++i)
        {
            if (base::addrs(i).hosts_size() == base::addrs(i).ports_size() &&
                base::addrs(i).hosts_size() != 0)
                _index[base::addrs(i).game_type()] = i;
        }

        return true;
    }
    const PBGameAddr* GetAddrByType(ENGameType type)
    {
        if (_index.find(type) != _index.end())
            return &base::addrs(_index[type]);
        else
            return NULL;
    }
private:
    unordered_map<uint64, uint64> _index;
};

class ConfigPrize: public Singleton<ConfigPrize>
{
public:
    struct stPrize
    {
        uint64 value;
        string name;
    };
public:
    bool Init(const PBMysqlAddr* paddr)
    {
        if (!paddr)
        {
            LOG_DEBUG("ConfigPrize init failed");
            return false;
        }

        MysqlClient client(paddr->host().c_str(), paddr->port(), paddr->db().c_str(),
            paddr->user().c_str(), paddr->password().c_str());

        string sql("select id, value, prizeName from prize;");
        mysqlpp::Query query = client.Commit(sql);

        if (query.errnum() != 0)
        {
            LOG_DEBUG("ConfigPrize init failed");
            return false;
        }

        const mysqlpp::StoreQueryResult& result = query.store();
        for (uint32 i = 0; i < result.num_rows(); ++i)
        {
            stPrize prize;
            prize.value = result[i][1];
            prize.name = result[i][2].data();

            LOG_DEBUG("prize: %llu, %s", prize.value, prize.name.c_str());
            if (prize.value == 0) continue;
            _index[result[i][0]] = prize;
        }

        return true;
    }
    bool IsExist(uint64 prize_id)
    {
        return _index.find(prize_id) != _index.end();
    }
    const stPrize* GetPrize(uint64 prize_id)
    {
        if (!IsExist(prize_id)) return NULL;
        return &_index[prize_id];
    }
private:
    unordered_map<uint64, stPrize> _index;
};

class ConfigMysql: public PBConfigMysqlAddr, public Singleton<ConfigMysql>
{
typedef PBConfigMysqlAddr  base;
public:
    bool Init(const char* path)
    {
        PBConfig config;
        int fd = open(path, O_RDONLY);
        if (fd < 0) return false;
        google::protobuf::io::FileInputStream input(fd);
        input.SetCloseOnDelete(true);
        if (!google::protobuf::TextFormat::Parse(&input, &config)) return false;

        base::Clear();
        base::CopyFrom(config.mysql_addr());
        _index.clear();
        for (auto i = 0; i < base::addrs_size(); ++i)
        {
            _index[base::addrs(i).table()] = i;
        }

        bool ret = true;
        ret &= ConfigPrize::Instance()->Init(GetAddrByTableName("prize"));

        return ret;
    }
    const PBMysqlAddr* GetAddrByTableName(string name)
    {
        if (_index.find(name) != _index.end())
            return &base::addrs(_index[name]);
        else
            return NULL;
    }
    void Update(string name)
    {
        if (name == "prize")
        {
            ConfigPrize::Instance()->Init(GetAddrByTableName(name));
        }
    }
private:
    unordered_map<string, uint64> _index;
};

