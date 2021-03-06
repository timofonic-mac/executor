#if !defined (__SCRAP__)
#define __SCRAP__

#include "ResourceMgr.h"

/*
 * Copyright 1986, 1989, 1990 by Abacus Research and Development, Inc.
 * All rights reserved.
 *
 * $Id: ScrapMgr.h 63 2004-12-24 18:19:43Z ctm $
 */

#define noScrapErr	(-100)
#define noTypeErr	(-102)

namespace Executor {
struct ScrapStuff { GUEST_STRUCT;
    GUEST< LONGINT> scrapSize;
    GUEST< Handle> scrapHandle;
    GUEST< INTEGER> scrapCount;
    GUEST< INTEGER> scrapState;
    GUEST< StringPtr> scrapName;
};
typedef ScrapStuff *PScrapStuff;

#if 0
#if !defined (ScrapHandle_H)
extern GUEST<Handle> 	ScrapHandle_H;
extern GUEST<StringPtr> 	ScrapName_H;
extern LONGINT 	ScrapSize;
extern INTEGER 	ScrapCount;
extern INTEGER 	ScrapState;
#endif

#define ScrapHandle	(ScrapHandle_H.p)
#define ScrapName	(ScrapName_H.p)
#endif

extern pascal trap PScrapStuff C_InfoScrap( void  );
extern pascal trap LONGINT C_UnloadScrap( void  );
extern pascal trap LONGINT C_LoadScrap( void  );
extern LONGINT ROMlib_ZeroScrap( void  ); 
extern pascal trap LONGINT C_ZeroScrap( void  );
extern pascal trap LONGINT C_PutScrap( LONGINT len, ResType rest, Ptr p );
extern pascal trap LONGINT C_GetScrap( Handle h, ResType rest, 
 GUEST<LONGINT> *off );
}
#endif /* __SCRAP__ */
