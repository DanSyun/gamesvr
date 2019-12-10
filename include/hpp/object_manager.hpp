#pragma once

#include "jemalloc.h"
#include "base.h"
#include "share_mem.hpp"
#include "serialize.hpp"


template<class DataType>
class Iterator
{
public:
    Iterator(DataType* pool, USED_LIST::iterator iter): _pool(pool), _iter(iter) {}

    bool operator==(const Iterator& rhs) { return _pool == rhs._pool && _iter == rhs._iter; }
    bool operator!=(const Iterator& rhs) { return !(_pool == rhs._pool && _iter == rhs._iter); }
    Iterator& operator++() { ++_iter; return *this; }
    DataType& operator*() { return _pool[*_iter]; }
    DataType* operator->() { return &_pool[*_iter]; }

    uint32 GetPos() { return *_iter; }
private:
    DataType* _pool;
    USED_LIST::iterator _iter;
};


template<class DataType>
class ObjectManager
{
public:
    typedef Iterator<DataType> iterator;
public:
    // init from shm if exist
    bool Init(uint32 size, uint32 shm_key)
    {
        if (size == 0) return false;
        if (size > _free_list.max_size()) return false;
        if (size > _used_list.max_size()) return false;

        _size = size;
        _shm_key = shm_key;
        _pool = new DataType[_size];
        for (uint32 pos = 0; pos < size; ++pos)
        {
            _free_list.push_back(pos);
        }

        if (_shm_key == 0) return true;

        // shm
        ShareMem shm(_shm_key);
        if (!shm.IsAttached()) return true;

        uint8* buff = (uint8*)shm.GetPtr();
        size_t byte_size = shm.GetByteSize();
        size_t offset = 0;

        uint32 obj_size = 0;
        if (!UnSerializeBaseType(buff, byte_size, offset, obj_size))
            return false;

        for (uint32 i = 0; i < _size && i < obj_size; ++i)
        {
            int pos = Malloc();
            DataType* pdata = Get(pos);
            if (!pdata->UnSerialize(buff, byte_size, offset))
                return false;
        }

        shm.Detach();
        ShareMem::Delete(_shm_key);
        return true;
    }

    // release to shm
    bool Release()
    {
        if (_shm_key == 0) return true;

        if (ShareMem::IsExist(_shm_key) && !ShareMem::Delete(_shm_key))
            return false;

        // calculate total size
        size_t byte_size = 4;
        for (auto iter = Begin(); iter != End(); ++iter)
        {
            byte_size += iter->GetByteSize();
        }

        // shm
        if (!ShareMem::Create(_shm_key, byte_size))
            return false;

        ShareMem shm(_shm_key);
        uint8* buff = (uint8*)shm.GetPtr();
        size_t offset = 0;

        // serialize
        if (!SerializeBaseType(buff, byte_size, offset, Size())) return false;
        for (auto iter = Begin(); iter != End(); ++iter)
        {
            if (!iter->Serialize(buff, byte_size, offset))
                return false;
        }

        shm.Detach();
        return true;
    }

    iterator Begin() { return iterator(_pool, _used_list.begin()); }
    iterator End() { return iterator(_pool, _used_list.end()); }
    uint32 Size() { return _used_list.size(); }
    uint32 MaxSize() { return _size; }
    bool IsFull() { return _free_list.empty(); }

protected:
    // get one free obj index and use it
    int32 Malloc()
    {
        if (_free_list.empty())
            return -1;

        uint32 pos = _free_list.front();
        _free_list.pop_front();
        _used_list.insert(pos);

        DataType& data = _pool[pos];
        new(&data) DataType();

        return pos;
    }
    // free one used obj
    bool Free(uint32 pos)
    {
        if (pos >= _size)
            return false;

        _free_list.push_back(pos);
        _used_list.erase(pos);

        DataType& data = _pool[pos];
        data.~DataType();

        return true;
    }
    // get the obj
    DataType* Get(uint32 pos)
    {
        if (pos >= _size)
            return NULL;

        return &_pool[pos];
    }
private:
    uint32 _size;
    uint32 _shm_key;
    DataType* _pool;
    USED_LIST _used_list;
    FREE_LIST _free_list;
};


