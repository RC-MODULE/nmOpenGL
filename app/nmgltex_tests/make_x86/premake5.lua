#!lua

-- A solution contains projects, and defines the available configurations
solution "demo3d-host-x86"
   configurations { "Debug", "Release" }
   -- A project defines one build target
   project "demo3d-host-x86"
		kind "ConsoleApp"
		language "C++"
		files { "../../../include/*.h","../src_host/*.cpp","../src_host/*.c", "../../../nmglvs_mc12101-gcc/src_host/*.cpp"}
		libdirs { "$(VSHELL)/lib","$(NMPP)/lib","$(HAL)/lib"}
		includedirs { "$(MC12101)/include","$(HAL)/include","$(VSHELL)/include","$(NMPP)/include","../../../include"}
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
   --   "../../src_proc0/pc/*.*","../../src_proc0/common/*.*","../../src_proc0/nmgl/*.*",
      files { "../../../include/*.h","../include/*.h","../src_target0/*.*","../src_target0/tests/*.*","../../../nmglvs_mc12101-gcc/src_nmc0/*.*","../../../src_proc0/pc/*.*","../../../src_proc0/common/*.*" }
	  libdirs { "$(NMPP)/lib","$(HAL)/lib", "../../../lib"}
	  includedirs { "$(MC12101)/include","$(HAL)/include","$(NMPP)/include","../../../include","../include"}
     links { "nmpp-x86.lib"} 
     defines { "TEST_NMGL_TEX_FUNC" } 
     configuration "Debug"
         defines { "DEBUG" }
         symbols  "On"
         links { "nmopengl-x86d.lib","hal-virtual-x86d.lib"}   
 
      configuration "Release"
         defines { "NDEBUG" }
         symbols  "Off" 
         links { "nmopengl-x86.lib","hal-virtual-x86.lib"}
         
solution "demo3d-target1-x86"
   configurations { "Debug", "Release" }
   --platforms{"x64","x86"}

   -- A project defines one build target
   project "demo3d-target1-x86"
      kind "ConsoleApp"
     language "C++"
   -- "../../../src_proc1/pc/*.*","../../../src_proc1/common/*.*",
      files { "../../../include/*.h","../src_target1/*.*", "../../../nmglvs_mc12101-gcc/src_nmc1/*.*","../../../src_proc1/pc/*.*","../../../src_proc1/common/*.*"}
	  libdirs { "$(NMPP)/lib","$(HAL)/lib", "$(VSHELL)/lib", "../../../lib"}
	  includedirs { "$(MC12101)/include","$(HAL)/include","$(NMPP)/include","../../../include", "$(VSHELL)/include"}
     links { "nmpp-x86.lib", "vshell.lib"}
     defines { "TEST_NMGL_TEX_FUNC" }
      configuration "Debug"
         defines { "DEBUG" }
         symbols  "On" 
         links { "nmopengl-x86d.lib","hal-virtual-x86d.lib"}

      configuration "Release"
         defines { "NDEBUG" }
         symbols  "Off" 	   
         links { "nmopengl-x86.lib","hal-virtual-x86.lib"}
  
       