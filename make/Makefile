

ROOT             = ..
ifeq ($(OS),Windows_NT)
nmc4vars_win.mk:
	-copy "$(NMC_GCC_TOOLPATH)\nmc4-ide\include\nmc4vars_win.mk" nmc4vars_win.mk

-include 		 nmc4vars_win.mk
SHELL 			 = cmd
endif
ERRECHO			 = 2>&1|errecho

-include 		 nmc4vars_win.mk
#include          $(ROOT)/global.mk


ARCH             ?=nmc4_fixed
CONFIGURATION	 ?=Release

#--------------  target & input dirs -------------------
PROJECT          = libnmopengl_$(ARCH).a
OUT_DIR          = $(ROOT)/lib
TARGET           = $(OUT_DIR)/$(PROJECT)
INC_DIRS         = -I$(ROOT)/include -I$(NMPP)/include -I$(HAL)/include
TMP_DIR			 = $(CONFIGURATION)/$(ARCH)


#include          $(ROOT)/make/src-nmc-float-gcc.mk
#------------------------------------------------
CC               = nmc-g++
AR               = nmc-ar
AS               = nmc-as
AR_FLAGS         = rcs $(TARGET)
#AS_FLAGS         = -$(ARCH) -nm2ms  -split_sir -W-111 -W-109 $(INC_DIRS) -q
AS_FLAGS         = -$(ARCH) -q -split_sir
#AS_FLAGS         = -mmas -x assembler

ifeq ($(ARCH),nmc4_float)
SRC_DIRS         = $(ROOT)/src_proc0/nm $(ROOT)/src_proc0/common $(ROOT)/src_proc0/nmgl
CC_FLAGS 	     = -mnmc4-float $(INC_DIRS)  -std=c++11 -O0
else ifeq ($(ARCH),nmc4_fixed)
SRC_DIRS         = $(ROOT)/src_proc1/nm $(ROOT)/src_proc1/common
CC_FLAGS 	     = -mnmc4-fixed $(INC_DIRS)  -std=c++11 -O2
#AS_FLAGS         += -Wa,-split_sir 
else
$(error unknow ARCH, please define ARCH type.)
endif

ifeq ($(CONFIGURATION),Debug)
CC_FLAGS	+= -DDEBUG
endif
#=================== SOURCE & OBJECTS COLLECTION ===========================
.SUFFIXES:
# regular search of *.c *.cpp,*.asm,*.s sources in all $(SRC_DIRS)
ALL_CPP =$(wildcard $(addsuffix /*.cpp,$(SRC_DIRS)))
ALL_ASM =$(wildcard $(addsuffix /*.asm,$(SRC_DIRS)))
ALL_C   =$(wildcard $(addsuffix /*.c,$(SRC_DIRS)))
ALL_S   =$(wildcard $(addsuffix /*.s,$(SRC_DIRS)))
ALL_H   =$(wildcard $(addsuffix /*.h,$(HEADER_DIRS)))

VPATH    = $(SRC_DIRS)

# collecting of objects
OBJECTS_CPP = $(notdir $(patsubst %.cpp,%.o,$(ALL_CPP)))
OBJECTS_ASM = $(notdir $(patsubst %.asm,%.o,$(ALL_ASM)))
OBJECTS_C   = $(notdir $(patsubst %.c,%.o,$(ALL_C)))
OBJECTS_S   = $(notdir $(patsubst %.s,%.o,$(ALL_S)))
export OBJECTS     = $(addprefix $(TMP_DIR)/,$(OBJECTS_C) $(OBJECTS_CPP)  $(OBJECTS_ASM) $(OBJECTS_S))

#======================== BUILD RULES ====================================
.PHONY: all
.DEFAULT_GOAL := all
all:
	$(MAKE) ARCH=nmc4_float -j4 lib
	$(MAKE) ARCH=nmc4_fixed -j4 lib
	@echo all done!	

.PHONY: lib
lib: $(TARGET)



$(TARGET): $(TMP_DIR) $(OUT_DIR) $(OBJECTS) 
	$(info "[Linking...]")
	$(AR) rcs $(TARGET) $(OBJECTS) $(ERRECHO)
	$(info "[OK!]")
	

$(TMP_DIR):
	-mkdir "$(@)"
	
$(OUT_DIR): 
	-mkdir "$(@)"
	

$(TMP_DIR)/%.o: %.c   $(ALL_H)
	$(CC) $(CC_FLAGS) -c $(<) -o$(@) $(INC_DIRS) $(ERRECHO)

$(TMP_DIR)/%.o: %.cpp $(ALL_H)
	$(CC) $(CC_FLAGS) -c $(<) -o$(@) $(INC_DIRS) $(ERRECHO)
	
$(TMP_DIR)/%.o: %.asm  $(ALL_H)
	$(AS) $(AS_FLAGS) $(<) -o$(@) $(ERRECHO)

$(TMP_DIR)/%.o: %.s  $(ALL_H)
	$(AS) $(AS_FLAGS) $(<) -o$(@) $(ERRECHO)
	

#========================== cleanup ===================
include $(ROOT)/clean.mk
