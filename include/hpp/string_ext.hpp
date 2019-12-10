#pragma once

#include <string>

static inline std::string SpiltString(const std::string& input, const std::string& delimit, uint offset)
{
    size_t off_mark = 0;
    size_t begin = 0;
    size_t cur = input.find_first_of(delimit);
    while (begin != std::string::npos)
    {
        if (off_mark++ == offset)
            return input.substr(begin, cur - begin);
        begin = input.find_first_not_of(delimit, cur);
        cur  = input.find_first_of(delimit, begin);
    }
    return "";
}

