#ifndef __FUSIONGRIDCAVE_CPP__
#define __FUSIONGRIDCAVE_CPP__

#include "kiu3DSusa1.0.h"

#include "fusionGridCave.h"

HARDWARE_OFFSET_ARM(TARGET_MEDIA_ADDRESS)

void Context_fusionGridCave(void);

NOINLINE
void Entry_fusionGridCave(void) {
	HOOK(MOD_SECTION_ADDRESS);
	HEAD;

	SAVE_GPRS;

	// Jump to injected code
	LOWCALL(Context_fusionGridCave);

	RESTORE_GPRS;

	COMMENT(" Replaced instructions");
	LOAD_SYM(r0, r5ref);
_asm("	ldr     r0, [r0]");
_asm("	add     r0,  r0, #0x2b400");
_asm("	add     r0,  r0, #0x12c");
_asm("	ldrb    r0, [r0, #0x1]");

	LOAD_SYM(r6, returnAddr);
_asm("	bx      r6");
	LABEL(r5ref);
	LONG(CONTEXT_FUSIONGRID_UNKSTATICPTR0);
	LABEL(returnAddr);
	LONG(CONTEXT_FUSIONGRID_SAFEHOOKADDRESS + 0x10);

	HOOKEND;
}

// Assemble to here
NOINLINE
void __SECTION_FUSIONGRIDCAVE__(void) {
	CONTINUE(Entry_fusionGridCave);

	HOOKEND;
}

// Import our own SVC wrappers
// Maybe just the ones we actually need
//#include "n3dssvc.cpp"
#include "svcWrappers/n3dssvc_DuplicateHandle.cpp"
#include "svcWrappers/n3dssvc_ControlProcessMemory.cpp"
#include "svcWrappers/n3dssvc_CloseHandle.cpp"
// ARM has no intrinsic integer division, so we need another way to clamp values
#include "extensions/arm_modulusclamp_s32_0toN.cpp"

NOINLINE
bool initializeMemory(void) {
	// Don't attempt to make the page writeable if it already is!
	if (0xDEADBEEF == fusionGridExternalGlobals->memoryInitialized) {
		return true;
	}

	// Not using svcOpenProcess
	//u32 pID = (u32)-1;
	//svcGetProcessID(&pID, HANDLEALIAS_ACTIVEPROCESS);

	Handle handleAppProcess = HANDLEALIAS_ACTIVEPROCESS;
	svcDuplicateHandle(&handleAppProcess, HANDLEALIAS_ACTIVEPROCESS);

	// WARNING: Address must be page aligned!
	u32 addr0 = MOD_SECTION_ADDRESS & (~PAGE_MASK);

	// WARNING: Size must be a multiple of "PAGE_SIZE"!
	Result svcResult = svcControlProcessMemory(
		handleAppProcess,
		addr0,
		NULL,
		PAGE_SIZE,
		MemoryOperation(MEMOP_PROTECT),
		MemoryPermission(MEMPERM_RWX)
	);

	svcCloseHandle(handleAppProcess);

	if (0 <= svcResult) {
		fusionGridExternalGlobals->memoryInitialized = 0xDEADBEEF;
		return true;
	}

	return false;
}

NOINLINE
void pollInput(void) {
	fusionGridGlobals->lastKeys = fusionGridGlobals->currKeys;
	fusionGridGlobals->currKeys = fusionGridExternalGlobals->keysHeld;
}

void editWeapon(u32);

NOINLINE
void Context_fusionGridCave(void) {
	PUSH(r7);

	u32 fodderAIndex;

	POPVAR(fodderAIndex);

	editWeapon(fodderAIndex);
}

NOINLINE
void initializeWeaponEditor(void) {
	if (0xDEADBEEF == fusionGridGlobals->weaponEditorInitialized) {
		return;
	}

	// Initalize weapon editor debug variables
	fusionGridGlobals->selectedWeapon.unkInt0000 = 0;
	fusionGridGlobals->selectedWeapon.weaponBaseID = 0;
	for (int i = 0; i < MAX_MODIFIERS_PER_COLLECTION; i++) {
		fusionGridGlobals->selectedWeapon.starStrengths[i] = 0;
		fusionGridGlobals->selectedWeapon.modifiers[i] = 0;
	}
	fusionGridGlobals->weaponFavorited = 0;
	fusionGridGlobals->weaponType = 0;
	fusionGridGlobals->weaponModel = 0;
	fusionGridGlobals->rangedHalfStars = 0;
	fusionGridGlobals->meleeHalfStars = 0;

	// Initalize weapon editor state variables
	fusionGridGlobals->weaponEditorSelectedSlot = 0;

	// Mark initialization as complete
	fusionGridGlobals->weaponEditorInitialized = 0xDEADBEEF;
}

void readStars(Weapon* weapon, s32* outRangedHalfStars, s32* outMeleeHalfStars) {
	s32 tempRangedHalfStars = 0;
	s32 tempMeleeHalfStars = 0;

	u32 stars0 = weapon->starStrengths[0];
	u32 stars1 = weapon->starStrengths[1];

	if (STARID_EMPTY != stars1) {
		// Melee stars take priority for index 1
		if (MELEESTARID_MIN < stars1 && MELEESTARID_MAX > stars1) {
			tempMeleeHalfStars = stars1 - MELEESTARID_MIN;
		}
		else if (RANGEDSTARID_MIN < stars1 && RANGEDSTARID_MAX > stars1) {
			tempRangedHalfStars = stars1 - RANGEDSTARID_MIN;
		}
	}
	// Index 0 stars take priority over index 1 stars
	if (STARID_EMPTY != stars0) {
		// Ranged stars take priority for index 0
		if (RANGEDSTARID_MIN < stars0 && RANGEDSTARID_MAX > stars0) {
			tempRangedHalfStars = stars0 - RANGEDSTARID_MIN;
		}
		else if (MELEESTARID_MIN < stars0 && MELEESTARID_MAX > stars0) {
			tempMeleeHalfStars = stars0 - MELEESTARID_MIN;
		}
	}

	*outRangedHalfStars = tempRangedHalfStars;
	*outMeleeHalfStars = tempMeleeHalfStars;
}

void writeStars(Weapon* weapon, s32 rangedHalfStars, s32 meleeHalfStars) {
	u16 slot0 = 0;
	u16 slot1 = 0;

	if (0 < rangedHalfStars) {
		slot0 = (u16)(rangedHalfStars + RANGEDSTARID_MIN);
		if (0 < meleeHalfStars) {
			slot1 = (u16)(meleeHalfStars + MELEESTARID_MIN);
		}
	}
	else {
		if (0 < meleeHalfStars) {
			slot0 = (u16)(meleeHalfStars + MELEESTARID_MIN);
		}
	}

	weapon->starStrengths[0] = slot0;
	weapon->starStrengths[1] = slot1;

	// Don't use extra star slots
	for (int i = 2; i < MAX_MODIFIERS_PER_COLLECTION; i++) {
		weapon->starStrengths[i] = 0;
	}
}

NOINLINE
void editWeapon(u32 fodderAIndex) {
	// Uh oh...
	if (!initializeMemory()) {
		return;
	}

	// Initialize associated globals
	initializeWeaponEditor();

	// Scan HID
	pollInput();
	u32 keysDown = KEYS_DOWN(fusionGridGlobals->lastKeys, fusionGridGlobals->currKeys);

	// Get the reference to the selected fodder A weapon
	u32 fusionGridStateRefAddr = *((u32*)CONTEXT_FUSIONGRID_UNKSTATICPTR0);
	fusionGridStateRefAddr += 0x2B400 + 0x12C;
	u8 fusionGridStateUnkBool0 = *((u8*)fusionGridStateRefAddr);
	FuncP_WeaponRefFromInstanceID weaponFromIndexFunc = KIU_func_0x00234B00;
	if (1 == fusionGridStateUnkBool0) {
		weaponFromIndexFunc = KIU_func_0x002C9018;
	}
	Weapon* selectedWeapon = weaponFromIndexFunc(fodderAIndex);
	if (NULL == selectedWeapon) {
		return;
	}
	// TOMAYBE: Location may not be static;
	// should we switch to locating it dynamically?
	bool weaponInventoryAtExpectedLocation = 0 == ((((u32)selectedWeapon) - CURRENT_WEAPON_ARRAY_ADDR) - (fodderAIndex * sizeof(Weapon)));

	// Dump debug info about the selected weapon
	// TOMAYBE: Move this into its own function?
	// Inlined memcpy
	u8* destPtr = (u8*)(&fusionGridGlobals->selectedWeapon);
	u8* srcPtr = (u8*)(selectedWeapon);
	for (u32 i = 0; i < sizeof(Weapon); i++) {
		destPtr[i] = srcPtr[i];
	}
	fusionGridGlobals->weaponFavorited = GET_WEAPON_FAVORITED(fusionGridGlobals->selectedWeapon.weaponBaseID);
	fusionGridGlobals->weaponType = GET_WEAPON_TYPE(fusionGridGlobals->selectedWeapon.weaponBaseID);
	fusionGridGlobals->weaponModel = GET_WEAPON_MODEL(fusionGridGlobals->selectedWeapon.weaponBaseID);
	readStars(selectedWeapon, &fusionGridGlobals->rangedHalfStars, &fusionGridGlobals->meleeHalfStars);

	// Weapon editor requires L to be held
	if (!KEYS_PRESSED(fusionGridGlobals->currKeys, HIDBUTTON_L1)) {
		return;
	}

	// R button changes effect of other buttons
	bool alternateMode = false;
	if (KEYS_PRESSED(fusionGridGlobals->currKeys, HIDBUTTON_R1)) {
		alternateMode = true;
	}

	// D Pad Up and D Pad Down change the index of which slot to modify for the selected weapon
	// (e.g. weapon type, weapon model, ranged/melee stars, 1 of the 6 mods)
	if (KEYS_PRESSED(keysDown, HIDBUTTON_UP)) {
		fusionGridGlobals->weaponEditorSelectedSlot -= 1;
		if (alternateMode) {
			fusionGridGlobals->weaponEditorSelectedSlot = 0;
		}
	}
	if (KEYS_PRESSED(keysDown, HIDBUTTON_DOWN)) {
		fusionGridGlobals->weaponEditorSelectedSlot += 1;
		if (alternateMode) {
			fusionGridGlobals->weaponEditorSelectedSlot = WEAPONEDITORSLOT_MAX - 1;
		}
	}
	fusionGridGlobals->weaponEditorSelectedSlot = __aeabix_modulusclamp_s32_0toN(fusionGridGlobals->weaponEditorSelectedSlot, WEAPONEDITORSLOT_MAX);

	if (KEYS_PRESSED(keysDown, HIDBUTTON_B)) {
		if (alternateMode) {
			if (
				WEAPONEDITORSLOT_TYPE == fusionGridGlobals->weaponEditorSelectedSlot ||
				WEAPONEDITORSLOT_MODEL == fusionGridGlobals->weaponEditorSelectedSlot
			) {
				// Clone weapon to the end of the array if there's room,
				// and update the weapon count accordingly
				u32* weaponCountRef = *((u32**)CONTEXT_FUSIONGRID_UNKSTATICPTR1);
				if (NULL != weaponCountRef) {
					u32 weaponCountRefAddr = (u32)weaponCountRef;
					weaponCountRefAddr += 0x6000;
					weaponCountRefAddr += 0x278;
					weaponCountRefAddr += 0x10000;
					weaponCountRefAddr += 0x16C;
					weaponCountRefAddr += 0x4;
					weaponCountRef = (u32*)weaponCountRefAddr;
					u32 weaponCountData = *weaponCountRef;
					int currWeaponCount = GET_WEAPON_COUNT(weaponCountData);
					if (WEAPONINVENTORYSLOT_MAX > currWeaponCount && weaponInventoryAtExpectedLocation) {
						Weapon* newWeapon = (Weapon*)(CURRENT_WEAPON_ARRAY_ADDR + (currWeaponCount * sizeof(Weapon)));
						bool dataEmpty = true;
						destPtr = (u8*)(newWeapon);
						srcPtr = (u8*)(selectedWeapon);
						for (u32 i = 0; i < sizeof(Weapon); i++) {
							if (0 != destPtr[i]) {
								dataEmpty = false;
								break;
							}
						}
						if (dataEmpty) {
							// Inlined memcpy
							for (u32 i = 0; i < sizeof(Weapon); i++) {
								destPtr[i] = srcPtr[i];
							}
							currWeaponCount += 1;
							weaponCountData = SET_WEAPON_COUNT(weaponCountData, currWeaponCount);
							*weaponCountRef = weaponCountData;
						}
					}
				}
			}
		}
	}

	// All of the other button functions actually edit the selected weapon,
	// so let's exit here, if the weapon is favorited, to protect it
	if (0 != GET_WEAPON_FAVORITED(selectedWeapon->weaponBaseID)) {
		return;
	}

	s32 rangedHalfStars = 0;
	s32 meleeHalfStars = 0;

	readStars(selectedWeapon, &rangedHalfStars, &meleeHalfStars);

	bool doEdits = false;
	int typeChangeAmount = 0;
	int modelChangeAmount = 0;
	int halfStarChangeAmount = 0;
	int modIDChangeAmount = 0;
	// D Pad Right has priority
	if (KEYS_PRESSED(keysDown, HIDBUTTON_RIGHT)) {
		typeChangeAmount = 1;
		modelChangeAmount = 1;
		halfStarChangeAmount = 1;
		modIDChangeAmount = 1;
		doEdits = true;
	}
	else if (KEYS_PRESSED(keysDown, HIDBUTTON_LEFT)) {
		typeChangeAmount = -1;
		modelChangeAmount = -1;
		halfStarChangeAmount = -1;
		modIDChangeAmount = -1;
		doEdits = true;
	}
	if (alternateMode) {
		typeChangeAmount *= 2;
		modelChangeAmount *= 3;
		halfStarChangeAmount *= 4;
		modIDChangeAmount *= 16;
	}
	if (doEdits) {
		// XXX: Adapt system doesn't support ARM switches yet
		if (WEAPONEDITORSLOT_TYPE == fusionGridGlobals->weaponEditorSelectedSlot) {
			s32 newType = (s32)(fusionGridGlobals->weaponType + typeChangeAmount);
			newType = __aeabix_modulusclamp_s32_0toN(newType, WEAPONTYPEID_MAX);
			selectedWeapon->weaponBaseID = SET_WEAPON_TYPE(selectedWeapon->weaponBaseID, (u32)newType);
		}
		else if (WEAPONEDITORSLOT_MODEL == fusionGridGlobals->weaponEditorSelectedSlot) {
			s32 newModel = (s32)(fusionGridGlobals->weaponModel + modelChangeAmount);
			newModel = __aeabix_modulusclamp_s32_0toN(newModel, WEAPONMODELID_MAX);
			selectedWeapon->weaponBaseID = SET_WEAPON_MODEL(selectedWeapon->weaponBaseID, (u32)newModel);
		}
		else if (WEAPONEDITORSLOT_RANGEDSTARS == fusionGridGlobals->weaponEditorSelectedSlot) {
			s32 newRangedHalfStars = rangedHalfStars + halfStarChangeAmount;
			newRangedHalfStars = __aeabix_modulusclamp_s32_0toN(newRangedHalfStars, HALFSTARAMOUNT_MAX);
			rangedHalfStars = newRangedHalfStars;
		}
		else if (WEAPONEDITORSLOT_MELEESTARS == fusionGridGlobals->weaponEditorSelectedSlot) {
			s32 newMeleeHalfStars = meleeHalfStars + halfStarChangeAmount;
			newMeleeHalfStars = __aeabix_modulusclamp_s32_0toN(newMeleeHalfStars, HALFSTARAMOUNT_MAX);
			meleeHalfStars = newMeleeHalfStars;
		}
		else if (
			WEAPONEDITORSLOT_MODIFIER1 == fusionGridGlobals->weaponEditorSelectedSlot ||
			WEAPONEDITORSLOT_MODIFIER2 == fusionGridGlobals->weaponEditorSelectedSlot ||
			WEAPONEDITORSLOT_MODIFIER3 == fusionGridGlobals->weaponEditorSelectedSlot ||
			WEAPONEDITORSLOT_MODIFIER4 == fusionGridGlobals->weaponEditorSelectedSlot ||
			WEAPONEDITORSLOT_MODIFIER5 == fusionGridGlobals->weaponEditorSelectedSlot ||
			WEAPONEDITORSLOT_MODIFIER6 == fusionGridGlobals->weaponEditorSelectedSlot
		) {
			int modIndex = fusionGridGlobals->weaponEditorSelectedSlot - WEAPONEDITORSLOT_MODIFIER1;
			s32 newModID = (s32)(selectedWeapon->modifiers[modIndex] + modIDChangeAmount);
			newModID = __aeabix_modulusclamp_s32_0toN(newModID, MODIFIERID_MAX);
			selectedWeapon->modifiers[modIndex] = (u16)newModID;
		}
	}

	// B button resets values or, alternatively, resets the weapon to v100 or sorts modifiers
	// Edits via B button takes precedence over edits via D Pad,
	// so B button edits are handled last
	if (KEYS_PRESSED(keysDown, HIDBUTTON_B)) {
		if (!alternateMode) {
			if (WEAPONEDITORSLOT_TYPE == fusionGridGlobals->weaponEditorSelectedSlot) {
				selectedWeapon->weaponBaseID = SET_WEAPON_TYPE(selectedWeapon->weaponBaseID, 0);
			}
			else if (WEAPONEDITORSLOT_MODEL == fusionGridGlobals->weaponEditorSelectedSlot) {
				selectedWeapon->weaponBaseID = SET_WEAPON_MODEL(selectedWeapon->weaponBaseID, 0);
			}
			else if (WEAPONEDITORSLOT_RANGEDSTARS == fusionGridGlobals->weaponEditorSelectedSlot) {
				rangedHalfStars = HALFSTARAMOUNT_MAX/2;
			}
			else if (WEAPONEDITORSLOT_MELEESTARS == fusionGridGlobals->weaponEditorSelectedSlot) {
				meleeHalfStars = HALFSTARAMOUNT_MAX/2;
			}
			else if (
				WEAPONEDITORSLOT_MODIFIER1 == fusionGridGlobals->weaponEditorSelectedSlot ||
				WEAPONEDITORSLOT_MODIFIER2 == fusionGridGlobals->weaponEditorSelectedSlot ||
				WEAPONEDITORSLOT_MODIFIER3 == fusionGridGlobals->weaponEditorSelectedSlot ||
				WEAPONEDITORSLOT_MODIFIER4 == fusionGridGlobals->weaponEditorSelectedSlot ||
				WEAPONEDITORSLOT_MODIFIER5 == fusionGridGlobals->weaponEditorSelectedSlot ||
				WEAPONEDITORSLOT_MODIFIER6 == fusionGridGlobals->weaponEditorSelectedSlot
			) {
				int modIndex = fusionGridGlobals->weaponEditorSelectedSlot - WEAPONEDITORSLOT_MODIFIER1;
				selectedWeapon->modifiers[modIndex] = MODIFIERID_MAX/2;
			}
		}
	}

	if (KEYS_PRESSED(keysDown, HIDBUTTON_B)) {
		if (alternateMode) {
			if (
				WEAPONEDITORSLOT_RANGEDSTARS == fusionGridGlobals->weaponEditorSelectedSlot ||
				WEAPONEDITORSLOT_MELEESTARS == fusionGridGlobals->weaponEditorSelectedSlot
			) {
				// Reset weapon to v100
				rangedHalfStars = 0;
				meleeHalfStars = 0;
				for (int i = 0; i < MAX_MODIFIERS_PER_COLLECTION; i++) {
					selectedWeapon->modifiers[i] = 0;
				}
			}
		}
	}

	writeStars(selectedWeapon, rangedHalfStars, meleeHalfStars);

	if (KEYS_PRESSED(keysDown, HIDBUTTON_B)) {
		if (alternateMode) {
			if (
				WEAPONEDITORSLOT_MODIFIER1 == fusionGridGlobals->weaponEditorSelectedSlot ||
				WEAPONEDITORSLOT_MODIFIER2 == fusionGridGlobals->weaponEditorSelectedSlot ||
				WEAPONEDITORSLOT_MODIFIER3 == fusionGridGlobals->weaponEditorSelectedSlot ||
				WEAPONEDITORSLOT_MODIFIER4 == fusionGridGlobals->weaponEditorSelectedSlot ||
				WEAPONEDITORSLOT_MODIFIER5 == fusionGridGlobals->weaponEditorSelectedSlot ||
				WEAPONEDITORSLOT_MODIFIER6 == fusionGridGlobals->weaponEditorSelectedSlot
			) {
				// TODO: Sort modifiers
			}
		}
	}
}

#endif
