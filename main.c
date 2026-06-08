#include "hardware.h"

void CommandHandler(u32 latch);

////////////////////////////////////////////////////////////////////////////////
// Hardware memory mapping

static volatile u32 * const LATCH = (volatile u32 * const) 0x40000000;
static volatile u16 * const SHARERAM = (volatile u16 * const) 0x50800000;

////////////////////////////////////////////////////////////////////////////////
// Globals

//u32 gLatchSave;
//u16 gLastCommand;

u16 gValueKey;

////////////////////////////////////////////////////////////////////////////////
// Helpers

u32 CalcKey()
{
	u32 key = (gValueKey >> 8) | gValueKey;
	key |= key << 16;
	return key;
}

u32 ReadLatch()
{
	return *LATCH;
}

void WriteLatch(u32 v)
{
	*LATCH = v ^ CalcKey();
}

////////////////////////////////////////////////////////////////////////////////
// Generic main type1 loop

void main()
{
	// init
	SHARERAM[4] = gRegion;
	
	// main command handler polling loop
	while (1)
	{
		// read communication latch register
		u32 latch = ReadLatch();

		// see if high word has been written
		// 68k 32bit latch written as 16bit pair
		// ARM little endian reads the second word as the most significant
		u8 highByte = latch >> 24;
		if (highByte)
		{
			if (highByte == 0xff) gValueKey = 0xff00;

			latch ^= CalcKey();

			gValueKey += 0x0100;
			gValueKey &= 0xff00;
			if (gValueKey == 0xff00) gValueKey = 0x0100;

//			gLatchSave = latch;
//			gLastCommand = latch & 0xff;

			// now do command handler
			CommandHandler(latch);
		}
	}
}
