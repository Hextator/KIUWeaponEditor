#ifndef __FUSIONGRIDHOOK_CPP__
#define __FUSIONGRIDHOOK_CPP__

#include "kiu3DSusa1.0.h"

#include "fusionGridHook.h"

HARDWARE_OFFSET_ARM(TARGET_MEDIA_ADDRESS)

NOINLINE
void Hook_fusionGrid(void) {
	HOOK(MOD_SECTION_ADDRESS);
	HEAD;

	COMMENT(" Original instruction from those replaced");
_asm("	add	r5, r4,  #0x2800");
/*
 * Thumb code for testing if compilation is for wrong mode
_asm("	add	r5, r4");
*/
	LOAD_SYM(r0, HookTarget_fusionGrid);
	REG_BRANCH(r0);
	// If the code is being treated as Thumb when it shouldn't be, this becomes necessary
	BYTE_ALIGN(4);
	LABEL(HookTarget_fusionGrid);
	// +0x10 to make room for data meant to be visible to the Rosalina trainer
	LONG(CONTEXT_FUSIONGRID_SAFERXADDRESS + 0x10 | MODE_BIT_ARM);

	HOOKEND;
}

#endif
