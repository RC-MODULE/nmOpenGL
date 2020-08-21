#!lua

-- A solution contains projects, and defines the available configurations
solution "patterns"
   configurations { "Debug", "Release" }
   platforms{"x64", "x86"}
   -- A project defines one build target
   
   project "patterns"
		kind "ConsoleApp"
		language "C++"
		files { "../../include/pattern.h", "./*cpp", "../../nmglvs_mc12101-gcc/src_host/*.cpp"}
		libdirs { "$(VSHELL)/lib","$(NMPP)/lib", "$(HAL)/lib"}
		includedirs { "$(MC12101)/include","$(HAL)/include", "$(VSHELL)/include","$(NMPP)/include", "../../include"}
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
		 
		  