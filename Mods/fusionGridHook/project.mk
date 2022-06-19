MODSRC   := $(wildcard $(subdirectory)/*.c) $(wildcard $(subdirectory)/*.cpp)
MODLLSRC := $(wildcard $(subdirectory)/*.asm)

$(eval $(call registerProject, $(MODSRC), $(MODLLSRC)))
