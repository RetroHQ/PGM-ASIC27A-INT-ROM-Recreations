#include "hardware.h"

//#define SUPER

////////////////////////////////////////////////////////////////////////////////
// Command handler for puzzli2

// should be correct, note each value only appears once
u8 puzzli2_level_decode[256] = {
	// 0  ,  1  ,  2  ,  3  ,  4  ,  5   , 6  ,  7  ,  8  ,  9  ,  a  ,  b  ,  c  ,  d  ,  e  ,  f  ,
	0x32, 0x3e, 0xb2, 0x37, 0x31, 0x22, 0xd6, 0x0d, 0x35, 0x5c, 0x8d, 0x3c, 0x7a, 0x5f, 0xd7, 0xac, // 0x0
//   0  ,  0  ,  0  ,  0  ,  0  ,  1  ,  1  ,  0  ,  1  ,  1  ,  0  ,  0  ,  0  ,  0  ,  x  ,  x  ,
	0x53, 0xff, 0xeb, 0x44, 0xe8, 0x11, 0x69, 0x77, 0xd9, 0x34, 0x36, 0x45, 0xa6, 0xe9, 0x1c, 0xc6, // 0x1
//   0  ,  0  ,  x  ,  x  ,  x  ,  0  ,  x  ,  0  ,  x  ,  0  ,  0  ,  0  ,  0  ,  x  ,  0  ,  x  ,
	0x3b, 0xbd, 0xad, 0x2e, 0x18, 0xdf, 0xa1, 0xab, 0xdd, 0x52, 0x57, 0xc2, 0xe5, 0x0a, 0x00, 0x6d, // 0x2
//   0  ,  0  ,  0  ,  1  ,  1  ,  1  ,  1  ,  x  ,  1  ,  1  ,  0  ,  0  ,  1  ,  1  ,  x  ,  0  ,
	0x67, 0x64, 0x15, 0x70, 0xb6, 0x39, 0x27, 0x78, 0x82, 0xd2, 0x71, 0xb9, 0x13, 0xf5, 0x93, 0x92, // 0x3
//   0  ,  x  ,  1  ,  1  ,  x  ,  1  ,  1  ,  1  ,  1  ,  1  ,  1  ,  1  ,  x  ,  0  ,  x  ,  x  ,
	0xfa, 0xe7, 0x5e, 0xb0, 0xf6, 0xaf, 0x95, 0x8a, 0x7c, 0x73, 0xf9, 0x63, 0x86, 0xcb, 0x1a, 0x56, // 0x4
//   0  ,  1  ,  1  ,  0  ,  0  ,  0  ,  0  ,  1  ,  1  ,  1  ,  1  ,  0  ,  1  ,  1  ,  1  ,  0  ,
	0xf1, 0x3a, 0xae, 0x61, 0x01, 0x29, 0x97, 0x23, 0x8e, 0x5d, 0x9a, 0x65, 0x74, 0x21, 0x20, 0x40, // 0x5
//   0  ,  1  ,  1  ,  1  ,  1  ,  0  ,  x  ,  x  ,  x  ,  0  ,  0  ,  1  ,  1  ,  1  ,  1  ,  1  ,
	0xd3, 0x05, 0xa2, 0xe1, 0xbc, 0x9e, 0x1e, 0x10, 0x14, 0x0c, 0x88, 0x9c, 0xec, 0x38, 0xb5, 0x9d, // 0x6
//   1  ,  0  ,  0  ,  x  ,  1  ,  1  ,  0  ,  0  ,  x  ,  0  ,  x  ,  0  ,  0  ,  1  ,  1  ,  1  ,
	0x2d, 0xf7, 0x17, 0x0e, 0x84, 0xc7, 0x7d, 0xce, 0x94, 0x16, 0x48, 0xa8, 0x81, 0x6e, 0x7b, 0xd8, // 0x7
//   1  ,  1  ,  1  ,  1  ,  x  ,  0  ,  x  ,  0  ,  1  ,  1  ,  1  ,  x  ,  x  ,  1  ,  1  ,  1  ,
	0xa7, 0x7f, 0x42, 0xe6, 0xa0, 0x2a, 0xef, 0xee, 0x24, 0xba, 0xb8, 0x7e, 0xc9, 0x2b, 0x90, 0xcc, // 0x8
//   1  ,  x  ,  1  ,  1  ,  1  ,  1  ,  1  ,  1  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,
	0x5b, 0xd1, 0xf3, 0xe2, 0x6f, 0xed, 0x9f, 0xf0, 0x4b, 0x54, 0x8c, 0x08, 0xf8, 0x51, 0x68, 0xc8, // 0x9
//   x  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  x  ,  0  ,  x  ,  0  ,  0  ,  0  ,  0  ,  1  ,
	0x03, 0x0b, 0xbb, 0xc1, 0xe3, 0x4d, 0x04, 0xc5, 0x8f, 0x09, 0x0f, 0xbf, 0x62, 0x49, 0x76, 0x59, // 0xa
//   1  ,  1  ,  1  ,  1  ,  1  ,  x  ,  0  ,  1  ,  1  ,  0  ,  x  ,  1  ,  1  ,  1  ,  1  ,  0  ,
	0x1d, 0x80, 0xde, 0x60, 0x07, 0xe0, 0x1b, 0x66, 0xa5, 0xbe, 0xcd, 0x87, 0xdc, 0xc3, 0x6b, 0x4e, // 0xb
//   0  ,  1  ,  1  ,  1  ,  1  ,  x  ,  0  ,  x  ,  0  ,  x  ,  0  ,  1  ,  1  ,  0  ,  1  ,  1  ,
	0xd0, 0xfd, 0xd4, 0x3f, 0x98, 0x96, 0x2f, 0x4c, 0xb3, 0xea, 0x2c, 0x75, 0xe4, 0xc0, 0x6c, 0x6a, // 0xc
//   0  ,  x  ,  1  ,  1  ,  0  ,  1  ,  1  ,  1  ,  1  ,  0  ,  1  ,  1  ,  x  ,  1  ,  1  ,  1  ,
	0x9b, 0xb7, 0x43, 0x8b, 0x41, 0x47, 0x02, 0xdb, 0x99, 0x3d, 0xa3, 0x79, 0x50, 0x4f, 0xb4, 0x55, // 0xd
//   1  ,  0  ,  0  ,  0  ,  1  ,  0  ,  0  ,  x  ,  x  ,  1  ,  1  ,  1  ,  0  ,  1  ,  1  ,  1  ,
	0x5a, 0x25, 0xf4, 0xca, 0x58, 0x30, 0xc4, 0x12, 0xa9, 0x46, 0xda, 0x91, 0xa4, 0xaa, 0xfc, 0x85, // 0xe
//   1  ,  1  ,  0  ,  1  ,  1  ,  1  ,  1  ,  0  ,  0  ,  1  ,  1  ,  1  ,  1  ,  0  ,  0  ,  x  ,
	0xfb, 0x89, 0x06, 0xcf, 0xfe, 0x33, 0xd5, 0x28, 0x1f, 0x19, 0x4a, 0xb1, 0x83, 0xf2, 0x72, 0x26, // 0xf
//   x  ,  x  ,  1  ,  1  ,  1  ,  1  ,  1  ,  1  ,  x  ,  0  ,  1  ,  1  ,  1  ,  1  ,  1  ,  1  ,
};

s32 command_31_write_type;
s32 stage;
s32 tableoffs;
s32 tableoffs2;
s32 entries_left;
s32 currentcolumn;
s32 currentrow;
s32 num_entries;
s32 full_entry;
s32 prev_tablloc;
s32 numbercolumns;
s32 depth;
u16 m_row_bitmask;
s32 hackcount;
s32 hackcount2;
s32 hack_47_value;
s32 hack_31_table_offset;
s32 hack_31_table_offset2;
s32 p2_31_retcounter;
u16 level_structure[8][10];
s32 m_puzzli_54_trigger;

u32 count_bits(u16 value)
{
	u32 count = 0;
	for (u32 i = 0; i < 16; i++)
	{
		u32 bit = (value >> i) & 1;

		if (bit) count++;
	}

	return count;
}

s32 get_position_of_bit(u16 value, u32 bit_wanted)
{
	u32 count = 0;
	for (u32 i = 0; i < 16; i++)
	{
		u32 bit = (value >> i) & 1;

		if (bit) count++;

		if (count==(bit_wanted+1))
			return i;
	}

	return -1;
}
	
u32 puzzli2_take_leveldata_value(u8 datvalue)
{
	if (stage == -1)
	{
		entries_left = 0;
		currentcolumn = 0;
		currentrow = 0;
		num_entries = 0;
		full_entry = 0;
		prev_tablloc = 0;
		numbercolumns = 0;
		depth = 0;
		m_row_bitmask = 0;

		tableoffs = datvalue;
		tableoffs2 = 0;
		stage = 0;
	}
	else
	{
		u8 rawvalue = datvalue;
		u8 tableloc = (tableoffs+tableoffs2) & 0xff;
		rawvalue ^= puzzli2_level_decode[tableloc];

		tableoffs2++;
		tableoffs2 &= 0xf;

		if (stage == 0)
		{
			stage = 1;

			// this seems to be the first thing returned back when reading the level structure always seems to be 0x8 or 0x7, makes sense, levels would be too difficult otherwise ;-) (actually puzzli2 seems to have one specifying 5 unless it's a decrypt table error?!)
			depth = (rawvalue & 0xf0);
			numbercolumns = (rawvalue & 0x0f);
			numbercolumns++;
		}
		else if (stage == 1)
		{
			stage = 2;
			entries_left = (rawvalue >> 4);
			m_row_bitmask = (rawvalue & 0x0f)<<8;

			full_entry = rawvalue;
			prev_tablloc = tableloc;

			num_entries = entries_left;
		}
		else if (stage == 2)
		{
			stage = 3;

			m_row_bitmask |= rawvalue;

			int num_mask_bits = count_bits(m_row_bitmask);

			//
			if (entries_left == 0)
			{
				// for 0 entries skip back to state 1 instead of 3, because there is nothing following
				stage = 1;
				currentcolumn++;
				currentrow = 0;
				m_row_bitmask = 0;

				if (currentcolumn == numbercolumns)
				{
					return 1;
				}

			}
			else
			{
				if (num_entries> 0xa)
				{
				}
				else
				{
					// this isn't a strict rule
					// the mask is used so they can specify spaces between elements too without storing the 00 bytes

					int desired_mask = 0;

					if (num_entries == 0x00) desired_mask = 0x00;
					if (num_entries == 0x01) desired_mask = 0x01;
					if (num_entries == 0x02) desired_mask = 0x03;
					if (num_entries == 0x03) desired_mask = 0x07;
					if (num_entries == 0x04) desired_mask = 0x0f;
					if (num_entries == 0x05) desired_mask = 0x1f;
					if (num_entries == 0x06) desired_mask = 0x3f;
					if (num_entries == 0x07) desired_mask = 0x7f;
					if (num_entries == 0x08) desired_mask = 0xff;
					if (num_entries == 0x09) desired_mask = 0xff;
					if (num_entries == 0x0a) desired_mask = 0xff;

				}

			}

		}
		else if (stage == 3)
		{
			u16 object_value;

			// return values
			// 0x0100 = normal fish
			// 0x0120 = fish in bubble
			// 0x0140 = fish in egg
			// 0x0160 = buggy fish in egg (displayed as normal fish)
			// 0x0180 = fish on hook
			// 0x01a0 = fish on hook (uncatchable)
			// 0x01c0 - fish on hook (uncatchable)
			// 0x01e0 - fish on hook (uncatchable)

			// fish values
			// 100 101 102 103 104 105 106 107 108 normal
			// 110 - renders as a flashing fish you can't catch? glitches game?
			// 111 - repeat of other fish type you can't catch...

			if (rawvalue <= 0x10) // regular fish
			{
				int fishtype = rawvalue;
				object_value = 0x0100 + fishtype;
				// 0x110 is a flashy fish? might be glitchy and need a special number..
			}
			else if (rawvalue <= 0x21) // fish in bubbles
			{
				int fishtype = rawvalue - 0x11;
				object_value = 0x0120 + fishtype;
				// 0x130 is a flashy fish? might be glitchy and need a special number..
			}
			else if (rawvalue <= 0x32) // fish in eggs
			{
				int fishtype = rawvalue - 0x22;
				object_value = 0x0140 + fishtype;
				// 0x150 is a flashy fish? might be glitchy and need a special number..

			}
			else if (rawvalue <= 0x43) // fish on hook cases, seem to be base 0x180
			{
				int fishtype = rawvalue - 0x33;
				object_value = 0x0180 + fishtype;
				// 0x190 is a flashy fish? might be glitchy and need a special number..

			}
			////////////////////// special objects follow
			else if (rawvalue == 0xd0) {object_value = 0x0200; }

			else if (rawvalue == 0xe0) {object_value = 0x8000; }
			else if (rawvalue == 0xe1) {object_value = 0x8020; } // solid slant top down
			else if (rawvalue == 0xe2) {object_value = 0x8040; } // solid slant top up
			else if (rawvalue == 0xe3) {object_value = 0x8060; }
			else if (rawvalue == 0xe4) {object_value = 0x8080; } // sold slant bottom up

			else                     {object_value = 0xffff; }

			if (object_value == 0xffff)
			{
				object_value = 0x110;
			}

			const s32 realrow = get_position_of_bit(m_row_bitmask, currentrow);

			if (realrow != -1)
				level_structure[currentcolumn][realrow] = object_value;

			currentrow++;

			entries_left--;
			if (entries_left == 0)
			{
				stage = 1;
				currentcolumn++;
				currentrow = 0;
				m_row_bitmask = 0;

				if (currentcolumn == numbercolumns)
				{
					return 1;
				}

			}
		}

	}

	return 0;
}

void CommandHandler(u32 latch)
{
	u16 latchLow = latch & 0xffff;
	switch ((latch >> 16) & 0xff)
	{
		case 0x31:
		{
			if (command_31_write_type==2)
			{
				// this shouldn't apply to the stuff written on startup, only the level data..

				if (hackcount2==0)
				{
					puzzli2_take_leveldata_value(latchLow & 0xff);

					hack_31_table_offset = latchLow & 0xff;
					hack_31_table_offset2 = 0;
					hackcount2++;
					WriteLatch(0x00d20000);
				}
				else  // how do we decide end?
				{
					int end = puzzli2_take_leveldata_value(latchLow & 0xff);

					if (!end)
					{
						// always d2 0000 when writing doing level data
						// but different for the writes on startup?
						WriteLatch(0x00d20000);

						//u8 tableaddr = (hack_31_table_offset + (hack_31_table_offset2 & 0xf)) & 0xff;
						//u8 xoredval = (latch & 0xffff) ^ puzzli2_level_decode[tableaddr];

						hackcount2++;
						hack_31_table_offset2++;
					}
					else
					{
						hackcount2 = 0;

						// when the ARM detects the end of the stream has been reached it returns a 0x63 status with the number of columns in the data word
						WriteLatch(0x00630000 | numbercolumns);

						//u8 tableaddr = (hack_31_table_offset + (hack_31_table_offset2 & 0xf)) & 0xff;
						//u8 xoredval = (latch & 0xffff) ^ puzzli2_level_decode[tableaddr];
					}
				}
			}
			else
			{
				// todo, responses when uploading the startup values are different

				WriteLatch(0x00d20000 | p2_31_retcounter);
				p2_31_retcounter++; // returns 0xc for the first one, 0x19 for the last one
			}

		}
		break;

		// after writing the compressed and scrambled data stream for the level (copied from ROM) with command 0x31
		// the game expects to read back a fully formed level structure from the ARM
		case 0x13:
		{

			// this is the how to play screen, correctly returned with current code
			/*
			u16 retvals[61] =
			{ 0x0008, // depth (-2?)
			  0x0103, 0x0101, 0x0102, 0x0102, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // first column
			  0x0103, 0x0100, 0x0101, 0x0105, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
			  0x0100, 0x0101, 0x0105, 0x0104, 0x0104, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
			  0x0102, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
			  0x0100, 0x0101, 0x0103, 0x0102, 0x0104, 0x0100 ,0x0100, 0x0000, 0x0000, 0x0000,
			  0x0105, 0x0105, 0x0101, 0x0101, 0x0103, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000  // last column
			};
			*/


			u16* leveldata = &level_structure[0][0];
			if (hackcount==0)
			{
				WriteLatch(0x002d0000 | ((depth>>4)+1)); // this *seems* to come from upper bits of the first real value written to the device during the level stream (verify, seems wrong for some levels because you get a black bar on the bottom of the screen, but might be bad xors)
			}
			else if (hackcount<((10*numbercolumns)+1))
			{
				WriteLatch(0x002d0000 | leveldata[hackcount-1]);
			}
			else
			{
				hackcount=0;
				WriteLatch(0x00740054);  // 0x0074 0054 is returned after how to play reads above.. where does 0x54 come from?

			}

			hackcount++;


			// 2d seems to be used when there is more data available
			// 74 seems to be used when there isn't.. (end of buffer reached?)
			// 2d or 74! (based on?)

		}
		break;



		case 0x38: // Reset
			gValueKey = 0x100;
			WriteLatch(0x780000 | (REGIONSHARE << 8)); // this must also return the cart region or the game will act in odd ways when inserting a coin on continue, or during the game on later levels
			m_puzzli_54_trigger = 0;

		break;




		// 47 and 52 are used to get the images during the intro sequence, different each loop
		// also some other gfx?
		// logic here seems correct, not sure where the 0x19 and 0x5 etc. come from tho!
		case 0x47:

			hack_47_value = latch;

			WriteLatch(0x00740047);

		break;

		case 0x52:
			// it writes a value of 0x0000 then expects to read back like
			// this for the lower part of the game backgrounds
			if (latchLow == 0x0000)
			{
				int val = ((hack_47_value & 0x0f00) >> 8) * 0x19;
				WriteLatch(0x00740000 | (val & 0xffff));
			}
			else
			{
				int val = ((hack_47_value & 0x0f00) >> 8) * 0x19;
				val +=((hack_47_value & 0x000f) >> 0) * 0x05;
				val += latchLow & 0x000f;
				WriteLatch(0x00740000 | (val & 0xffff));

			}



		break;



		case 0x61: // ??
			// this command is written before the values used to decrypt the z80 addresses (assumed) are uploaded with command 31
			command_31_write_type = 1;

			WriteLatch(0x36 << 16);
			p2_31_retcounter = 0xc;
		break;

		case 0x41: // ASIC status?
			// this command is written after the values used to decrypt the z80 addresses (assumed) are uploaded with command 31
			command_31_write_type = 0;

			//m_valueresponse = 0x74 << 16;
			WriteLatch(0x740061);
		break;

		case 0x54: // ??
			// this command is written before uploading the compressed level data stream with command 31

			command_31_write_type = 2;
			stage = -1;
			m_puzzli_54_trigger = 1;
			hackcount2 = 0;
			hackcount = 0;
			WriteLatch(0x36 << 16);

			//  clear the return structure
			u16 *p = (u16 *) &level_structure[0][0];
			u32 c = sizeof(level_structure) / 2;
			while (c--) *p++ = 0;

		break;

		case 0x63: // used as a read address by the 68k code (related to previous uploaded values like cave?) should point at a table of ~0x80 in size? seems to use values as further pointers?
			#ifndef SUPER
				if (latchLow == 0x0000)
				{
					WriteLatch(0x001694a8);
				}
				else if (latchLow == 0x0001)
				{
					WriteLatch(0x0016cfae);
				}
				else if (latchLow == 0x0002)
				{
					WriteLatch(0x0016ebf2); // right for puzzli2 , wrong for puzzli2s, probably calculated from the writes then?
				}
				else if (latchLow == 0x0003) // before 'cast' screen
				{
					WriteLatch(0x0016faa8);
				}
				else if (latchLow == 0x0004) // 2 player demo
				{
					WriteLatch(0x00174416);
				}
				else
				{
					WriteLatch(0x00600000); // wrong

				}
			#else // puzzli2 super
				if (latchLow == 0x0000)
				{
					WriteLatch(0x19027a);
				}
				else if (latchLow == 0x0001)
				{
					WriteLatch(0x193D80);
				}
				else if (latchLow == 0x0002)
				{
					WriteLatch(0x1959c4);
				}
				else if (latchLow == 0x0003)
				{
					WriteLatch(0x19687a);
				}
				else if (latchLow == 0x0004)
				{
					WriteLatch(0x19b1e8);
				}
				else
				{
					WriteLatch(0x00600000); // wrong
				}
			#endif
		break;

		case 0x67: // used as a read address by the 68k code (related to previous uploaded values like cave?) directly reads ~0xDBE from the address..
			#ifndef SUPER
				if ( (latchLow == 0x0000) || (latchLow == 0x0001) || (latchLow == 0x0002) || (latchLow == 0x0003) )
				{
					WriteLatch(0x00166178); // right for puzzli2 , wrong for puzzli2s, probably calculated from the writes then?
				}
				else if ( latchLow == 0x0004 ) // 2 player demo
				{
					WriteLatch(0x00166e72);
				}
				else
				{
					WriteLatch(0x00400000); // wrong
				}
			#else // puzzli2 super
				if ((latchLow == 0x0000) || (latchLow == 0x0001) || (latchLow == 0x0002) ||  (latchLow == 0x0003))
				{
					WriteLatch(0x18cf4a);
				}
				else if ( latchLow == 0x0004 ) // 2 player demo
				{
					WriteLatch(0x0018dc44);
				}
				else
				{
					WriteLatch(0x00600000); // wrong
				}
			#endif
		break;

		default:
			WriteLatch(0x74 << 16);
		break;
	}
}
