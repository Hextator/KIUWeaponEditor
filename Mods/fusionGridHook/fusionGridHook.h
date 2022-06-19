#ifndef __FUSIONGRIDHOOK_H__
#define __FUSIONGRIDHOOK_H__

#include "kiu3DSusa1.0.h"

#include "../.common/globalVariables.h"

// Address of media to assemble to
#define TARGET_MEDIA_ADDRESS 0

// Specific address within the chosen media to assemble to
#define MOD_SECTION_ADDRESS (CONTEXT_FUSIONGRID_SAFEHOOKADDRESS - TARGET_MEDIA_ADDRESS)

#endif
