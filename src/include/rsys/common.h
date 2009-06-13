#if !defined (_COMMON_H_)
#define _COMMON_H_

#if defined (_WIN32) && !defined(WIN32)
#define WIN32 /* evil hackage needed to make SDL happy */
#endif

/* #include this first, so we know what wrapper we are. */
#include "rsys/wrappers.h"

/* #include this second, so we know what release type we are. */
#include "rsys/release.h"

#if defined (COMPILE_FOR_HOST)
#include "host-os-config.h"
#include "host-arch-config.h"
#else /* !COMPILE_FOR_HOST */
#include "target-os-config.h"
#include "target-arch-config.h"
#endif /* !COMPILE_FOR_HOST */

#if !defined (CYGWIN32) && defined (USE_WINDOWS_NOT_MAC_TYPEDEFS_AND_DEFINES)
#undef USE_WINDOWS_NOT_MAC_TYPEDEFS_AND_DEFINES
#endif

#if !defined (LINUX) && !defined (MSDOS) && !defined (NEXT) && !defined(CYGWIN32) && !defined (MACOSX)
# error "Unsupported host"
#endif

#include "rsys/cruft.h"
#include "rsys/macros.h"
#include "rsys/types.h"
#include "rsys/mactype.h"
#include "rsys/byteswap.h"

#if !defined USE_WINDOWS_NOT_MAC_TYPEDEFS_AND_DEFINES
#include <syn68k_public.h>
#include "MacTypes.h"
#include "rsys/trapdecl.h"
#include "rsys/stubify.h"
#include "rsys/slash.h"
#include "rsys/error.h"
#include "rsys/lowglobals.h"
#endif

#if !defined (COMPILE_FOR_HOST)
#include "front-end-config.h"
#endif

typedef struct
{
}
host_spf_reply_block;

typedef enum { get, put } getorput_t;
typedef enum { original_sf, new_sf, new_custom_sf } sf_flavor_t;

extern boolean_t host_has_spfcommon (void);
extern boolean_t host_spfcommon (host_spf_reply_block *replyp,
				 const char *prompt, const char *filename,
				 void *fp, void *filef, int numt,
				 void *tl, getorput_t getorput,
				 sf_flavor_t flavor,
				 void *activeList, void *activateproc,
				 void *yourdatap);

#endif /* !_COMMON_H_ */