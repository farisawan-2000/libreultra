#include <os_internal.h>
#include "piint.h"

extern OSPiHandle CartRomHandle2;
extern OSPiHandle LeoDiskHandle80098E78;

OSDevMgr __osPiDevMgr = {0};
OSPiHandle *__osPiTable = NULL;
OSPiHandle *__osCurrentHandle[2] = {&CartRomHandle2, &LeoDiskHandle80098E78};
static OSThread piThread;
static char piThreadStack[OS_PIM_STACKSIZE];
static OSMesgQueue piEventQueue;
static OSMesg piEventBuf[1];
static void func_800323FC(void);
void osCreatePiManager(OSPri pri, OSMesgQueue *cmdQ, OSMesg *cmdBuf, s32 cmdMsgCnt)
{
	u32 savedMask;
	OSPri oldPri;
	OSPri myPri;
	if (!__osPiDevMgr.active)
	{
		func_800323FC();
		osCreateMesgQueue(cmdQ, cmdBuf, cmdMsgCnt);
		osCreateMesgQueue(&piEventQueue, (OSMesg*)&piEventBuf, 1);
		if (!__osPiAccessQueueEnabled)
			__osPiCreateAccessQueue();
		osSetEventMesg(OS_EVENT_PI, &piEventQueue, (OSMesg)0x22222222);
		oldPri = -1;
		myPri = osGetThreadPri(NULL);
		if (myPri < pri)
		{
			oldPri = myPri;
			osSetThreadPri(NULL, pri);
		}
		savedMask = __osDisableInt();
		__osPiDevMgr.active = 1;
		__osPiDevMgr.thread = &piThread;
		__osPiDevMgr.cmdQueue = cmdQ;
		__osPiDevMgr.evtQueue = &piEventQueue;
		__osPiDevMgr.acsQueue = &__osPiAccessQueue;
		__osPiDevMgr.dma = osPiRawStartDma;
		__osPiDevMgr.edma = osEPiRawStartDma;
		osCreateThread(&piThread, 0, __osDevMgrMain, &__osPiDevMgr, &piThreadStack[OS_PIM_STACKSIZE], pri);
		osStartThread(&piThread);
		__osRestoreInt(savedMask);
		if (oldPri != -1)
		{
			osSetThreadPri(NULL, oldPri);
		}
	}
}
static void func_800323FC()
{
	CartRomHandle2.latency			= IO_READ(PI_BSD_DOM1_LAT_REG);
	CartRomHandle2.pulse			= IO_READ(PI_BSD_DOM1_PWD_REG);
	CartRomHandle2.pageSize			= IO_READ(PI_BSD_DOM1_PGS_REG);
	CartRomHandle2.relDuration		= IO_READ(PI_BSD_DOM1_RLS_REG);
	CartRomHandle2.domain			= 0;
	LeoDiskHandle80098E78.latency			= IO_READ(PI_BSD_DOM2_LAT_REG);
	LeoDiskHandle80098E78.pulse				= IO_READ(PI_BSD_DOM2_PWD_REG);
	LeoDiskHandle80098E78.pageSize			= IO_READ(PI_BSD_DOM2_PGS_REG);
	LeoDiskHandle80098E78.relDuration		= IO_READ(PI_BSD_DOM2_RLS_REG);
	LeoDiskHandle80098E78.domain			= 1;
}
