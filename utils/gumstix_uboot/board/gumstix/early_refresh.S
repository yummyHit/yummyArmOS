#define COTULLA_CP15_B1_VAL	0x69052903
             /*  Workaround for early termination of SDRAM autorefresh on exit from 
                 processor's sleep state in B1 stepping of XPA250/210. (sighting 27004) 
	             Need first forced refresh within 8 usec.
             
                 - Code snippet received from validation team, slightly modified
             
               Notes: - MMU assumed to be inactive at this time, so use physical addresses
                      -  Eboot didn't boot on A1 stepping without the leading exclusion,
                         so the core code must be restricted to B1 only.
             */
             
             	mrc	p15, 0, r9, c0, c0, 0		@ Grab CPU ID
             	ldr	r3, =COTULLA_CP15_B1_VAL	@ Load the B1 CPU ID value
             	cmp	r9, r3
             	bne	EARLY_REFRESH_DONE
             
             	@ Eboot loads BIN image into RAM,
             	@ turns off the MMU and then jumps here.
             	@ If we're already executing from RAM, don't mess with it
             
		mov	r0, #0x400000		@ Just at the end of boot device addressing space
		cmp	r0, pc			@ Are we executing from boot flash space?
		bls	EARLY_REFRESH_DONE	@ if outside that space, skip this.


		/*
			Need to set MDREFR:DRI field to 0 for this to work.  Side effect is
			picking the MEMCLK:SDCLK ratio.  The specified value (0x038FF000)
       	    sets that ratio as 2:1.
			This corresponds to the defaults after reset, including sleep reset.
	 		0x038ff000  ==	MDREFR_K2FREE | MDREFR_K1FREE | MDREFR_K0FREE |
        	     			MDREFR_K2DB2 | MDREFR_K2RUN |
							MDREFR_K1DB2 | MDREFR_K1RUN | MDREFR_E1PIN |
							MDREFR_K0DB2 | MDREFR_K0RUN | MDREFR_E0PIN
		*/

				ldr	r0, =0X48000004 			@ Memory controller base physical addr+ REFR Offset.
				mov	r1, #0x03800000
 				orr	r1, r1, #0x000FF000
             	str	r1, [r0]
             
             	mov	r0, #CFG_DRAM_BASE
             	ldr	r1, [r0]				@ CAUSES the 1st row refresh to all partitions
             	mov	r2, #0x2000				@ 8k loo
1:	
             	ldr	r1, [r0]				@ CAUSES a row refresh to all partitions
             	subs	r2, r2, #0x1
             	bpl	1b						@ while >= 0
EARLY_REFRESH_DONE:
