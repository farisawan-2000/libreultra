#include <os_internal.h>
#include <sptask.h>

void osAfterPreNMI(void) {
	__osSpSetPc(0);
}