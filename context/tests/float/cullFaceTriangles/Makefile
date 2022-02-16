

ROOT    = ../../..
CORE	= float

ifeq ($(CORE),float)
CORE_SUFFIX = f
CORE_NUM = 0
else ifeq ($(CORE),fixed)
CORE_SUFFIX = 
CORE_NUM = 1
else 
$(error unknown CORE)
endif


ifeq ($(OS),Windows_NT)
nmc4vars_win.mk:
	-copy "$(NMC_GCC_TOOLPATH)\nmc4-ide\include\nmc4vars_win.mk" nmc4vars_win.mk

-include 		 nmc4vars_win.mk
SHELL 			 = cmd
endif
#ERRECHO			 = 2>&1|cerrecho

LEGACY ?= 1

ARCH             ?=nmc4

#--------------  target & input dirs -------------------
OUT_DIR			?= .
TARGET			 = $(OUT_DIR)/main.abs
INC_DIRS         = -I$(ROOT)/include -I$(NMPP)/include -I$(HAL)/include -I$(ROOT)/../utilities/include
TMP_DIR			 = obj
SRC_DIRS         = $(ROOT)/src/float $(ROOT)/../utilities/src/float $(ROOT)/../utilities/src/common .

#------------------------------------------------
# assembler
AS               = nmc-gcc
AS_FLAGS         = -mnmc4-$(CORE)

# compiler
CC               = nmc-g++
CC_FLAGS 	     = -mnmc4-$(CORE) $(INC_DIRS) -O0 -std=c++11
#CXX_FLAGS	 	 = -std=c++11
#C_FLAGS 	 	 = -std=c99
CC_FLAGS	    += -DDEBUG_LEVEL=1

# link
LINK             = nmc-gcc
LIBS             = -lnmpp-nmc4$(CORE_SUFFIX) -lhal-mc12101 

LD_SCRIPT	 	 = $(ROOT)/lds/ldscript-$(CORE).ld
ifeq ($(LEGACY),1)
LIB_DIRS         = -L"$(NMPP)/lib" -L"$(HAL)/lib" -L"$(MC12101)/lib" -L"$(NMC_GCC_TOOLPATH)/nmc4-ide/lib"
LINKER_FLAGS     = $(LIB_DIRS) $(LIBS) -Wl,-T$(LD_SCRIPT) -Wl,--whole-archive -lmc12101 -Wl,--no-whole-archive
else
LIB_DIRS         = -L"$(NMPP)/lib" -L"$(HAL)/lib" -L"$(MC12101)/lib" 
LINKER_FLAGS     = $(LIB_DIRS) $(LIBS) -Wl,-T$(LD_SCRIPT) -Wl,--whole-archive -lmc12101load_nm -Wl,--no-whole-archive
endif

.SUFFIXES:

# regular search of *.c *.cpp,*.asm,*.s sources in all $(SRC_DIRS)
ALL_CPP =$(wildcard $(addsuffix /*.cpp,$(SRC_DIRS)))
ALL_ASM =$(wildcard $(addsuffix /*.asm,$(SRC_DIRS)))
ALL_C   =$(wildcard $(addsuffix /*.c,$(SRC_DIRS)))
ALL_S   =$(wildcard $(addsuffix /*.s,$(SRC_DIRS)))

VPATH   = $(SRC_DIRS)

# collecting of objects
OBJECTS_CPP = $(notdir $(patsubst %.cpp,%.o,$(ALL_CPP)))
OBJECTS_ASM = $(notdir $(patsubst %.asm,%.o,$(ALL_ASM)))
OBJECTS_C   = $(notdir $(patsubst %.c,%.o,$(ALL_C)))
OBJECTS_S   = $(notdir $(patsubst %.s,%.o,$(ALL_S)))
OBJECTS     = $(addprefix $(TMP_DIR)/,$(OBJECTS_C) $(OBJECTS_CPP)  $(OBJECTS_ASM) $(OBJECTS_S))

#-include $(OBJECTS:.o=.d)


#======================== BUILD RULES ====================================

.DEFAULT_GOAL := $(TARGET)

$(TARGET): $(OUT_DIR) $(OBJECTS) Makefile
	$(LINK) -o $(TARGET) $(LINKER_FLAGS) $(OBJECTS) $(LIBS)
	@echo all done

$(TMP_DIR):
	-mkdir "$(@)"
	
$(OUT_DIR): 
	-mkdir "$(@)"

$(TMP_DIR)/%.o: %.c Makefile $(TMP_DIR)
	$(CC) -c $(CC_FLAGS) $(<) -o $(@)

$(TMP_DIR)/%.o: %.cpp Makefile $(TMP_DIR)
	$(CC) -c $(CC_FLAGS) $(<) -o $(@)
	
$(TMP_DIR)/%.o: %.asm Makefile $(TMP_DIR)
	$(AS) -c $(AS_FLAGS) -mmas $(<) -o$(@) 

$(TMP_DIR)/%.o: %.s Makefile $(TMP_DIR)
	$(AS) -c $(AS_FLAGS) -mgas $(<) -o$(@)
	

#========================== run ===================
run-legacy:
	mc12101run $(TARGET) -q

run:
ifeq ($(LEGACY),1)
	mc12101run $(TARGET) -q
else
	mc12101run $(TARGET) -q -a$(CORE_NUM) -p
endif
#========================== cleanup ===================
clean:
	rm -rf *.o *.abs board emu