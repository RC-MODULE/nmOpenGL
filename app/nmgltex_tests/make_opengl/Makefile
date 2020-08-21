ROOT = ../..
include $(ROOT)/global.mk
# BUILD AND RUN MAKEFILE
# This makefile builds simple nmc-application containing single cpp-file  
# Rule 'run' executes the application on MC12101 board and stores return code in ".exitcode"  file

.SUFFIXES:


TARGET   = opengl-x86

VS ?=vs2015
ALL: $(VS)
.PHONY = vs2005 vs2015


$(TARGET): $(VS)
	
vs2005: $(TARGET).vcproj
	"$(VS80COMNTOOLS)vsvars32" && vcbuild $(TARGET).vcproj	
	

vs2015:	$(TARGET).vcxproj 
	"$(VS140COMNTOOLS)vsvars32" && msbuild $(TARGET).vcxproj    /p:Configuration=Debug

$(TARGET).vcproj:
	premake5 vs2005

$(TARGET).vcxproj:
	premake5 vs2015


run: 
	$(MAKE) -j4 runtarget

	
runtarget: $(TARGET) 
	bin\debug\$(TARGET)
	
	

include $(ROOT)/clean.mk
