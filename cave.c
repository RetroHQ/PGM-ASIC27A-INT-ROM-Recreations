#include "hardware.h"

////////////////////////////////////////////////////////////////////////////////
// Command handler for cave games

u32 gCurSlot;
u32 gSlot[0x40];

void CommandHandler(u32 latch)
{
	switch ((latch >> 16) & 0xff)
	{
		default:
			WriteLatch(0x880000);
			break;

		case 0x40:
			WriteLatch(0x880000);
			gSlot[(latch >> 10) & 0x1F] = (gSlot[(latch >> 5) & 0x1F] + gSlot[(latch >> 0) & 0x1F]) & 0xffffff;
			break;

		case 0x67: // set high bits
			WriteLatch(0x880000);
			gCurSlot = ((latch & 0xff00) >> 8) & 0x3f;
			gSlot[gCurSlot] = (latch & 0x00ff) << 16;
			break;

		case 0xe5: // OR in bits 15:0
			WriteLatch(0x880000);
			gSlot[gCurSlot] |= (latch & 0xffff);
			break;

		case 0x8e: // read back result of operations
			WriteLatch(gSlot[latch & 0x3f]);
			break;

		case 0x99: // reset?
			gValueKey = 0x100;
			WriteLatch(0x00880000 | (REGIONSHARE << 8));
			break;
	}	
}
