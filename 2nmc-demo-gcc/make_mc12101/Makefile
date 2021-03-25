ROOT = ../..
include $(ROOT)/global.mk
# BUILD AND RUN MAKEFILE
# This makefile builds simple nmc-application containing single cpp-file  
# Rule 'run' executes the application on MC12101 board and stores return code in ".exitcode"  file

.SUFFIXES:


ifndef MC12101
$(warning *** WARNING: 'MC12101' environment variable is not defined! Board SDK might not have installed correctly. )
endif 

PLATFORM ?= x64
CONFIGURATION ?= Release
ifdef DEBUG
CONFIGURATION = Debug
endif
BOARD    = mc12101
HOST     = $(BOARD)-host

TARGET0 ?= main0.abs
TARGET1 ?= main1.abs
ifdef DEBUG
TARGET0 ?= main0d.abs
TARGET1 ?= main1d.abs
endif

VS ?= vs2015 
#PATH:=$(MC12101)/bin;$(PATH)
all: $(TARGET0) $(TARGET1)  $(HOST)

nm0 $(TARGET0): 
ifdef DEBUG
	$(MAKE) -f Makefile0 DEBUG=y
else
	$(MAKE) -f Makefile0
endif

nm1 $(TARGET1):
ifdef DEBUG
	$(MAKE) -f Makefile1 DEBUG=y
else
	$(MAKE) -f Makefile1
endif
	
host $(HOST): $(VS)

profile: profile0 profile1
profile0:
	$(MAKE) -f Makefile0 profile
profile1:
	$(MAKE) -f Makefile1 profile

dump1:
	$(MAKE) -f Makefile1 dump
	
vs2005: $(HOST).vcproj 
	"$(VS80COMNTOOLS)vsvars32" && vcbuild $(HOST).vcproj

vs2015:	$(HOST).vcxproj
	"$(VS140COMNTOOLS)vsvars32" && msbuild $(HOST).vcxproj /p:Configuration=Release /p:Platform=x64
#	"$(VS140COMNTOOLS)vsvars32" && msbuild $(HOST).vcxproj /p:Configuration=Debug /p:Platform=x64

$(HOST).vcproj:
	premake5 vs2005

$(HOST).vcxproj:
	premake5 vs2015


run: $(TARGET0) $(TARGET1) $(HOST)
	bin\$(PLATFORM)\$(CONFIGURATION)\$(HOST)-$(PLATFORM)
	
include $(ROOT)/clean.mk
