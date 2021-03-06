#if !defined(__MEMORY_LAYOUT__)
#define __MEMORY_LAYOUT__

/*
 * This is a cheesy layout that doesn't really work well
 * for Executor/DOS.  It's pretty wasteful, so we'll have to
 * clean it up before we cook a new production version of E/D.
 */

#define PAGE_ZERO_START		0x00000L
#define PAGE_ZERO_SIZE		0x10000L

#define PAGE_ZERO_SHADOW_START	(PAGE_ZERO_START + PAGE_ZERO_SIZE)

#define NATIVE_STACK_START	(PAGE_ZERO_SHADOW_START + PAGE_ZERO_SIZE)
#define NATIVE_STACK_SIZE	((2 * 1024L * 1024L) - (2 * PAGE_ZERO_SIZE))

#define TEXT_SEGMENT_START	(NATIVE_STACK_START + NATIVE_STACK_SIZE)
#define TEXT_SEGMENT_SIZE	(3 * 1024L * 1024L)

#define SYS_ZONE_START		(TEXT_SEGMENT_START + TEXT_SEGMENT_SIZE)
#define SYS_ZONE_SIZE		(512L * 1024L)

#define APPL_ZONE_START		(SYS_ZONE_START + SYS_ZONE_SIZE)
#define APPL_ZONE_SIZE		((16 * 1024L * 1024L) - MAC_STACK_SIZE - \
				 SYS_ZONE_SIZE - TEXT_SEGMENT_SIZE - \
				 NATIVE_STACK_SIZE - (2 * PAGE_ZERO_SIZE) - \
				 MAGIC_SIZE)

#define MAC_STACK_START		(APPL_ZONE_START + APPL_ZONE_SIZE)
#define MAC_STACK_SIZE		(1024L * 1024L - MAGIC_SIZE)

#define MAGIC_START		((16 * 1024L * 1024L) - MAGIC_SIZE)
#define MAGIC_SIZE		4096


#endif /* !defined(__MEMORY_LAYOUT__) */
