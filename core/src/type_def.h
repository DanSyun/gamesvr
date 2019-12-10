#pragma once

#include <list>
#include <forward_list>
#include <unordered_set>
#include <unordered_map>

typedef unsigned char       uint8;
typedef unsigned short      uint16;
typedef unsigned int        uint32;
typedef unsigned long long  uint64;
typedef char                int8;
typedef short               int16;
typedef int                 int32;
typedef long long           int64;

typedef std::unordered_set<uint32>  USED_LIST;
typedef std::list<uint32>           FREE_LIST;
typedef std::forward_list<uint32>   FREE_LIST2;

