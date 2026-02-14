#pragma once

#if HOTRELOAD && !defined(PLATFORM_wasm)
#include "generated/demo_lib_export.hpp"
#define DEMO_API DEMO_LIB_EXPORT
#else
#define DEMO_API
#endif

extern "C" {
DEMO_API void demo_init();

DEMO_API void demo_update(float time);

DEMO_API unsigned int* demo_get_buffer();
};
