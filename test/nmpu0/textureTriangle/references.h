#ifndef __REFERENCES_H__
#define __REFERENCES_H__

#ifdef __GNUC__
#define SECTION(sec) __attribute__((section(sec)))
#else
#define SECTION(sec)
#endif // __GNUC__

extern nm32s references[][1024];

#endif //__REFERENCES_H__
