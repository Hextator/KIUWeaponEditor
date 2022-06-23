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
// WARNING: This shouldn't be needed and breaks SPOT
//#define SIZE_OF_WEAPON                     0x20

#define WEAPON_FAVORITED_BIT                              (0x01 << 0x15)
#define GET_WEAPON_FAVORITED(weaponBaseID)                (((weaponBaseID) >> 0x15) & 0x01)
#define SET_WEAPON_FAVORITED(weaponBaseID, boolFavorited) (((weaponBaseID) & ~WEAPON_FAVORITED_BIT) | (((boolFavorited) & 0x01) << 0x15))
#define WEAPON_GEMMARKED_BIT                              (0x01 << 0x13)
#define GET_WEAPON_GEMMARKED(weaponBaseID)                (((weaponBaseID) >> 0x13) & 0x01)
#define SET_WEAPON_GEMMARKED(weaponBaseID, boolGemMarked) (((weaponBaseID) & ~WEAPON_GEMMARKED_BIT) | (((boolGemMarked) & 0x01) << 0x13))
#define WEAPON_TYPE_BITS                                  (0x1F << 0x0A)
#define GET_WEAPON_TYPE(weaponBaseID)                     (((weaponBaseID) >> 0x0A) & 0x1F)
#define SET_WEAPON_TYPE(weaponBaseID, weaponTypeID)       (((weaponBaseID) & ~WEAPON_TYPE_BITS) | (((weaponTypeID) & 0x1F) << 0x0A))
#define WEAPON_MODEL_BITS                                 (0x0F << 0x0F)
#define GET_WEAPON_MODEL(weaponBaseID)                    (((weaponBaseID) >> 0x0F) & 0x0F)
#define SET_WEAPON_MODEL(weaponBaseID, weaponModelID)     (((weaponBaseID) & ~WEAPON_MODEL_BITS) | (((weaponModelID) & 0x0F) << 0x0F))

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

typedef Result(* FuncP_svcControlMemory)(u32*, u32, u32, u32, MemoryOperation, MemoryPermission);
typedef Weapon*(* FuncP_WeaponRefFromInstanceID)(u32);
typedef void(* FuncP_SortU8ModIDArray)(u8*, int);

#ifdef __VERSION_USA1_0__
#include "kiu3DSusa1.0.h"
#endif

#endif
