				.text
				.global	start
				.global gRegion
				.extern	main
				.arm
				.section .startup, "ax"

////////////////////////////////////////////////////////////////////////////////

ARM_MODE_FIQ	= 0x11			/* FIQ Fast Interrupts Mode */
ARM_MODE_IRQ	= 0x12			/* IRQ Standard Interrupts Mode */
ARM_MODE_SVC	= 0x13			/* Supervisor Interrupts Mode */

I_BIT			= 0x80			/* Disables IRQ when I bit is set */
F_BIT			= 0x40			/* Disables FIQ when F bit is set */

STACK_IRQ		= 0x10000400	/* typical type1 stack setup */
STACK_FIQ		= 0x100003C0
STACK_SVC		= 0x10000380

////////////////////////////////////////////////////////////////////////////////
// Interrupt vectors

vectors:		b		start				// RESET
				b		vec_null			// Undefined instruction
				b		vec_null			// Software interrupt, ASIC27 passthrough
				b		vec_null			// Prefetch abort
				b		vec_null			// Data abort
				nop							// Reserved
				b		vec_null			// Interrupt
				b		vec_null			// Fast interrupt

////////////////////////////////////////////////////////////////////////////////

vec_null:		b		vec_null

////////////////////////////////////////////////////////////////////////////////

				.bss
gRegion:		.space	4

				.text
start:			MSR		CPSR_c, #ARM_MODE_IRQ | I_BIT | F_BIT
				ldr		SP, =STACK_IRQ

				MSR		CPSR_c, #ARM_MODE_FIQ | I_BIT | F_BIT
				ldr		SP, =STACK_FIQ

				MSR		CPSR_c, #ARM_MODE_SVC | I_BIT | F_BIT
				ldr		SP, =STACK_SVC

				MSR		CPSR_c, #ARM_MODE_SVC

				// set the region to be used, this will be overridden by the
				// PGMBuilder as required
				ldr		r0, =gRegion
				mov		r1, #0xdd
				strb	r1, [r0]

				// init data
				LDR		r1, =__data_init
				LDR		r2, =__data_start
				LDR		r3, =__data_end
initlp:			CMP		r2, r3
				LDRCC	r0, [r1], #4
				STRCC	r0, [r2], #4
				BCC		initlp

				// clear BSS
				LDR		r3, =__bss_end
				MOV		r0, #0
bsslp:			CMP		r2, r3
				STRCC	r0, [r2], #4
				BCC		bsslp
				
				// jump to C code
				
				BL		main

				.end
