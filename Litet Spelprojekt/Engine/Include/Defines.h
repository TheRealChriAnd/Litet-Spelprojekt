#pragma once
#if defined(ENGINE_EXPORT)
#define API __declspec(dllexport)
#else
#define API __declspec(dllimport)
#endif

#define NUM_CLIP_DISTANCES 8

#if !defined(Delete)
#define Delete(x) delete x; x = nullptr
#endif

#if !defined(DeleteArr)
#define DeleteArr(x) delete[] x; x = nullptr
#endif

#if !defined(DeleteSafe)
#define DeleteSafe(x) if (x != nullptr) { Delete(x); }
#endif

#if !defined(DeleteArrSafe)
#define DeleteArrSafe(x) if (x != nullptr) { DeleteArr(x); }
#endif