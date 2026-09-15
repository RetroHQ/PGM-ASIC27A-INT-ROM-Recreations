				.text
				.global	start
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

				.text
start:			MSR		CPSR_c, #ARM_MODE_IRQ | I_BIT | F_BIT
				ldr		SP, =STACK_IRQ

				MSR		CPSR_c, #ARM_MODE_FIQ | I_BIT | F_BIT
				ldr		SP, =STACK_FIQ

				MSR		CPSR_c, #ARM_MODE_SVC | I_BIT | F_BIT
				ldr		SP, =STACK_SVC

				MSR		CPSR_c, #ARM_MODE_SVC

				// write the region id to shared 64KB memory
				// read from 500158 in 68K memory space
				ldr		r0,=0x38000158
				mov		r1, #0xdd			// overridden with region
				ldr		r2,=0x40000018		// bank seletion
				mov		r3, #0				// bank 0
				mov		r4, #1				// bank 1
				
				// region in bank 1
				strb	r4,[r2]
				strh	r1, [r0]

				// region in bank 0, leave in bank 0
				strb	r3,[r2]
				strh	r1, [r0]

				// jump to external ROM
				mov		r0,#0x8000000
				bx		r0

				.end
