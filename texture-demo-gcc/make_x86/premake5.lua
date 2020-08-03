#!lua

-- A solution contains projects, and defines the available configurations
solution "demo3d-host-x86"
   configurations { "Debug", "Release" }
   -- A project defines one build target
   project "demo3d-host-x86"
		kind "ConsoleApp"
		language "C++"
		files { "../../include/*.h","../src_host/*.cpp","../src_host/*.c", "../../nmglvs_mc12101-gcc/src_host/*.cpp"}
		libdirs { "$(VSHELL)/lib","$(NMPP)/lib","$(HAL)/lib"}
		includedirs { "$(MC12101)/include","$(HAL)/include","$(VSHELL)/include","$(NMPP)/include","../../include"}
		links { "vshell.lib","nmpp-x86.lib","hal-virtual-x86.lib"}

      configuration "Debug"
         defines { "DEBUG", "EMULATION", "TEXTURE_ENABLED" }
         symbols  "On" 

      configuration "Release"
         defines { "NDEBUG", "EMULATION", "TEXTURE_ENABLED" }
         symbols  "Off" 
		 
		 
		 
solution "demo3d-target0-x86"
   configurations { "Debug", "Release" }
   --platforms{"x64","x86"}

   -- A project defines one build target
   project "demo3d-target0-x86"
      kind "ConsoleApp"
	  language "C++"
      files { "../../include/*.h","../src_target0/*.*", "../../src_proc0/pc/*.*","../../src_proc0/common/*.*","../../src_proc0/nmgl/*.*","../../nmglvs_mc12101-gcc/src_nmc0/*.*", "Makefile0" }
	  libdirs { "$(NMPP)/lib","$(HAL)/lib"}
	  includedirs { "$(MC12101)/include","$(HAL)/include","$(NMPP)/include","../../include"}
	  links { "nmpp-x86.lib","hal-virtual-x86.lib"}
	 
      configuration "Debug"
         defines { "DEBUG", "TEXTURE_ENABLED" }
         symbols  "On" 

      configuration "Release"
         defines { "NDEBUG", "TEXTURE_ENABLED" }
         symbols  "Off" 
		 
solution "demo3d-target1-x86"
   configurations { "Debug", "Release" }
   --platforms{"x64","x86"}

   -- A project defines one build target
   project "demo3d-target1-x86"
      kind "ConsoleApp"
	  language "C++"
      files { "../../include/*.h","../src_target1/*.*", "../../src_proc1/pc/*.*","../../src_proc1/common/*.*", "../../nmglvs_mc12101-gcc/src_nmc1/*.*", "Makefile1" }
	  libdirs { "$(NMPP)/lib","$(HAL)/lib", "$(VSHELL)/lib"}
	  includedirs { "$(MC12101)/include","$(HAL)/include","$(NMPP)/include","../../include", "$(VSHELL)/include"}
	  links { "nmpp-x86.lib","hal-virtual-x86.lib", "vshell.lib"}
	 
      configuration "Debug"
         defines { "DEBUG", "TEXTURE_ENABLED" }
         symbols  "On" 

      configuration "Release"
         defines { "NDEBUG", "TEXTURE_ENABLED" }
         symbols  "Off" 	   
		   
		  
