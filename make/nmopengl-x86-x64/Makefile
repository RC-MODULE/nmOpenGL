ROOT = ../..
include $(ROOT)/global.mk
#-include ../local.mk

.SUFFIXES:
PLATFORM ?= x64
PROJECT  = nmopengl
OUT_DIR = $(ROOT)/lib


#--------------- X86 build --------------------------------	
vs2015:	$(PROJECT_X86).vcxproj
	"$(VS140COMNTOOLS)vsvars32" && msbuild $(PROJECT).vcxproj /p:Configuration=Debug      /p:Platform=x64
	"$(VS140COMNTOOLS)vsvars32" && msbuild $(PROJECT).vcxproj /p:Configuration=Release    /p:Platform=x64
	"$(VS140COMNTOOLS)vsvars32" && msbuild $(PROJECT).vcxproj /p:Configuration=Debug      /p:Platform=x86
	"$(VS140COMNTOOLS)vsvars32" && msbuild $(PROJECT).vcxproj /p:Configuration=Release    /p:Platform=x86


	
$(PROJECT_X86).vcproj:	premake5.lua 
	premake5 vs2005

$(PROJECT_X86).vcxproj :	premake5.lua 
	premake5 vs2015
	
#--------------- cleanup  --------------------------------	
-include $(ROOT)/clean.mk