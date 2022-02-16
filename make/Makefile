

ROOT             = ..

ifeq ($(OS),Windows_NT)
nmc4vars_win.mk:
	-copy "$(NMC_GCC_TOOLPATH)\nmc4-ide\include\nmc4vars_win.mk" nmc4vars_win.mk

-include 		 nmc4vars_win.mk
SHELL 			 = cmd
endif
#ERRECHO			 = 2>&1|cerrecho
ERRECHO			 = 


ARCH             ?=nmc4
CONFIGURATION	 ?=Debug

#--------------  target & input dirs -------------------
ifeq ($(ARCH),nmc4_float)
PROJECT          = libnmopengl_nmc4f.a
else ifeq ($(ARCH),nmc4_fixed)
PROJECT          = libnmopengl_nmc4.a
endif
OUT_DIR          = $(ROOT)/lib
TARGET           = $(OUT_DIR)/$(PROJECT)
INC_DIRS         = -I$(ROOT)/include -I$(NMPP)/include -I$(HAL)/include -I$(ROOT)/utilities/include 
TMP_DIR			 = $(CONFIGURATION)/$(ARCH)


#include          $(ROOT)/make/src-nmc-float-gcc.mk
#------------------------------------------------
CC               = nmc-g++
AS               = nmc-gcc
AR               = nmc-ar
AR_FLAGS         = rcs $(TARGET)

ifeq ($(ARCH),nmc4_float)
SRC_DIRS         = $(ROOT)/src_proc0/nm $(ROOT)/src_proc0/common $(ROOT)/src_proc0/nmgl $(ROOT)/utilities/src/float $(ROOT)/utilities/src/common
CC_FLAGS 	     = -mnmc4-float $(INC_DIRS) -O0
AS_FLAGS         = -mnmc4-float
else ifeq ($(ARCH),nmc4_fixed)
SRC_DIRS         = $(ROOT)/src_proc1/nm $(ROOT)/src_proc1/common $(ROOT)/utilities/src/fixed $(ROOT)/utilities/src/common
CC_FLAGS 	     = -mnmc4-fixed $(INC_DIRS) -O2
AS_FLAGS         = -mnmc4-fixed
endif
CXX_FLAGS	 	 = -std=c++11
C_FLAGS 	 	 = -std=c99

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
OBJECTS     = $(addprefix $(TMP_DIR)/,$(OBJECTS_C) $(OBJECTS_CPP)  $(OBJECTS_ASM) $(OBJECTS_S))

-include $(OBJECTS:.o=.d)


#======================== BUILD RULES ====================================
.PHONY: all
.DEFAULT_GOAL := all
all:
	$(MAKE) ARCH=nmc4_float -j4 lib
	$(MAKE) ARCH=nmc4_fixed -j4 lib
	@echo all done!	

float:
	$(MAKE) ARCH=nmc4_float -j4 lib
	@echo float done!	

fixed:
	$(MAKE) ARCH=nmc4_fixed -j4 lib
	@echo fixed done!	

x86:
	$(MAKE) -f Makefile_x86
	@echo x86 done!	

.PHONY: lib
lib: $(TARGET)



$(TARGET): $(TMP_DIR) $(OUT_DIR) $(OBJECTS) 
	$(AR) rcs $(TARGET) $(OBJECTS) $(ERRECHO)
	

$(TMP_DIR):
	-mkdir "$(@)"
	
$(OUT_DIR): 
	-mkdir "$(@)"

#depend: .depend
#
#.depend: $(ALL_CPP) $(ALL_ASM) $(ALL_C) $(ALL_S)
#	rm -f ./.depend

$(TMP_DIR)/%.o: %.c Makefile
	$(CC) -c -MMD  $(CC_FLAGS) $(C_FLAGS) $(INC_DIRS) $(<) -o $(@) $(ERRECHO)
#$(CC) -MM $(CC_FLAGS) $(C_FLAGS) $(INC_DIRS) $(<) >  $(@).d

$(TMP_DIR)/%.o: %.cpp Makefile
	$(CC) -c -MMD  $(CC_FLAGS) $(CXX_FLAGS) $(INC_DIRS) $(<) -o $(@) $(ERRECHO)
#$(CC) -MM $(CC_FLAGS) $(CXX_FLAGS) $(INC_DIRS) $(<) >  $(@).d
	
$(TMP_DIR)/%.o: %.asm Makefile
	$(AS) $(AS_FLAGS) -c -mmas -Wa,-split_sir $(<) -o$(@) $(ERRECHO)

$(TMP_DIR)/%.o: %.s Makefile
	$(AS) $(AS_FLAGS) -c -mgas $(<) -o$(@) $(ERRECHO)
	

#========================== cleanup ===================
include $(ROOT)/clean.mk
