#pragma once
#include <execinfo.h>
#include <cstdio>
#include <cstdlib>
#define DUMP_STACK_DEPTH_MAX 16
void dump_trace();
#ifndef NEDBUG
#define assert_msg(condition, message) \
    do { \
        if (! (condition)) { \
            std::cerr << "Assertion `" #condition "` failed in " << __FILE__ \
                      << " line " << __LINE__ << ": " << message << std::endl; \
            dump_trace();\
            std::terminate(); \
        } \
    } while (false)
#else
#   define ASSERT(condition, message) do { } while (false)
#endif
