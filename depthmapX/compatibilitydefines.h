#ifndef COMPATIBILITYDEFINES_H
#define COMPATIBILITYDEFINES_H

#define TRUE 1
#define FALSE 0

#define LOBYTE(w)           ((unsigned char)((w) & 0xff))
#define GetRValue(rgb)      (LOBYTE(rgb))
#define GetGValue(rgb)      (LOBYTE((rgb) >> 8))
#define GetBValue(rgb)      (LOBYTE((rgb)>>16))

#endif // COMPATIBILITYDEFINES_H
