#pragma once

#include<stdio.h>
#include<sys/ipc.h>
#include<sys/shm.h>


class ShareMem
{
public:
    static bool Create(key_t key, size_t size, int shmflg = 0666)
    {
        return (shmget(key, size, shmflg |IPC_CREAT) != -1)? true: false;
    }
    static bool IsExist(key_t key)
    {
        int shmid;
        return ((shmid = shmget(key, 0, 0)) != -1)? true: false;
    }
    static bool Delete(key_t key)
    {
        int shmid;
        if ((shmid = shmget(key, 0, 0)) == -1) return false;
        if (shmctl(shmid, IPC_RMID, NULL) == 0) return false;
        return true;
    }

public:
    explicit ShareMem(key_t key): _key(0), _mem(NULL) { Attach(key); }
    explicit ShareMem(const ShareMem& rhs): _key(0), _mem(NULL) { if (rhs.IsAttached()) Attach(rhs._key); }
    ~ShareMem() {}//Detach(); }

    void* GetPtr()
    { 
        return _mem;
    }
    size_t GetByteSize()
    {
        int shmid;
        shmid_ds shmds;
        if ((shmid = shmget(_key, 0, 0)) == -1) return 0;
        if (shmctl(shmid, IPC_STAT, &shmds) == -1) return 0;

        return shmds.shm_segsz;
    }
    bool IsAttached() const
    {
        return _key != 0 && _mem != NULL;
    }
    void Attach(key_t key)
    {
        int shmid;
        void* mem;
        if (IsAttached()) Detach();
        if ((shmid = shmget(key, 0, 0)) == -1) return;
        if ((mem = shmat(shmid, NULL, 0)) == (void*)-1) return;

        _key = key;
        _mem = mem;
    }
    void Detach()
    {
        if (!IsAttached()) return;
        shmdt(_mem);
        _key = 0;
        _mem = NULL;
    }

private:
    key_t _key;
    void* _mem;
};


