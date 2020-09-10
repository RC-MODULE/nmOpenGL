ROOT = ../../..
include $(ROOT)/global.mk
# BUILD AND RUN MAKEFILE
# This makefile builds simple nmc-application containing single cpp-file  
# Rule 'run' executes the application on MC12101 board and stores return code in ".exitcode"  file

.SUFFIXES:


ifndef MC12101
$(warning *** WARNING: 'MC12101' environment variable is not defined! Board SDK might not have installed correctly. )
endif 

BOARD    = mc12101
HOST     = $(BOARD)-x86

TARGET0  = main0.abs
TARGET1  = main1.abs

TARGET_LIB  = main1.abs
TARGET1  = main1.abs
TARGET1  = main1.abs

NMTARGET=

VS ?= vs2015 
#PATH:=$(MC12101)/bin;$(PATH)


all: NMTARGET = 
all: $(TARGET0) $(TARGET1)  $(HOST)

debug: NMTARGET = _debug
debug: $(TARGET0) $(TARGET1)  $(HOST)

nm0 $(TARGET0): 
	$(MAKE) -f Makefile0 $(NMTARGET)

nm1 $(TARGET1):
	$(MAKE) $(NMTARGET) -f Makefile1 $(NMTARGET)
	
host $(HOST): $(VS)

	
vs2005: $(HOST).vcproj 
	"$(VS80COMNTOOLS)vsvars32" && vcbuild $(HOST).vcproj

vs2015:	$(HOST).vcxproj
	"$(VS140COMNTOOLS)vsvars32" && msbuild $(HOST).vcxproj /p:Configuration=Debug

$(HOST).vcproj:
	premake5 vs2005

$(HOST).vcxproj:
	premake5 vs2015


run: $(TARGET0) $(TARGET1) $(HOST)
	bin\debug\$(HOST)

libs:
	
	$(MAKE) -C $(ROOT)/make/nmopengl-nmc4 -f ./Makefile clean
	$(MAKE) -C $(ROOT)/make/nmopengl-nmc4 -f ./Makefile
	
	$(MAKE) -C $(ROOT)/make/nmopengl-nmc4f -f ./Makefile clean
	$(MAKE) -C $(ROOT)/make/nmopengl-nmc4f -f ./Makefile
	
	$(MAKE) -C $(ROOT)/make/nmopengl-x86-x64 -f ./Makefile kill
	$(MAKE) -C $(ROOT)/make/nmopengl-x86-x64 -f ./Makefile
	
	
include $(ROOT)/clean.mk
