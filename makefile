### Make scripts root path

MAKEBUILDING := ../../../../../../../../../MakeBuilding

### Import system variables

include $(MAKEBUILDING)/system-NT.mk

### Import environment root paths

include $(MAKEBUILDING)/environments.mk

### Import solution system

include $(MAKEBUILDING)/solution.mk

### Project variables

# Mod target
TARGET       := kiu3DS

# Version
VERSION      := usa1.0

# Build targets
OUTPUT_DIR   := Codes

### Environment variables

# Target architecture
include $(MAKEBUILDING)/n3ds/architecture.mk

# Project libraries
MODTARGET    := $(HEXDOC)/Media/Games/Reverse Engineering/Console/N3DS/Kid Icarus Series/Uprising
include $(MAKEBUILDING)/n3ds/lib/devkitPro/libconfig.mk

### Generated variables

MANIFESTS    := $(shell find . -name project.mf)
ROSASOURCES  := $(shell find . -name *.rosa)

### Rules

include $(MAKEBUILDING)/n3ds/lib/devkitPro/rules.mk
