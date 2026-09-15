#include "hardware.h"

////////////////////////////////////////////////////////////////////////////////
// Command handler for Knights of Valour Plus

u32 m_slots[16];
u16 m_kov_c0_value;
u16 m_kov_cb_value;
u16 m_kov_fe_value;
u16 m_curslot;

static const u8 kov_BATABLE[0x40] = {
	0x00,0x29,0x2c,0x35,0x3a,0x41,0x4a,0x4e,0x57,0x5e,0x77,0x79,0x7a,0x7b,0x7c,0x7d,
	0x7e,0x7f,0x80,0x81,0x82,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x90,
	0x95,0x96,0x97,0x98,0x99,0x9a,0x9b,0x9c,0x9e,0xa3,0xd4,0xa9,0xaf,0xb5,0xbb,0xc1
};

static const u8 kov_B0TABLE[16] = { 2, 0, 1, 4, 3 }; // Maps char portraits to tables

void CommandHandler(u32 latch)
{
	u16 latchLow = (u16) latch;

	switch ((latch >> 16) & 0xff)
	{
		case 0x67: // unknown or status check?
		case 0x8e:
		case 0xa3:
		case 0x33: // kovsgqyz (a3)
		case 0x3a: // kovplus
		case 0xc5: // kovplus
			WriteLatch(0x880000);
			break;

		case 0x99: // Reset
			gValueKey = 0x100;
			WriteLatch(0x00880000 | (REGIONSHARE << 8));
			break;

		case 0x9d: // Sprite palette offset
			WriteLatch(0xa00000 + ((latchLow & 0x1f) * 0x40));
			break;

		case 0xb0: // Read from data table
			WriteLatch(kov_B0TABLE[latchLow & 0x0f]);
			break;

		case 0xb4: // Copy slot 'a' to slot 'b'
		case 0xb7: // kovsgqyz (b4)
			WriteLatch(0x880000);
			if (latchLow == 0x0102) latchLow = 0x0100; // why?
			m_slots[(latchLow >> 8) & 0x0f] = m_slots[(latchLow >> 0) & 0x0f];
			break;

		case 0xba: // Read from data table
			WriteLatch(kov_BATABLE[latchLow & 0x3f]);
			break;

		case 0xc0: // Text layer 'x' select
			WriteLatch(0x880000);
			m_kov_c0_value = latchLow;
			break;

		case 0xc3: // Text layer offset
			WriteLatch(0x904000 + ((m_kov_c0_value + (latchLow * 0x40)) * 4));
			break;

		case 0xcb: // Background layer 'x' select
			WriteLatch(0x880000);
			m_kov_cb_value = latchLow;
			break;

		case 0xcc: // Background layer offset
			{
				int y = latchLow;
				if (y & 0x400) y = -(0x400 - (y & 0x3ff));
				WriteLatch(0x900000 + ((m_kov_cb_value + (y * 0x40)) * 4));
			}
			break;

		case 0xd0: // Text palette offset
		case 0xcd: // kovsgqyz (d0)
			WriteLatch(0xa01000 + (latchLow * 0x20));
			break;

		case 0xd6: // Copy slot to slot 0
			WriteLatch(0x880000);
			m_slots[0] = m_slots[latchLow & 0x0f];
			break;

		case 0xdc: // Background palette offset
		case 0x11: // kovsgqyz (dc)
			WriteLatch(0xa00800 + (latchLow * 0x40));
			break;

		case 0xe0: // Sprite palette offset
		case 0x9e: // kovsgqyz (e0)
			WriteLatch(0xa00000 + ((latchLow & 0x1f) * 0x40));
			break;

		case 0xe5: // Write slot (low)
			WriteLatch(0x880000);
			m_slots[m_curslot] = (m_slots[m_curslot] & 0x00ff0000) | ((latchLow & 0xffff) <<  0);
			break;

		case 0xe7: // Write slot (and slot select) (high)
			WriteLatch(0x880000);
			m_curslot = (latchLow >> 12) & 15;
			m_slots[m_curslot] = (m_slots[m_curslot] & 0x0000ffff) | ((latchLow & 0x00ff) << 16);
			break;

		case 0xf0: // Some sort of status read?
			WriteLatch(0x00c000);
			break;

		case 0xf8: // Read slot
		case 0xab: // kovsgqyz (f8)
			WriteLatch(m_slots[latchLow & 0x0f] & 0x00ffffff);
			break;

		case 0xfc: // Adjust damage level to char experience level
			WriteLatch((latchLow * m_kov_fe_value) >> 6);
			break;

		case 0xfe: // Damage level adjust
			WriteLatch(0x880000);
				m_kov_fe_value = latchLow;
		break;

		default:
			WriteLatch(0x880000);
//			logerror("Unknown ASIC27 command: %2.2x data: %4.4x\n", (data ^ m_valuekey) & 0xff, latchLow);
			break;
	}
}
