#ifndef __FUSIONGRIDCAVE_H__
#define __FUSIONGRIDCAVE_H__

#include "kiu3DSusa1.0.h"

#include "../.common/globalVariables.h"

// Address of media to assemble to
#define TARGET_MEDIA_ADDRESS 0

// Specific address within the chosen media to assemble to
// +0x10 to make room for data meant to be visible to the Rosalina trainer
#define MOD_SECTION_ADDRESS (CONTEXT_FUSIONGRID_SAFERXADDRESS - TARGET_MEDIA_ADDRESS + 0x10)

#endif
