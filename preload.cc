#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#include <ostream>
#endif

#include <cstdint>
#include <iostream>
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
    uint64_t mallocs;
    uint64_t frees;
} _stats;

void __attribute__((destructor)) print_stats() {
    std::cout << "Mallocs: " << _stats.mallocs << std::endl
              << "Frees: " << _stats.frees << std::endl
    ;
}

static void* (*real_malloc)(size_t) = nullptr;
static void (*real_free)(void*) = nullptr;

void *malloc(size_t size)
{
    TRY_PRELOAD(malloc);

    _stats.mallocs++;
    return real_malloc(size);
}

void free(void* addr) {
    TRY_PRELOAD(free);

    _stats.frees++;
    return real_free(addr);
}
