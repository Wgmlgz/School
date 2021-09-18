
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

PREFIX(void) EXPORT_setLhs(double val) { setLhs(val); }
PREFIX(void) EXPORT_setRhs(double val) { setRhs(val); }
PREFIX(void) EXPORT_setStep(double val) { setStep(val); }
PREFIX(double) EXPORT_getSurface() { return getSurface(); }
PREFIX(void) EXPORT_free(char* p) { free(p); }

#ifdef __cplusplus
}
#endif
