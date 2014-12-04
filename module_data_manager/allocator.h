//
// allocator.h
//
// Created by qiong on 14-12-4
//
//

#ifndef __Allocator_H__
#define __Allocator_H__

template <typename T>
auto getAllocator() -> typename T::AllocatorType&
{
    static typename T::AllocatorType allocator;
    return allocator;
}

#endif