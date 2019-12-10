#pragma once

#include "type_def.h"

template <typename ElemType>
static inline bool SerializeBaseType(uint8* buf, size_t buf_size, size_t& buf_offset, const ElemType& elem)
{
    size_t elem_size = sizeof(elem);
    if (elem_size + buf_offset > buf_size)
        return false;

    ElemType* pelem = (ElemType*)(buf + buf_offset);
    *pelem = elem;
    buf_offset += elem_size;

    return true;
}

template <typename ElemType>
static inline bool UnSerializeBaseType(const uint8* buf, size_t buf_size, size_t& buf_offset, ElemType& elem)
{
    size_t elem_size = sizeof(elem);
    if (elem_size + buf_offset > buf_size)
        return false;

    ElemType* pelem = (ElemType*)(buf + buf_offset);
    elem = *pelem;
    buf_offset += elem_size;

    return true;
}

template <class ProtoType>
static inline bool SerializeProtoBuf(uint8* buf, size_t buf_size, size_t& buf_offset, const ProtoType& proto)
{
    size_t elem_size = proto.ByteSize();
    if (elem_size + sizeof(elem_size) + buf_offset > buf_size)
        return false;

    if (!SerializeBaseType(buf, buf_size, buf_offset, elem_size))
        return false;

    if (!proto.SerializeWithCachedSizesToArray(buf + buf_offset))
    {
        buf_offset -= sizeof(elem_size);
        return false;
    }

    buf_offset += elem_size;

    return true;
}

template <class ProtoType>
static inline bool UnSerializeProtoBuf(const uint8* buf, size_t buf_size, size_t& buf_offset, ProtoType& proto)
{
    size_t elem_size = 0;
    if (!UnSerializeBaseType(buf, buf_size, buf_offset, elem_size))
        return false;

    if (elem_size + buf_offset > buf_size)
    {
        buf_offset -= sizeof(elem_size);
        return false;
    }

    if (!proto.ParseFromArray(buf + buf_offset, elem_size))
    {
        buf_offset -= sizeof(elem_size);
        return false;
    }

    buf_offset += elem_size;
    return true;
}

