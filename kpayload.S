@constants -----------------------------------
.pool
	.set printf, 0x801A6BE9
.text
@main code -----------------------------------
.code 32
_start: .global _start
	LDR	R0, =msg_welcome	
	LDR	R3, =printf
	BLX	R3

	@ hfs_MountHFSVolume epilog :
	ADD     SP, SP, #0x110
	POP     {R2-R4}
	MOV     R8, R2
	MOV     R10, R3
	MOV     R11, R4
	POP     {R4-R7,PC}
.align 2
msg_welcome: .asciz	"Kernel payload started.\n"

.end
