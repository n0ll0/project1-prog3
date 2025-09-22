#pragma once

// Platform compatibility definitions
#ifdef _WIN32
  // Windows platform
  #ifdef DATASOURCE_EXPORTS
    #define LIBSPEC extern "C" __declspec(dllexport)
  #else
    #define LIBSPEC extern "C" __declspec(dllimport)
  #endif
  
  // Use Microsoft secure string functions
  #define STRCPY_SAFE(dest, size, src) strcpy_s(dest, size, src)
  
#else
  // Non-Windows platforms (Linux, macOS, etc.)
  #define LIBSPEC extern "C"
  
  // Safe string copy implementation for non-Windows
  #include <cstring>
  #include <algorithm>
  inline int strcpy_safe(char* dest, size_t destSize, const char* src) {
    if (!dest || !src || destSize == 0) return -1;
    size_t srcLen = std::strlen(src);
    if (srcLen >= destSize) return -1;
    std::strcpy(dest, src);
    return 0;
  }
  #define STRCPY_SAFE(dest, size, src) strcpy_safe(dest, size, src)
  
#endif