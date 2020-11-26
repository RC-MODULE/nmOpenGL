ROOT = ../../..
include $(ROOT)/global.mk
# BUILD AND RUN MAKEFILE
# This makefile builds simple nmc-application containing single cpp-file  
# Rule 'run' executes the application on MC12101 board and stores return code in ".exitcode"  file

.SUFFIXES:

PLATFORM  ?= x64
CONFIGURATION  ?= Release

TARGET0   = demo3d-target0
TARGET1   = demo3d-target1
HOST      = demo3d-host

ifeq ($(CONFIGURATION),Debug)
	TARGET_SUFFIX = d
else
	TARGET_SUFFIX = 
endif

VS ?=vs2015
ALL: $(VS)
.PHONY = vs2005 vs2015

debug: $(HOST).vcxproj $(TARGET0).vcxproj $(TARGET1).vcxproj
	"$(VS140COMNTOOLS)vsvars32" && msbuild $(HOST).vcxproj    /p:Configuration=Debug /p:Platform=$(PLATFORM)
	"$(VS140COMNTOOLS)vsvars32" && msbuild $(TARGET0).vcxproj /p:Configuration=Debug /p:Platform=$(PLATFORM)
	"$(VS140COMNTOOLS)vsvars32" && msbuild $(TARGET1).vcxproj /p:Configuration=Debug /p:Platform=$(PLATFORM)

$(TARGET0) $(TARGET1) $(HOST): $(VS)
	
vs2005: $(HOST).vcproj $(TARGET0).vcproj $(TARGET1).vcproj
	"$(VS80COMNTOOLS)vsvars32" && vcbuild $(HOST).vcproj	
	"$(VS80COMNTOOLS)vsvars32" && vcbuild $(TARGET0).vcproj
	"$(VS80COMNTOOLS)vsvars32" && vcbuild $(TARGET1).vcproj
	

vs2015:	$(HOST).vcxproj $(TARGET0).vcxproj $(TARGET1).vcxproj
	"$(VS140COMNTOOLS)vsvars32" && msbuild $(HOST).vcxproj    /p:Configuration=$(CONFIGURATION)    /p:Platform=$(PLATFORM)
	"$(VS140COMNTOOLS)vsvars32" && msbuild $(TARGET0).vcxproj /p:Configuration=$(CONFIGURATION)    /p:Platform=$(PLATFORM)
	"$(VS140COMNTOOLS)vsvars32" && msbuild $(TARGET1).vcxproj /p:Configuration=$(CONFIGURATION)    /p:Platform=$(PLATFORM)
#	"$(VS140COMNTOOLS)vsvars32" && msbuild $(HOST).vcxproj    /p:Configuration=Release    /p:Platform=$(PLATFORM)
#	"$(VS140COMNTOOLS)vsvars32" && msbuild $(TARGET0).vcxproj /p:Configuration=Release    /p:Platform=$(PLATFORM)
#	"$(VS140COMNTOOLS)vsvars32" && msbuild $(TARGET1).vcxproj /p:Configuration=Release    /p:Platform=$(PLATFORM)

$(HOST).vcproj $(TARGET0).vcproj $(TARGET1).vcproj:
	premake5 vs2005

$(HOST).vcxproj $(TARGET0).vcxproj $(TARGET1).vcxproj:
	premake5 vs2015


run: $(HOST) $(TARGET0) $(TARGET1)
	$(MAKE) -j4 runhost runtarget0  runtarget1 
	
drun: 
	$(MAKE) -j4 runhostd runtarget0d  runtarget1d 
	
runhost: $(HOST)
	bin\$(PLATFORM)\$(CONFIGURATION)\$(HOST)-$(PLATFORM)$(TARGET_SUFFIX)
	
runtarget0: $(TARGET0) 
	bin\$(PLATFORM)\$(CONFIGURATION)\$(TARGET0)-$(PLATFORM)$(TARGET_SUFFIX)
	
runtarget1: $(TARGET1) 
	bin\$(PLATFORM)\$(CONFIGURATION)\$(TARGET1)-$(PLATFORM)$(TARGET_SUFFIX)
	
runhostd: $(HOST)
	bin\$(PLATFORM)\debug\$(HOST)-$(PLATFORM)d
	
runtarget0d: $(TARGET0) 
	bin\$(PLATFORM)\debug\$(TARGET0)-$(PLATFORM)d
	
runtarget1d: $(TARGET1) 
	bin\$(PLATFORM)\debug\$(TARGET1)-$(PLATFORM)d
	
libs:
	
	$(MAKE) -C $(ROOT)/make/nmopengl-nmc4 -f ./Makefile clean
	$(MAKE) -C $(ROOT)/make/nmopengl-nmc4 -f ./Makefile
	
	$(MAKE) -C $(ROOT)/make/nmopengl-nmc4f -f ./Makefile clean
	$(MAKE) -C $(ROOT)/make/nmopengl-nmc4f -f ./Makefile
	
	$(MAKE) -C $(ROOT)/make/nmopengl-x86-x64 -f ./Makefile kill
	$(MAKE) -C $(ROOT)/make/nmopengl-x86-x64 -f ./Makefile

include $(ROOT)/clean.mk
