#ifndef __CONTAINEROF_H__
#define __CONTAINEROF_H__


#include <stddef.h>

#define containerof(type, member, mptr)	\
	((type *)(((char *)(mptr)) - offsetof(type, member)))


#endif /* __CONTAINEROF_H__ */
