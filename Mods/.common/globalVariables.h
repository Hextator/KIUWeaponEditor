#ifndef __GLOBALVARIABLES_H__
#define __GLOBALVARIABLES_H__

#include "kiu3DS.h"

// ----------------------------
// Register global variables here
// ----------------------------

#ifdef __VERSION_USA1_0__
#define CONTEXT_FUSIONGRID_SAFEHOOKADDRESS 0x002C7810
#else
#define CONTEXT_FUSIONGRID_SAFEHOOKADDRESS 0x????????
#endif

typedef struct CTX_FusionGrid_ExternalGlobals {
	// Minimal HID polling
	// WARNING: This HAS to be first if it's stored here!
	u32 keysHeld;

	// Code page state
	u32 memoryInitialized;
} CTX_FusionGrid_ExternalGlobals;

CTX_FusionGrid_ExternalGlobals*const fusionGridExternalGlobals = (CTX_FusionGrid_ExternalGlobals*const)CONTEXT_FUSIONGRID_SAFERXADDRESS;

typedef struct CTX_FusionGrid_Globals {
	// Minimal HID polling
	// WARNING: This HAS to be first if it's stored here!
	//u32 keysHeld;

	// User managed HID polling variables
	u32 currKeys;
	u32 lastKeys;

	// For convenience; not intended for use
	u32 pad0;
	u32 pad1;

	// Weapon editor debugging
	Weapon selectedWeapon;
	u32 weaponFavorited;
	s32 weaponType;
	s32 weaponModel;
	s32 rangedHalfStars;
	s32 meleeHalfStars;

	// Weapon editor state
	u32 weaponEditorInitialized;
	s8 weaponEditorSelectedSlot;
} CTX_FusionGrid_Globals;

typedef enum WeaponEditorSlot {
	WEAPONEDITORSLOT_TYPE = 0,
	WEAPONEDITORSLOT_MODEL = 1,
	WEAPONEDITORSLOT_RANGEDSTARS = 2,
	WEAPONEDITORSLOT_MELEESTARS = 3,
	WEAPONEDITORSLOT_MODIFIER1 = 4,
	WEAPONEDITORSLOT_MODIFIER2 = 5,
	WEAPONEDITORSLOT_MODIFIER3 = 6,
	WEAPONEDITORSLOT_MODIFIER4 = 7,
	WEAPONEDITORSLOT_MODIFIER5 = 8,
	WEAPONEDITORSLOT_MODIFIER6 = 9,
	WEAPONEDITORSLOT_MAX = 10,
} WeaponEditorSlot;

BYTE_ALIGN_ENTITY(0x10)
CTX_FusionGrid_Globals fusionGridGlobalsMemory;

//CTX_FusionGrid_Globals*const fusionGridGlobals = (CTX_FusionGrid_Globals*const)CONTEXT_FUSIONGRID_SAFENVRWADDRESS;
CTX_FusionGrid_Globals*const fusionGridGlobals = (CTX_FusionGrid_Globals*const)&fusionGridGlobalsMemory;

//#define RWX_ALLOCATION_ADDRESS             (CONTEXT_FUSIONGRID_ENDOFHEAP - RWX_ALLOCATION_SIZE)
//#define RWX_ALLOCATION_SIZE                0x00010000

//#define CONTEXT_FUSIONGRID_UNUSEDLHADDRESS (((LINEAR_HEAP_LOC + 0x02649C48) + 0x800) & ((u32)(0 - PAGE_SIZE)))

#endif
