#pragma once

// Windows build
#if defined (_WIN32)
#if defined (DLL_EXPORTS)
#define DLL_CPP_CLASS __declspec(dllexport)
#define DLL_CPP_FUNCTION __declspec(dllexport)
#define DLL_C_FUNCTION extern "C" __declspec(dllexport)
#else
#define DLL_CPP_CLASS __declspec(dllimport)
#define DLL_CPP_FUNCTION __declspec(dllimport)
#define DLL_C_FUNCTION extern "C" __declspec(dllimport)
#endif // SAMPLELIBRARY_DLL_EXPORTS
#endif // _WIN32