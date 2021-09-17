
#include "GraphsEport.hpp"

#ifdef __EMSCRIPTEN__
#define PREFIX(type) EMSCRIPTEN_KEEPALIVE type
#include <emscripten.h>
#else
#define PREFIX(type) type
#endif

#ifdef __cplusplus
extern "C" {
#endif

PREFIX(char*) EXPORT_getJson() { return getJson(); }
PREFIX(void) EXPORT_setGraphs(char* cstr) { setGraphs(cstr); }

#ifdef __cplusplus
}
#endif
