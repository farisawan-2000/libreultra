#include <rcp.h>
#include "controller.h"
#include "siint.h"

static void __osPackEepReadData(u8 address);
OSPifRam __osEepPifRam;
s32 osEepromRead(OSMesgQueue* arg0, u8 address, u8* buffer) {
    s32 ret = 0;
    s32 i = 0;
    u16 type;
    u8 *ptr;
    OSContStatus sdata;
    __OSContEepromFormat eepromformat;

    ptr = (u8*)&__osEepPifRam.ramarray;
    __osSiGetAccess();
    ret = __osEepStatus(arg0, &sdata);
    if (ret == 0) {
        type = sdata.type & 0xC000;
        
        switch (type) {
            case 0x8000:
                if (address >= 0x40) {
                    ret = -1;
                }
                break;
            case 0xC000:
                if (address >= 0x100) {
                    ret = -1;
                }
                break;
            default:
                ret = 8;
                break;
        }
    }
    
    if (ret != 0) {
        __osSiRelAccess();
        return ret;
    } else {
        while (sdata.status & 0x80) {
            __osEepStatus(arg0, &sdata);
        }
        __osPackEepReadData(address);
        __osSiRawStartDma(1, &__osEepPifRam);
        osRecvMesg(arg0, NULL, 1);
        __osSiRawStartDma(0, &__osEepPifRam);
        __osContLastCmd = 4;
        osRecvMesg(arg0, NULL, 1);
        for (i = 0; i < 4; i++) {
            ptr += 1;
        }
        eepromformat = *(__OSContEepromFormat *)ptr;
        
        ret = CHNL_ERR(eepromformat);
        if (ret == 0) {
    		for (i = 0; i < ARRLEN(eepromformat.data); i++) {
    			*buffer++ = eepromformat.data[i];
    		}
    	}
        __osSiRelAccess();
        return ret;
    }
}

static void __osPackEepReadData(u8 address) {
    u8 *ptr = (u8*)&__osEepPifRam.ramarray;
    __OSContEepromFormat eepromformat;
    int i;

    __osEepPifRam.pifstatus = 1;
    eepromformat.txsize = CONT_CMD_READ_EEPROM_TX;
	eepromformat.rxsize = CONT_CMD_READ_EEPROM_RX;
	eepromformat.cmd = CONT_CMD_READ_EEPROM;
	eepromformat.address = address;

    for (i = 0; i < MAXCONTROLLERS; i++) {
        *ptr++ = 0;
    }
    ptr += sizeof(__OSContEepromFormat);
    *(__OSContEepromFormat *)(ptr - sizeof(__OSContEepromFormat)) = eepromformat;
    ptr[0] = CONT_CMD_END;
}
