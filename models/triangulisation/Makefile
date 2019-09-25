ROOT = ../..
include $(ROOT)/global.mk
# BUILD AND RUN MAKEFILE
# This makefile builds simple nmc-application containing single cpp-file  
# Rule 'run' executes the application on MC12101 board and stores return code in ".exitcode"  file

.SUFFIXES:


ifndef MC12101
$(warning *** WARNING: 'MC12101' environment variable is not defined! Board SDK might not have installed correctly. )
endif 

HOST     = triangulisation


	
$(HOST): vs2015

ALL: $(TARGET0) $(TARGET1) $(HOST)

PATH:=$(MC12101)/bin;$(PATH)

run: $(HOST)
	bin\debug\$(HOST)
	
vs2005: $(HOST).vcproj 
	"$(VS80COMNTOOLS)vsvars32" && vcbuild $(HOST).vcproj

vs2015:	$(HOST).vcxproj
	"$(VS140COMNTOOLS)vsvars32" && msbuild $(HOST).vcxproj /p:Configuration=Debug

$(HOST).vcproj:
	premake5 vs2005

$(HOST).vcxproj:
	premake5 vs2015
	
	
include $(ROOT)/clean.mk