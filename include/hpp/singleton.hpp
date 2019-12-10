#pragma once

template <class T>
class Singleton
{
public:
    static T* Instance()
    {
        static T _t;
        return &_t;
    }
protected:
    Singleton() {}
    ~Singleton() {}
private:
    Singleton(const Singleton& rhs);
    Singleton& operator=(const Singleton& rhs);
};

