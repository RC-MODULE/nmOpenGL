ROOT = ../..


include $(ROOT)/global.mk
include 		 nmc4vars_win.mk
DEBUG := y

ifeq ($(CORE),float)
CORE_NUM	= 0
else ifeq($(CORE),fixed)
CORE_NUM	= 1
else
$(error unknown core)
endif

#-------------- target & input dirs -------------------

PROJECT          = main$(CORE_NUM)
ifdef DEBUG
PROJECT 		 = main$(CORE_NUM)d
endif
OUT_DIR          = .
TARGET           = $(OUT_DIR)/$(PROJECT).abs
INC_DIRS         = -I"$(NMPP)/include" -I$(HAL)/include -I$(ROOT)/include -I$(ROOT)/utilities/include -I$(ROOT)/context/include
SRC_DIRS         = $(ROOT)/nmglvs_mc12101-gcc/src_nmc$(CORE_NUM) ../src_target$(CORE_NUM)
LIB_DIRS         = -L"$(NMC_GCC_TOOLPATH)/nmc4-ide/lib" -L"$(NMPP)/lib" -L"$(HAL)/lib" -L"$(ROOT)/lib"
ifdef DEBUG
TMP_DIR          = Debug
else
TMP_DIR          = Release
endif
#-------------- RELEASE/ALL config -------------------
CC 				 = nmc-g++
CC_FLAGS	 	 = -O2 -fno-builtin-printf -std=c++11 -mnmc4-$(CORE)
AS               = nmc-gcc 	
AS_FLAGS    	 = -mnmc4-$(CORE)
ifeq ($(CORE),float)
LIBS             = -lnmopengl_nmc4f -lnmpp-nmc4f -lhal-mc12101 
else ifeq($(CORE),fixed)
LIBS             = -lnmopengl_nmc4 -lnmpp-nmc4 -lhal-mc12101 
endif
MAP		         = $(PROJECT).map
CFG				 = ldscript-nmc$(CORE_NUM).ld
LINKER_FLAGS     = -Xlinker -Map=$(MAP) $(LIB_DIRS) "-Wl,-T$(CFG)" -Wl,--whole-archive -lmc12101 -Wl,--no-whole-archive -o$(TARGET)

ifdef DEBUG
	CC_FLAGS	+= -DDEBUG
else
	CC_FLAGS	+= -DNDEBUG
endif
#=================== SOURCE & OBJECTS COLLECTION ===========================

.SUFFIXES:

# regular search of *.c *.cpp,*.asm sources in all $(SRC_DIRS)
ALL_CPP =$(wildcard $(addsuffix /*.cpp, $(SRC_DIRS)))
ALL_ASM =$(wildcard $(addsuffix /*.asm, $(SRC_DIRS)))
ALL_C   =$(wildcard $(addsuffix /*.c, $(SRC_DIRS)))

VPATH   = $(SRC_DIRS)

# collecting of objects
OBJECTS_CPP = $(notdir $(patsubst %.cpp,%.o,$(ALL_CPP)))
OBJECTS_ASM = $(notdir $(patsubst %.asm,%.o,$(ALL_ASM)))
OBJECTS_C   = $(notdir $(patsubst %.c,%.o,$(ALL_C)))
OBJECTS     = $(addprefix $(TMP_DIR)/, $(OBJECTS_C) $(OBJECTS_CPP)  $(OBJECTS_ASM))

#======================== BUILD RULES ====================================
.PHONY:all  clean skip

.DEFAULT_GOAL=all
all:
	$(MAKE) CORE=float -j4 build
	$(MAKE) CORE=fixed -j4 build
	$(MAKE) -f Makefile_host
	@echo all done!	
nm0:
	$(MAKE) CORE=float -j4 build
	@echo all done!	
nm1:
	$(MAKE) CORE=fixed -j4 build
	
run:
	$(MAKE) -f Makefile_host run

dump:
	nmc-objdump -d -s -x -l $(PROJECT).abs > $(PROJECT).dasm
build: $(TARGET)
	
$(TMP_DIR):
	-mkdir "$(@)"

$(TARGET): $(TMP_DIR) $(OBJECTS) nmc4vars_win.mk Makefile lib
	$(CC) $(CC_FLAGS) $(LINKER_FLAGS) $(OBJECTS) $(LIBS)
	@echo nm$(CORE_NUM) done

nmc4vars_win.mk:
	copy "$(NMC_GCC_TOOLPATH)\nmc4-ide\include\nmc4vars_win.mk" nmc4vars_win.mk

lib:
	$(MAKE) -C $(ROOT)/make ARCH=nmc4_$(CORE) -j4 lib

$(OUT_DIR):
	-mkdir "$(@)"
	
$(TMP_DIR)/%.o: %.asm Makefile
	$(AS) $(AS_FLAGS) $(<) -o$(@) $(INC_DIRS)
	
$(TMP_DIR)/%.o: %.cpp Makefile
	$(CC) -MMD $(CC_FLAGS) -c $(<) -o$(@) $(INC_DIRS)

$(TMP_DIR)/%.o: %.c Makefile
	$(CC) -MMD $(CC_FLAGS) -c $(<) -o$(@) $(INC_DIRS)
	
	
$(PROFILE):
	python $(HAL)/script/profiler.py > $(PROFILE)
profile: $(TARGET)
	python $(HAL)/script/profiler.py $(MAP) > $(PROFILE)

include $(ROOT)/clean.mk