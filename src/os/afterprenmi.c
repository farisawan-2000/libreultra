#include <os_internal.h>
#include <sptask.h>

s32 osAfterPreNMI(void) {
	return __osSpSetPc(0);
}
