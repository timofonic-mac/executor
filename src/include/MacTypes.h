#if !defined (_MACTYPES_H_)
#define _MACTYPES_H_

/*
 * Copyright 1986, 1989, 1990, 1995 by Abacus Research and Development, Inc.
 * All rights reserved.
 *
 * $Id: MacTypes.h 86 2005-05-25 00:47:12Z ctm $
 */

#include "rsys/mactype.h"

#ifdef __cplusplus
namespace Executor {
#endif
	
#define a5	(EM_A5)
	
#if !defined(__MACTYPES__) || defined(__cplusplus)
typedef int8 SignedByte;
#endif
typedef uint8 Byte;
#if !defined(__MACTYPES__) || defined(__cplusplus)
typedef int8 *Ptr;
#endif



#if !defined(__MACTYPES__) || defined(__cplusplus)
typedef GUEST<Ptr> *Handle;
#endif


typedef BOOLEAN Boolean;

typedef Byte Str15[16];
typedef Byte Str31[32];
typedef Byte Str32[33];
typedef Byte Str63[64];
typedef Byte Str255[256];
#if !defined(__MACTYPES__) || defined(__cplusplus)
typedef Byte *StringPtr;
#endif
	


typedef GUEST<StringPtr> *StringHandle;
#if !defined(__MACTYPES__) || defined(__cplusplus)

typedef int (*ProcPtr)();
#endif

#if !defined(__MACTYPES__) || defined(__cplusplus)
typedef LONGINT Fixed, Fract;
#endif
/* SmallFract represnts values between 0 and 65535 */
typedef unsigned short SmallFract;

#define MaxSmallFract 0xFFFF

typedef double Extended;

typedef LONGINT Size;

typedef INTEGER OSErr;
typedef LONGINT OSType;
typedef	LONGINT ResType;

typedef	LONGINT	OSErrRET;	/* for smashing d0 just like the Mac */
typedef	LONGINT	INTEGERRET;
typedef	LONGINT	BOOLEANRET;
typedef	LONGINT	SignedByteRET;



struct QHdr { GUEST_STRUCT;
    GUEST< INTEGER> qFlags;
    GUEST< union __qe*> qHead;    /* actually QElemPtr */
    GUEST< union __qe*> qTail;    /* actually QElemPtr */
};
typedef QHdr *QHdrPtr;
typedef union __qe *QElemPtr;



	enum {
		noErr = 0
	};

#if 0
// custom case in mactype.h
/* from Quickdraw.h */
struct Point { GUEST_STRUCT;
    GUEST< INTEGER> v;
    GUEST< INTEGER> h;
};

struct NativePoint {
    INTEGER v;
    INTEGER h;
};
#endif

#define ZEROPOINT(p) (p.v = CWC (0), p.h = CWC (0))

struct Rect { GUEST_STRUCT;
    GUEST<INTEGER> top;
    GUEST<INTEGER> left;
    GUEST<INTEGER> bottom;
    GUEST<INTEGER> right;

    Rect() = default;
    Rect(GUEST<INTEGER> t, GUEST<INTEGER> l, GUEST<INTEGER> b, GUEST<INTEGER> r)
        : top(t), left(l), bottom(b), right(r) {}

};

typedef Rect *RectPtr;

#define RECT_WIDTH(r)				\
({						\
  const Rect *__r = (r);			\
  CW (__r->right) - CW (__r->left);		\
})
#define RECT_HEIGHT(r)				\
({						\
  const Rect *__r = (r);			\
  CW (__r->bottom) - CW (__r->top);		\
})

#define NULL_RECTP	((Rect *) NULL)

#define RECT_ZERO(r)				\
do						\
  memset (r, 0, sizeof (Rect));			\
while (FALSE)

#define RECT_EQUAL_P(r1, r2)			\
({						\
  const uint32 *__p1 = (const uint32 *) (r1);	\
  const uint32 *__p2 = (const uint32 *) (r2);	\
  __p1[0] == __p2[0] && __p1[1] == __p2[1];	\
})

/* from IntlUtil.h */
typedef INTEGER ScriptCode;
typedef INTEGER LangCode;

/* DO NOT DELETE THIS LINE */
extern INTEGER 	ROM85;
extern INTEGER 	DSErrCode;
#ifdef __cplusplus
}
#endif
#endif /* _MACTYPES_H_ */
