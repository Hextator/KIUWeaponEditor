#ifndef __KIU3DSUSA1_0_H__
#define __KIU3DSUSA1_0_H__

#define FUNC_ADDR_SVCCONTROLMEMORY         0x0010B1E0
FuncP_svcControlMemory KIU_func_svcControlMemory = (FuncP_svcControlMemory)FUNC_ADDR_SVCCONTROLMEMORY;

#define FUNC_ADDR_FUNC_0x00234B00          0x00234B00
#define FUNC_ADDR_FUNC_0x002C9018          0x002C9018
FuncP_WeaponRefFromInstanceID KIU_func_weaponRefFromInventoryIndex = (FuncP_WeaponRefFromInstanceID)FUNC_ADDR_FUNC_0x00234B00;
FuncP_WeaponRefFromInstanceID KIU_func_weaponRefFromGemInventoryIndex = (FuncP_WeaponRefFromInstanceID)FUNC_ADDR_FUNC_0x002C9018;

#define FUNC_ADDR_FUNC_0x004D9890          0x004D9890
FuncP_SortU8ModIDArray KIU_func_sortU8ModIDArray = (FuncP_SortU8ModIDArray)FUNC_ADDR_FUNC_0x004D9890;

//#define CONTEXT_FUSIONGRID_SAFERXADDRESS 0x00C3198C
#define CONTEXT_FUSIONGRID_SAFERXADDRESS  0x00C95100

#define CONTEXT_FUSIONGRID_UNKSTATICPTR0  0x00D636B4
#define CONTEXT_FUSIONGRID_UNKSTATICPTR1  0x00D636C4

//#define CONTEXT_FUSIONGRID_SAFERWADDRESS 0x00DE9DC8

// These might not be static, but it seems they usually are
// Let's assume they're not
#define CURRENT_SAVEMEDIA_POI_0           (0x00006000 + 0x00000278)
//#define CURRENT_HEARTS_AMOUNT_ADDR        0x00EE9820
#define CURRENT_HEARTS_AMOUNT_OFFSET      (CURRENT_SAVEMEDIA_POI_0 + 0x000001E8)
//#define CURRENT_WEAPON_ARRAY_ADDR         0x00EE9CA8
#define CURRENT_WEAPON_ARRAY_OFFSET       (CURRENT_SAVEMEDIA_POI_0 + 0x00000670)
#define CURRENT_WEAPON_COUNT_OFFSET       (CURRENT_SAVEMEDIA_POI_0 + 0x00010000 + 0x0000016C + 0x00000004)
#define WEAPONINVENTORYSLOT_MAX           1000

// WARNING: This is marked with "NV" because it is non-volatile;
// it will be written to and loaded from the save medium!
// WARNING: When writing to the address specified here,
// approximately 27 weapons can safely be owned in the current save file.
// Any more than that will result in weapon data being written to memory at or after this address,
// which results in data stored here for weapons and variables for custom code corrupting each other.
// This is true for all 3 save files because the current save is always loaded to this area,
// which is why the player's Hearts amount is at a fixed address.
//#define CONTEXT_FUSIONGRID_SAFENVRWADDRESS 0x00EEA000
// WARNING: This still only allows 951 weapons
// Subtract 8 for 16 byte alignment
//#define CONTEXT_FUSIONGRID_SAFENVRWADDRESS (CURRENT_WEAPON_ARRAY_ENDADDR - (48 * SIZE_OF_WEAPON) - 8)

// 0x00EF19A8
//#define CURRENT_WEAPON_ARRAY_ENDADDR      (CURRENT_WEAPON_ARRAY_ADDR + (WEAPONINVENTORYSLOT_MAX * SIZE_OF_WEAPON))

#define WEAPON_COUNT_BITS                                 (0x3FF << 0x00)
#define GET_WEAPON_COUNT(weaponCountData)                 (((weaponCountData) >> 0x00) & 0x3FF)
#define SET_WEAPON_COUNT(weaponCountData, newAmt)         (((weaponCountData) & ~WEAPON_COUNT_BITS) | (((newAmt) & 0x3FF) << 0x00))

//#define CONTEXT_FUSIONGRID_ENDOFCODE       0x00F36000

//#define CONTEXT_FUSIONGRID_ENDOFHEAP       0x08096000

#endif
