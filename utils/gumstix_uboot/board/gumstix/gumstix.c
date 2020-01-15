/*
 * Copyright (C) 2004 Gumstix, Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 *
 * Written by Craig Hughes <craig@gumstix.com>
 *
 */

#include <common.h>
#include <asm/arch/pxa-regs.h>

int board_init( void ){
  DECLARE_GLOBAL_DATA_PTR;

  gd->bd->bi_arch_number = 373;	
  gd->bd->bi_boot_params = 0xa0000100;

  PSSR    = 0x20;

  GPDR1 = CFG_GPDR1_VAL;
  GAFR1_L = CFG_GAFR1_L_VAL;

  return 0;
}

int dram_init( void ){
  DECLARE_GLOBAL_DATA_PTR;

#if ( CONFIG_NR_DRAM_BANKS > 0 )
  gd->bd->bi_dram[0].start = PHYS_SDRAM_1;
  gd->bd->bi_dram[0].size  = PHYS_SDRAM_1_SIZE;
#endif
#if ( CONFIG_NR_DRAM_BANKS > 1 )  
  gd->bd->bi_dram[1].start = PHYS_SDRAM_2;
  gd->bd->bi_dram[1].size  = PHYS_SDRAM_2_SIZE;
#endif
#if ( CONFIG_NR_DRAM_BANKS > 2 )  
  gd->bd->bi_dram[2].start = PHYS_SDRAM_3;
  gd->bd->bi_dram[2].size  = PHYS_SDRAM_3_SIZE;
#endif
#if ( CONFIG_NR_DRAM_BANKS > 3 )
  gd->bd->bi_dram[3].start = PHYS_SDRAM_4;
  gd->bd->bi_dram[3].size  = PHYS_SDRAM_4_SIZE;
#endif
	
  return 0;
}

extern flash_info_t flash_info[];       /* info for FLASH chips */

static u8 system_serial[8];

void get_board_serial(struct tag_serialnr *serialnr)
{
	serialnr->high = (system_serial[0]<<24) + (system_serial[1]<<16) + (system_serial[2]<<8) + system_serial[3];
	serialnr->low  = (system_serial[4]<<24) + (system_serial[5]<<16) + (system_serial[6]<<8) + system_serial[7];
}

/*
 * This hash function converts a structured 64-bit number to an unstructured, more bit-random 64-bit number.
 * The purpose is to provide more randomness for each bit of the system serial number, since we're going to
 * be using some of its bits (by extraction) as the varying part of an ethernet MAC.  So more random bits =
 * less chance of collision
 *
 * The has algorithm is basically going to work by looping through each bit in the incoming number.  If that bit
 * is set, then we'll XOR our result with a bitmask; if not, then we leave the result and continue the loop.
 * Each time through the loop, we rotate the bitmask by one bit position, so that each bit of the incoming number
 * will affect half of the bits of the result, but which bits it modifies are unique to that incoming bit position
 */
void gumstix_serial_hash(u8 *buf)
{
	unsigned int bit_number,byte;
	// Starting bitmask is 0b0101001100001111000000001111111100000000000000001111111111111111
	u8 xor_mask[] = { 0x53, 0x0F, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0xFF };

	u8 result[8];

	// Loop through all 64 bits
	for(bit_number=0; bit_number<64; bit_number++)
	{
		// If the given bit is set in the incoming buffer
		if(buf[bit_number/8] & (1 << (bit_number % 8)))
		{
			// XOR the masked bits of the result
			for(byte=0;byte<8;byte++) result[byte] ^= xor_mask[byte];
		}

		// Now rotate the bitmask
		for(byte=0;byte<8;byte++)
		{
			// Shift the byte up by one bit
			xor_mask[byte] = xor_mask[byte]<<1;
			// Take the most significant bit of the previous byte and tack that on the bottom
			// Special case for byte==0 when we take the MSB of the top byte instead
			xor_mask[byte] |= xor_mask[(byte == 0 ? 7 : byte-1)]>>7;
		}
	}

	// Copy result out
	for(byte=0;byte<8;byte++) buf[byte]=result[byte];
}

int misc_init_r(void)
{
	unsigned char serial[17];

	flash_read_user_serial(flash_info, (void *)system_serial, 0, 8);
	if(0xff == (system_serial[0] & system_serial[1] & system_serial[2] & system_serial[3] &
		system_serial[4] & system_serial[5] & system_serial[6] & system_serial[7]))
	{
		// User serial number is all ones, so use the factory serial number
		flash_read_factory_serial(flash_info, (void *)system_serial, 0, 8);
		gumstix_serial_hash(system_serial);
	}

	sprintf(serial,"%02lX%02lX%02lX%02lX%02lX%02lX%02lX%02lX",system_serial[0],system_serial[1],system_serial[2],system_serial[3],
								system_serial[4],system_serial[5],system_serial[6],system_serial[7]);

	setenv("serial#",serial);
}
