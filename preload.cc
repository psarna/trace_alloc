#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <cstdint>
#include <iostream>
#include <iomanip>
#include <dlfcn.h>

#define TRY_PRELOAD(func) \
    do { \
        if (!real_##func) { \
            real_##func = (decltype(real_##func))dlsym(RTLD_NEXT, #func); \
            if (!real_##func) { \
                fprintf(stderr, "Failed to override %s: %s\n", #func, dlerror()); \
            } \
        } \
    } while (0)

static struct stats {
    uint64_t mallocs = 0;
    uint64_t reallocs = 0;
    uint64_t callocs = 0;
    uint64_t frees = 0;
} _stats;

void __attribute__((destructor)) print_stats() {
    std::cout
        << "----------" << std::endl
        << "Alloc stats" << std::endl
        << "----------" << std::endl
        << "Mallocs:  " << std::setw(12) << _stats.mallocs << std::endl
        << "Reallocs: " << std::setw(12) << _stats.reallocs << std::endl
        << "Callocs:  " << std::setw(12) << _stats.callocs << std::endl
        << "----------" << std::endl
        << "sum =     " << std::setw(12) << (_stats.mallocs + _stats.reallocs + _stats.callocs) << std::endl
        << "----------" << std::endl
        << "Frees:    " << std::setw(12) << _stats.frees << std::endl
    ;
}

static void* (*real_malloc)(size_t) = nullptr;
static void* (*real_realloc)(void*, size_t) = nullptr;
static void* (*real_calloc)(size_t, size_t) = nullptr;
static void (*real_free)(void*) = nullptr;

void *malloc(size_t size)
{
    TRY_PRELOAD(malloc);

    _stats.mallocs++;
    return real_malloc(size);
}

void *realloc(void* addr, size_t size) {
    TRY_PRELOAD(realloc);

    _stats.reallocs++;
    return real_realloc(addr, size);
}

void *calloc(size_t nmemb, size_t size) {
    TRY_PRELOAD(calloc);

    _stats.callocs++;
    return real_calloc(nmemb, size);
}

void free(void* addr) {
    TRY_PRELOAD(free);

    _stats.frees++;
    return real_free(addr);
}
