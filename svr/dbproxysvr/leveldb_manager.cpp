#include "leveldb_manager.h"

bool LeveldbManager::Init(uint32 server_id)
{
    _version_update_time = GetTime();

    leveldb::Options options;
    options.create_if_missing = true;
    char name[64] = {0};
    snprintf(name, sizeof(name), "/ServerData/db_data_%03u", server_id);
    leveldb::Status status = leveldb::DB::Open(options, name, &_db);
    if (status.ok())
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool LeveldbManager::Query(uint64 uid, uint32 key, PBKVDBData& data)
{
    string db_key = GenerateDBKey(uid, key);
    string db_value;
    leveldb::ReadOptions options;
    leveldb::Status status = _db->Get(options, db_key, &db_value);
    if (status.ok())
    {
        if (data.ParseFromArray(db_value.c_str(), db_value.size()))
        {
            stVersionInfo info;
            info.version = data.version();
            info.update_time = GetTime();
            _version_map[db_key] = info;
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        if (status.IsNotFound())
        {
            data.set_key(key);

            stVersionInfo info;
            info.version = 0;
            info.update_time = GetTime();
            _version_map[db_key] = info;
            return true;
        }
        else
        {
            return false;
        }
    }
}

bool LeveldbManager::Save(uint64 uid, uint32 key, const PBKVDBData & data)
{
    uint32 buf_len = data.ByteSize();
    if (buf_len > MAX_KVDB_BUF_LEN)
        return false;

    static char buf[MAX_KVDB_BUF_LEN];
    data.SerializeWithCachedSizesToArray((google::protobuf::uint8*)buf);

    string db_key = GenerateDBKey(uid, key);
    leveldb::WriteOptions options;
    leveldb::Status status = _db->Put(options, db_key, leveldb::Slice(buf, buf_len));

    if (status.ok())
    {
        _version_map[db_key].version += 1;
        return true;
    }
    else
    {
        return false;
    }
}

bool LeveldbManager::GetVersion(uint64 uid, uint32 key, uint32 & version)
{
    string db_key = GenerateDBKey(uid, key);
    if (_version_map.find(db_key) != _version_map.end())
    {
        version = _version_map[db_key].version;
        return true;
    }
    else
    {
        return false;
    }
}

void LeveldbManager::VersionExpire()
{
    // 每日凌晨三点 (10800)清除超过一天的 (86400)

    auto now = GetTime();

    if (GetDiffDays(_version_update_time - 10800, now - 10800) == 0)
        return;

    _version_update_time = now;
    unordered_set<string> expires;
    for (auto iter = _version_map.begin(); iter != _version_map.end(); ++iter)
    {
        const stVersionInfo& info = iter->second;
        if (info.update_time + 86400 <= now)
        {
            expires.insert(iter->first);
        }
    }
    for (auto iter = expires.begin(); iter != expires.end(); ++iter)
    {
        _version_map.erase(*iter);
    }
}

string LeveldbManager::GenerateDBKey(uint64 uid, uint32 key)
{
    char key_buf[64];
    snprintf(key_buf, sizeof(key_buf), "%llu|%u", uid, key);

    return string(key_buf);
}

