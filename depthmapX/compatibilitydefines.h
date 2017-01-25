#ifndef COMPATIBILITYDEFINES_H
#define COMPATIBILITYDEFINES_H

#ifndef _WIN32
#define TRUE 1
#define FALSE 0

#define LOBYTE(w)           ((unsigned char)((w) & 0xff))
#define GetRValue(rgb)      (LOBYTE(rgb))
#define GetGValue(rgb)      (LOBYTE((rgb) >> 8))
#define GetBValue(rgb)      (LOBYTE((rgb)>>16))
#endif

#ifdef _WIN32
#  ifdef MODULE_API_EXPORTS
#    define MODULE_API __declspec(dllexport)
#  else
#    define MODULE_API __declspec(dllimport)
#  endif
#else
#  define MODULE_API
#endif

#endif // COMPATIBILITYDEFINES_H
