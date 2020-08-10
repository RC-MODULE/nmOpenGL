ROOT = ../..
include $(ROOT)/global.mk
#-include ../local.mk

.SUFFIXES:
PROJECT  = nmopengl
PROJECT_X86   = $(PROJECT)-x86
TARGET_X86 = $(OUT_DIR)/$(PROJECT_X86).lib
OUT_DIR = $(ROOT)/lib


#--------------- X86 build --------------------------------	
vs2015:	$(PROJECT_X86).vcxproj
	"$(VS140COMNTOOLS)vsvars32" && msbuild $(PROJECT_X86).vcxproj /p:Configuration=Debug
	"$(VS140COMNTOOLS)vsvars32" && msbuild $(PROJECT_X86).vcxproj /p:Configuration=Release

	
vs2005: $(PROJECT_X86).vcproj
	"$(VS80COMNTOOLS)vsvars32" && vcbuild $(PROJECT_X86).vcproj "Debug"


	
$(PROJECT_X86).vcproj:	premake5.lua 
	premake5 vs2005

$(PROJECT_X86).vcxproj :	premake5.lua 
	premake5 vs2015
	
#--------------- cleanup  --------------------------------	
-include $(ROOT)/clean.mk