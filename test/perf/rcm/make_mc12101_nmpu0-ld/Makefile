ROOT=../../../..

# BUILD AND RUN MAKEFILE
# This makefile builds simple nmc-application containing single cpp-file  
# Rule 'run' executes the application on MC12101 board 

#include $(NMC_GCC_TOOLPATH)/nmc4-ide/include/nmc4vars_win.mk
#NMCGCC = $(NMC_GCC_TOOLPATH)/nmc4-ide
-include nmc4vars_win.mk

SHELL =cmd
.SUFFIXES:

#NMPP?=

ifndef MC12101
$(warning *** WARNING: 'MC12101' environment variable is not defined! Board SDK might not have installed correctly. )
endif 

CC       = nmc-g++
AS       = nmc-gcc
CC_FLAGS = -std=c++11 -O2 -Wall -mnmc4-float
PROJECT  = main
TARGET   =$(PROJECT)
INC_DIRS = -I"$(NMPP)/include" -I"$(HAL)/include" -I$(ROOT)/include
LIB_DIRS = -L"$(NMCGCC)/lib" -L"$(NMCGCC)/nmc/lib/nmc4" -L"$(NMPP)/lib" -L"$(HAL)/lib" -L$(ROOT)/lib
LIBS     = -lnmopengl-nmc4f -lnmpp-nmc4f -lhal-mc12101

CFG      =  "-Wl,-Tmc12101board-nmpu0.ld"

$(TARGET): $(SRC_CPP) Makefile
	$(CC) $(CC_FLAGS) main.cpp $(INC_DIRS) $(LIB_DIRS) $(LIBS) -lc $(CFG) -Wl,--whole-archive -lmc12101 -Wl,--no-whole-archive -o $(TARGET) $(ERRECHO)

PATH:=$(MC12101)/bin;$(PATH)

run: $(TARGET)
	make -B 
	mc12101run $(TARGET)

clean:
	-rm -f main *.abs *.elf .*  *.o

skip:
	@echo SDK missing. Compilation is skipped!

reset:
	reset12101
	
nmc4vars_win.mk:
	copy "$(NMC_GCC_TOOLPATH)\nmc4-ide\include\nmc4vars_win.mk" nmc4vars_win.mk
	