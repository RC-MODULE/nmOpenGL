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
         defines { "DEBUG" }
         symbols  "On" 

      configuration "Release"
         defines { "NDEBUG" }
         symbols  "Off" 
		 
		 
		 
solution "demo3d-target0-x86"
   configurations { "Debug", "Release" }
   --platforms{"x64","x86"}

   -- A project defines one build target
   project "demo3d-target0-x86"
      kind "ConsoleApp"
	  language "C++"
      files { "../../include/*.h","../src_target0/*.*","../../nmglvs_mc12101-gcc/src_nmc0/*.*", "Makefile0" }
	  libdirs { "$(NMPP)/lib","$(HAL)/lib", "../../lib"}
	  includedirs { "$(MC12101)/include","$(HAL)/include","$(NMPP)/include","../../include"}
	  links { "nmopengl-x86d.lib", "nmpp-x86.lib","hal-virtual-x86.lib"}
	 
      configuration "Debug"
         defines { "DEBUG" }
         symbols  "On" 

      configuration "Release"
         defines { "NDEBUG" }
         symbols  "Off" 
		 
solution "demo3d-target1-x86"
   configurations { "Debug", "Release" }
   --platforms{"x64","x86"}

   -- A project defines one build target
   project "demo3d-target1-x86"
      kind "ConsoleApp"
	  language "C++"
      files { "../../include/*.h","../src_target1/*.*", "../../nmglvs_mc12101-gcc/src_nmc1/*.*", "Makefile1" }
	  libdirs { "$(NMPP)/lib","$(HAL)/lib", "$(VSHELL)/lib", "../../lib"}
	  includedirs { "$(MC12101)/include","$(HAL)/include","$(NMPP)/include","../../include", "$(VSHELL)/include"}
	  links { "nmopengl-x86d.lib", "nmpp-x86.lib","hal-virtual-x86.lib", "vshell.lib"}
	 
      configuration "Debug"
         defines { "DEBUG" }
         symbols  "On" 

      configuration "Release"
         defines { "NDEBUG" }
         symbols  "Off" 	   
		   
		  