#ifndef __KIU3DS_H__
#define __KIU3DS_H__

// KIU3DS runs on ARMv6k arch

#include "arm.h"
#include "adaptarm.h"
#include "n3ds.h"

# define MAX_MODIFIERS_PER_COLLECTION      6

typedef struct Weapon {
	u32 unkInt0000;
	u32 weaponBaseID;
	u16 starStrengths[MAX_MODIFIERS_PER_COLLECTION];
	u16 modifiers[MAX_MODIFIERS_PER_COLLECTION];
} Weapon;
#define SIZE_OF_WEAPON                     0x20

#define WEAPON_FAVORITED_BIT                              (0x01 << 0x15)
#define GET_WEAPON_FAVORITED(weaponBaseID)                ((weaponBaseID >> 0x15) & 0x01)
#define SET_WEAPON_FAVORITED(weaponBaseID, boolFavorited) ((weaponBaseID & ~WEAPON_FAVORITED_BIT) | ((boolFavorited & 0x01) << 0x15))
#define WEAPON_TYPE_BITS                                  (0x1F << 0x0A)
#define GET_WEAPON_TYPE(weaponBaseID)                     ((weaponBaseID >> 0x0A) & 0x1F)
#define SET_WEAPON_TYPE(weaponBaseID, weaponTypeID)       ((weaponBaseID & ~WEAPON_TYPE_BITS) | ((weaponTypeID & 0x1F) << 0x0A))
#define WEAPON_MODEL_BITS                                 (0x0F << 0x0F)
#define GET_WEAPON_MODEL(weaponBaseID)                    ((weaponBaseID >> 0x0F) & 0x0F)
#define SET_WEAPON_MODEL(weaponBaseID, weaponModelID)     ((weaponBaseID & ~WEAPON_MODEL_BITS) | ((weaponModelID & 0x0F) << 0x0F))

#define WEAPONTYPEID_MAX                   0x09
#define WEAPONMODELID_MAX                  0x0C

#define STARID_EMPTY                       0x00
#define RANGEDSTARID_NONE                  STARID_EMPTY
#define RANGEDSTARID_MIN                   0x00
#define RANGEDSTARID_MAX                   0x0D
#define MELEESTARID_NONE                   STARID_EMPTY
#define MELEESTARID_MIN                    0x10
#define MELEESTARID_MAX                    0x1D
#define HALFSTARAMOUNT_MIN                 0x00
#define HALFSTARAMOUNT_MAX                 0x0D
#define MODIFIERID_EMPTY                   0x00
#define MODIFIERID_MAX                     0xDD

#define FUNC_ADDR_SVCCONTROLMEMORY         0x0010B1E0
typedef Result(* FuncP_svcControlMemory)(u32*, u32, u32, u32, MemoryOperation, MemoryPermission);
FuncP_svcControlMemory KIU_svcControlMemory = (FuncP_svcControlMemory)FUNC_ADDR_SVCCONTROLMEMORY;

#define FUNC_ADDR_FUNC_0x00234B00          0x00234B00
#define FUNC_ADDR_FUNC_0x002C9018          0x002C9018
typedef Weapon*(* FuncP_WeaponRefFromInstanceID)(u32);
FuncP_WeaponRefFromInstanceID KIU_func_0x00234B00 = (FuncP_WeaponRefFromInstanceID)FUNC_ADDR_FUNC_0x00234B00;
FuncP_WeaponRefFromInstanceID KIU_func_0x002C9018 = (FuncP_WeaponRefFromInstanceID)FUNC_ADDR_FUNC_0x002C9018;

//#define CONTEXT_FUSIONGRID_SAFERXADDRESS 0x00C3198C
#define CONTEXT_FUSIONGRID_SAFERXADDRESS  0x00C95100

#define CONTEXT_FUSIONGRID_UNKSTATICPTR0  0x00D636B4
#define CONTEXT_FUSIONGRID_UNKSTATICPTR1  0x00D636C4

//#define CONTEXT_FUSIONGRID_SAFERWADDRESS 0x00DE9DC8

// These might not be static, but it seems they usually are
#define CURRENT_HEARTS_AMOUNT_ADDR        0x00EE9820
#define CURRENT_WEAPON_ARRAY_ADDR         0x00EE9CA8
#define WEAPONINVENTORYSLOT_MAX           1000

// WARNING: This is marked with "NV" because it is non-volatile;
// it will be written to and loaded from the save medium!
// XXX: When writing to the address specified here,
// approximately 27 weapons can safely be owned in the current save file.
// Any more than that will result in weapon data being written to memory at or after this address,
// which results in data stored here for weapons and variables for custom code corrupting each other.
// This is true for all 3 save files because the current save is always loaded to this area,
// which is why the player's Hearts amount is at a fixed address.
//#define CONTEXT_FUSIONGRID_SAFENVRWADDRESS 0x00EEA000
// WARNING: This still only allows 951 weapons
// Subtract 8 for 16 byte alignment
#define CONTEXT_FUSIONGRID_SAFENVRWADDRESS (CURRENT_WEAPON_ARRAY_ENDADDR - (48 * SIZE_OF_WEAPON) - 8)

// 0x00EF19A8
#define CURRENT_WEAPON_ARRAY_ENDADDR      (CURRENT_WEAPON_ARRAY_ADDR + (WEAPONINVENTORYSLOT_MAX * SIZE_OF_WEAPON))

#define WEAPON_COUNT_BITS                                 (0x3FF << 0x00)
#define GET_WEAPON_COUNT(weaponCountData)                 ((weaponCountData >> 0x00) & 0x3FF)
#define SET_WEAPON_COUNT(weaponCountData, newAmt)         ((weaponCountData & ~WEAPON_COUNT_BITS) | ((newAmt & 0x3FF) << 0x00))

#define CONTEXT_FUSIONGRID_ENDOFCODE       0x00F36000

#define CONTEXT_FUSIONGRID_ENDOFHEAP       0x08096000

#endif
