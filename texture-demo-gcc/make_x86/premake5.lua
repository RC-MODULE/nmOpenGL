#!lua

-- A solution contains projects, and defines the available configurations
solution "demo3d-host"
   configurations { "Debug", "Release" }
   platforms{"x64", "x86"}
   -- A project defines one build target
   
   project "demo3d-host"
		kind "ConsoleApp"
		language "C++"
		files { "../../include/*.h","../src_host/*.cpp","../src_host/*.c", "../../nmglvs_mc12101-gcc/src_host/*.cpp"}
		libdirs { "$(VSHELL)/lib","$(NMPP)/lib","$(HAL)/lib"}
		includedirs { "$(MC12101)/include","$(HAL)/include","$(VSHELL)/include","$(NMPP)/include","../../include"}
	    flags { "MultiProcessorCompile" }
        configuration {"Debug","x86"}
			links { "vshell.lib", "nmpp-x86d.lib","hal-virtual-x86d.lib"}
			targetsuffix ("-x86d")
			architecture "x32"
			defines { "DEBUG", "EMULATION"}
			symbols  "On" 
		configuration {"Release","x86"}
			links { "vshell.lib", "nmpp-x86.lib","hal-virtual-x86.lib"}
			targetsuffix ("-x86")
			architecture "x32"
			defines { "NDEBUG", "EMULATION"}
			symbols  "Off" 
		configuration {"Debug","x64"}
			links { "vshell.lib", "nmpp-x64d.lib","hal-virtual-x64d.lib"}
			targetsuffix ("-x64d")
			architecture "x64"
			defines { "DEBUG", "EMULATION"}
			symbols  "On" 
		configuration {"Release","x64"}
			links { "vshell.lib", "nmpp-x64.lib","hal-virtual-x64.lib"}
			targetsuffix ("-x64")
			architecture "x64"
			defines { "NDEBUG", "EMULATION"}
			symbols  "Off" 
		 
		 
		 
solution "demo3d-target0"
   configurations { "Debug", "Release" }
   platforms{"x64","x86"}
   defines {"TEXTURE_ENABLED"}

   -- A project defines one build target
   project "demo3d-target0"
      kind "ConsoleApp"
	  language "C++"
      files { "../../include/*.h","../src_target0/*.cpp", "../../nmglvs_mc12101-gcc/src_nmc0/*.*",
	  "../../src_proc0/pc/*.*", "../../src_proc0/common/*.*",  "../../src_proc0/nmgl/*.*"}
	  libdirs { "$(NMPP)/lib","$(HAL)/lib", "../../lib"}
	  includedirs { "$(MC12101)/include","$(HAL)/include","$(NMPP)/include","../../include"}
	  flags { "MultiProcessorCompile" }
	  configuration {"Debug","x86"}
			links { "nmpp-x86d.lib","hal-virtual-x86d.lib"}
			targetsuffix ("-x86d")
			architecture "x32"
			defines { "DEBUG", "EMULATION"}
			symbols  "On" 
	  configuration {"Release","x86"}
			links { "nmpp-x86.lib","hal-virtual-x86.lib"}
			targetsuffix ("-x86")
			architecture "x32"
			defines { "NDEBUG", "EMULATION"}
			symbols  "Off" 
	  configuration {"Debug","x64"}
			links { "nmpp-x64d.lib","hal-virtual-x64d.lib"}
			targetsuffix ("-x64d")
			architecture "x64"
			defines { "DEBUG"}
			symbols  "On" 
	  configuration {"Release","x64"}
			links { "nmpp-x64.lib","hal-virtual-x64.lib"}
			targetsuffix ("-x64")
			architecture "x64"
			defines { "NDEBUG", "EMULATION"}
			symbols  "Off" 
		 
solution "demo3d-target1"
   configurations { "Debug", "Release" }
   platforms{"x64","x86"}
   defines {"TEXTURE_ENABLED"}

   -- A project defines one build target
   project "demo3d-target1"
      kind "ConsoleApp"
	  language "C++"
      files { "../../include/*.h","../src_target1/*.cpp", "../../nmglvs_mc12101-gcc/src_nmc1/*.*",
	  "../../src_proc1/pc/*.*", "../../src_proc1/common/*.*" }
	  libdirs { "$(NMPP)/lib","$(HAL)/lib", "../../lib"}
	  includedirs { "$(MC12101)/include","$(HAL)/include","$(NMPP)/include","../../include"}
	  flags { "MultiProcessorCompile" }
	  configuration {"Debug","x86"}
			links { "nmpp-x86d.lib","hal-virtual-x86d.lib"}
			targetsuffix ("-x86d")
			architecture "x32"
			defines { "DEBUG", "EMULATION"}
			symbols  "On" 
	  configuration {"Release","x86"}
			links { "nmpp-x86.lib","hal-virtual-x86.lib"}
			targetsuffix ("-x86")
			architecture "x32"
			defines { "NDEBUG", "EMULATION"}
			symbols  "Off" 
	  configuration {"Debug","x64"}
			links { "nmpp-x64d.lib","hal-virtual-x64d.lib"}
			targetsuffix ("-x64d")
			architecture "x64"
			defines { "DEBUG"}
			symbols  "On" 
	  configuration {"Release","x64"}
			links { "nmpp-x64.lib","hal-virtual-x64.lib"}
			targetsuffix ("-x64")
			architecture "x64"
			defines { "NDEBUG", "EMULATION"}
			symbols  "Off" 
		   
		  
