#
# This is a  Makefile for NeuroMatrix projects (application or library) 
# with automatic recursive search of source files , compiling and linking.
# Makefile is processed by GNU make and may be used both in Linux and Windows OS
#
# USAGE:
#   For release build                    run 'make'
#   For debug   build                    run 'make DEBUG=y'
#   For debug & release  build           run 'make all'
#   For full  rebuild                    run 'make -B'
#   Multithread build (speed up)         run 'make -j4'
#   To clean                             run 'make clean'
#   To clean DEBUG & RELEASE             run 'make cleanall'
#   To create VisualStudio 2005 project  run 'make vs2005'
#   To create VisualStudio 2015 project  run 'make vs2015'
#   For verbose build (no colorizing)    run 'make VERBOSE=y'
#
# WARNING:
#   Makefile is sensitive to leading or trailing spaces and backslashes in the pathes
#
# DEFENITIONS:
#   PROJECT         - project name     
#   TARGET          - output target (.lib or .abs) file with path
#   INC_DIRS        - set of search pathes for the macro libraries and includes (with -I flag separated)
#   SRC_DIRS        - set of explicit  search pathes of all (.c .cpp .asm) source files (with space separated)
#   LIB_DIRS        - set of search library pathes (with -l flag separated)
#   TMP_DIR         - temporary build-folder (Debug/Release)
#   
#   CC              - C/C++ compiler
#   CC_FLAGS        - C/C++ compiler flags   
#   AS				- assembler
#   AS_FLAGS        - assembler flags for compiling of pure .asm files 
#   AR              - librarian
#   AR_FLAGS        - librarian flags of (libr/linker)
#
#============== PROJECT & BUILD CONFIGURATION ==========
ROOT             = ../..
ifeq ($(OS),Windows_NT)
nmc4vars_win.mk:
	-copy "$(NMC_GCC_TOOLPATH)\nmc4-ide\include\nmc4vars_win.mk" nmc4vars_win.mk

-include 		 nmc4vars_win.mk
SHELL 			 = cmd
endif

-include 		 nmc4vars_win.mk
#include          $(ROOT)/global.mk
DEBUG ?= n

ARCH             =nmc4
#--------------  target & input dirs -------------------
ifeq ($(DEBUG),y)
PROJECT          = libnmopengl-$(ARCH)d
else
PROJECT          = libnmopengl-$(ARCH)
endif
TMP_DIR          = Release
OUT_DIR          = $(ROOT)/lib
TARGET           = $(OUT_DIR)/$(PROJECT).a
INC_DIRS         = -I$(ROOT)/include -I$(NMPP)/include -I$(HAL)/include
LIB_DIRS         = -L$(NMPP)/lib -I$(HAL)/lib
LIBS         	 = -lnmpp-nmc4f -lhal-mc12101
SRC_DIRS         = $(ROOT)/src_proc1/nm $(ROOT)/src_proc1/common

#include          $(ROOT)/make/src-nmc-float-gcc.mk
#------------------------------------------------
CC               = nmc-g++
LD               = nmc-ld
AR               = nmc-ar
AS               = asm
CC_FLAGS 	     = -m$(ARCH) $(INC_DIRS)  -std=c++11 -c -O2
AR_FLAGS         = rcs $(TARGET)
AS_FLAGS         =-$(ARCH) -nm2ms  -split_sir -W-111 -W-109 $(INC_DIRS) 

ifeq ($(DEBUG),y)
	CC_FLAGS	+= -DDEBUG
else
	CC_FLAGS	+= -DNDEBUG
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
.DEFAULT_GOAL=default
default: 
	$(MAKE) $(TARGET) -j4 

$(TARGET): $(TMP_DIR) $(OUT_DIR) $(OBJECTS) 
	$(info "[Linking...]")
	$(AR) rcs $(TARGET) $(OBJECTS)   $(ERRECHO)
	

$(TMP_DIR):
	-mkdir "$(@)"
	
$(OUT_DIR): 
	-mkdir "$(@)"
	

$(TMP_DIR)/%.o: %.c  $(ALL_H)
	$(CC) $(CC_FLAGS) $(<) -o$(@) $(INC_DIRS) $(ERRECHO)

$(TMP_DIR)/%.o: %.cpp  $(ALL_H)
	$(CC) $(CC_FLAGS) $(<) -o$(@) $(INC_DIRS) $(ERRECHO)
	
$(TMP_DIR)/%.o: %.asm  $(ALL_H)
	$(AS) $(AS_FLAGS) $(<) -o$(@) $(ERRECHO)

$(TMP_DIR)/%.o: %.s  $(ALL_H)
	$(AS) $(AS_FLAGS) $(<) -o$(@) $(ERRECHO)
	
	


vs2015:
	premake5 vs2015
	"$(VS140COMNTOOLS)vsvars32" && msbuild $(PROJECT).vcxproj /p:Configuration=Release
	"$(VS140COMNTOOLS)vsvars32" && msbuild $(PROJECT).vcxproj /p:Configuration=Debug
	

#========================== cleanup ===================
include $(ROOT)/clean.mk
