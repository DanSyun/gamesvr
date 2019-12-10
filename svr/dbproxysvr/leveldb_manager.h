#pragma once

#include "singleton.hpp"
#include "google/leveldb/db.h"
#include "base.h"
#include "proto_base.h"

struct stVersionInfo
{
    uint32 version;
    time_t update_time;
};

class LeveldbManager: public Singleton<LeveldbManager>
{
public:
    bool Init(uint32 server_id);
    bool Query(uint64 uid, uint32 key, PBKVDBData& data);
    bool Save(uint64 uid, uint32 key, const PBKVDBData& data);
    bool GetVersion(uint64 uid, uint32 key, uint32& version);
    void VersionExpire();

private:
    string GenerateDBKey(uint64 uid, uint32 key);

private:
    leveldb::DB* _db;
    uint32 _version_update_time;
    unordered_map<string, stVersionInfo> _version_map;
};

