#include "hardware.h"

////////////////////////////////////////////////////////////////////////////////
// Command handler for Py2k

u16 gSlot[0x100];
u16 m_py2k2_sprite_pos;
u16 m_py2k2_sprite_base;
u16 m_py2k2_prev_base;

static u16 mux1(u16 v) {
    v ^= 0x0030;
    return (v & 0xfc30) | (((v>>1)&1)<<0) | (((v>>6)&1)<<1) | (((v>>7)&1)<<2) | (((v>>8)&1)<<3) | (((v>>9)&1)<<6) | (((v>>3)&1)<<7) | (((v>>2)&1)<<8) | (((v>>0)&1)<<9);
}

static u16 mux2(u16 v) {
    v ^= 0x03c0;
    return (v & 0xffc0) | (((v>>4)&1)<<0) | (((v>>3)&1)<<1) | (((v>>5)&1)<<2) | (((v>>0)&1)<<3) | (((v>>2)&1)<<4) | (((v>>1)&1)<<5);
}

static u16 mux3(u16 v) {
    return (v & 0xfc40) | (((v>>1)&1)<<0) | (((v>>2)&1)<<1) | (((v>>9)&1)<<2) | (((v>>5)&1)<<3) | (((v>>7)&1)<<4) | (((v>>8)&1)<<5) | (((v>>4)&1)<<7) | (((v>>3)&1)<<8) | (((v>>0)&1)<<9);
}

static u16 mux4(u16 v) {
    v ^= 0x0001;
    return (v & 0xfc01) | (((v>>7)&1)<<1) | (((v>>8)&1)<<2) | (((v>>9)&1)<<3) | (((v>>1)&1)<<4) | (((v>>2)&1)<<5) | (((v>>3)&1)<<6) | (((v>>4)&1)<<7) | (((v>>5)&1)<<8) | (((v>>6)&1)<<9);
}

static u16 mux5(u16 v) {
    v ^= 0x01c0;
    return (v & 0xfdc0) | (((v>>1)&1)<<0) | (((v>>0)&1)<<1) | (((v>>9)&1)<<2) | (((v>>5)&1)<<3) | (((v>>3)&1)<<4) | (((v>>4)&1)<<5) | (((v>>2)&1)<<9);
}

static u16 mux6(u16 v) {
    v ^= 0x0141;
    return (v & 0xfd41) | (((v>>3)&1)<<1) | (((v>>5)&1)<<2) | (((v>>9)&1)<<3) | (((v>>7)&1)<<4) | (((v>>1)&1)<<5) | (((v>>2)&1)<<7) | (((v>>4)&1)<<9);
}

static u16 mux7(u16 v) {
    v ^= 0x0090;
    return (v & 0xfc90) | (((v>>6)&1)<<0) | (((v>>8)&1)<<1) | (((v>>9)&1)<<2) | (((v>>0)&1)<<3) | (((v>>1)&1)<<5) | (((v>>2)&1)<<6) | (((v>>3)&1)<<8) | (((v>>5)&1)<<9);
}

static u16 mux8(u16 v) {
    v ^= 0x02a1;
    return (v & 0xfea1) | (((v>>3)&1)<<1) | (((v>>4)&1)<<2) | (((v>>2)&1)<<3) | (((v>>6)&1)<<4) | (((v>>8)&1)<<6) | (((v>>1)&1)<<8);
}

static u16 mux9(u16 v) {
    return (v & 0xfc00) | (((v>>4)&1)<<0) | (((v>>5)&1)<<1) | (((v>>6)&1)<<2) | (((v>>7)&1)<<3) | (((v>>8)&1)<<4) | (((v>>9)&1)<<5) | (((v>>0)&1)<<6) | (((v>>1)&1)<<7) | (((v>>2)&1)<<8) | (((v>>3)&1)<<9);
}

static u16 mux10(u16 v) {
    return (v & 0xfc00) | (((v>>5)&1)<<0) | (((v>>4)&1)<<1) | (((v>>3)&1)<<2) | (((v>>2)&1)<<3) | (((v>>1)&1)<<4) | (((v>>0)&1)<<5) | (((v>>9)&1)<<6) | (((v>>8)&1)<<7) | (((v>>7)&1)<<8) | (((v>>6)&1)<<9);
}

u32 py2k2_sprite_offset(u16 base, u16 pos)
{
	u16 ret = 0;
	u16 offset = (base * 16) + (pos & 0xf);

	switch (base & ~0x3f)
	{
		case 0x000: ret = mux1(offset); break;
		case 0x040: ret = mux2(offset); break;
		case 0x080: ret = mux3(offset); break;
		case 0x0c0: ret = mux4(offset); break;
		case 0x100: ret = mux1(offset); break;
		case 0x140: ret = mux5(offset); break;
		case 0x180: ret = mux6(offset); break;
		case 0x1c0: ret = mux7(offset); break;
		case 0x200: ret = mux8(offset); break;
		case 0x240: ret = mux9(offset); break;
		case 0x280: ret = mux8(offset); break;
		case 0x2c0: ret = mux3(offset); break;
		case 0x300: ret = mux2(offset); break;
		case 0x340: ret = mux1(offset); break;
		case 0x380: ret = mux4(offset); break;
		case 0x3c0: ret = mux7(offset); break;
		case 0x400: ret = mux8(offset); break;
		case 0x440: ret = mux2(offset); break;
		case 0x480: ret = mux6(offset); break;
		case 0x4c0: ret = mux5(offset); break;
		case 0x500: ret = mux6(offset); break;
		case 0x540: ret = mux1(offset); break;
		case 0x580: ret = mux2(offset); break;
		case 0x5c0: ret = mux7(offset); break;
		case 0x600: ret = mux3(offset); break;
		case 0x640: ret = mux10(offset); break;
	}

	if (offset >= 0xce80/2 && offset <= 0xceff/2) ret -= 0x0100;
	if (offset >= 0xcf00/2 && offset <= 0xcf7f/2) ret += 0x0100;

	return ret;
}

void CommandHandler(u32 latch)
{
	switch ((latch >> 16) & 0xff)
	{
		case 0x30:
			WriteLatch(py2k2_sprite_offset(m_py2k2_sprite_base, m_py2k2_sprite_pos++));
		break;
		case 0x32:
			m_py2k2_sprite_base = latch;
			m_py2k2_sprite_pos = 0;
			WriteLatch(py2k2_sprite_offset(m_py2k2_sprite_base, m_py2k2_sprite_pos++));
		break;
		case 0xba:
			WriteLatch(m_py2k2_prev_base);
			m_py2k2_prev_base = latch;
		break;
		case 0x99: // reset?
			m_py2k2_prev_base = latch;
			gValueKey = 0x100;
			WriteLatch(0x00880000 | (gRegion << 8));
		break;
		case 0xc0:
			WriteLatch(0x880000);
			break;
		case 0xc3:
			WriteLatch(0x904000 + ((gSlot[0xc0] + ((latch & 0xffff) * 0x40)) * 4));
		break;
		case 0xd0:
			WriteLatch(0xa01000 + ((latch & 0xffff) * 0x20));
		break;
		case 0xdc:
			WriteLatch(0xa00800 + ((latch & 0xffff) * 0x40));
		break;
		case 0xe0:
			WriteLatch(0xa00000 + ((latch & 0x1f) * 0x40));
		break;
		case 0xcb: // Background layer 'x' select (pgm3in1, same as kov)
			WriteLatch(0x880000);
		break;
		case 0xcc: // Background layer offset (pgm3in1, same as kov)
		{
			u16 y = latch;
			if (y & 0x400) y = -(0x400 - (y & 0x3ff));
			WriteLatch(0x900000 + ((gSlot[0xcb] + (y * 0x40)) * 4));
		}
		break;
		case 0x33:
		case 0x34:
		case 0x35:
		case 0x37:
		case 0x38:
		default:
			WriteLatch(0x880000);
			break;
	}
}
