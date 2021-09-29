#!lua

-- A solution contains projects, and defines the available configurations
solution "demo3d-mc12101"
   configurations { "Debug", "Release" }
   startproject "mc12101-host"
   platforms { "x64" }

   -- A project defines one build target
   project "mc12101-host"
		kind "ConsoleApp"
		language "C++"
		files { "../../../include/*.h","../src_host/*.cpp","../src_host/*.c", "../../../nmglvs_mc12101-remote/src_host/*.cpp"}
		libdirs { "$(MC12101)/lib","$(VSHELL)/lib","$(NMPP)/lib","$(HAL)/lib"}
		includedirs { "$(MC12101)/include","$(HAL)/include","$(VSHELL)/include","$(NMPP)/include","../../../include"}
		links { "mc12101load.lib","vshell.lib"}
		configuration {"Debug","x86"}
			links { "nmpp-x86d.lib","hal-mc12101-x86d.lib"}
			targetsuffix ("-x86")
			architecture "x32"
			defines { "DEBUG"}
			symbols  "On" 
		configuration {"Release","x86"}
			links { "nmpp-x86.lib","hal-mc12101-x86.lib"}
			targetsuffix ("-x86")
			architecture "x32"
			defines { "NDEBUG"}
			symbols  "Off" 
		configuration {"Debug","x64"}
			links { "nmpp-x64d.lib","hal-mc12101-x64d.lib"}
			targetsuffix ("-x64")
			architecture "x64"
			defines { "DEBUG"}
			symbols  "On" 
		configuration {"Release","x64"}
			links { "nmpp-x64.lib","hal-mc12101-x64.lib"}
			targetsuffix ("-x64")
			architecture "x64"
			defines { "NDEBUG"}
			symbols  "Off" 
	 
	project "mc12101-nmc4-0"
      kind "Makefile"
      files { "../../../include/*.h","../src_target0/*.*", "mc12101brd-nmc0.cfg", "Makefile0" }
	  includedirs { "$(MC12101)/include","$(HAL)/include","$(NMPP)/include","../../include"}
	  defines { "__GNUC__"}
	 
	  configuration "Debug"
		   defines { "DEBUG"}
		   buildcommands {"make -f Makefile0 DEBUG=y"}
		   rebuildcommands {"make -B -f Makefile0 DEBUG=y"}
		   cleancommands {"make clean"}
		   
	  configuration "Release"
		   defines { "NDEBUG"}
		   buildcommands {"make -f Makefile0"}
		   rebuildcommands {"make -B -f Makefile0"}
		   cleancommands {"make clean"}		   
		   
	project "mc12101-nmc4-1"
      kind "Makefile"
      files { "../../../include/*.h","../src_target1/*.*", "mc12101brd-nmc1.cfg", "Makefile1" }
	  includedirs { "$(MC12101)/include","$(HAL)/include","$(NMPP)/include","../../../include"}
	  defines { "__GNUC__"}
	 
	  configuration "Debug"
		   defines { "DEBUG"}
		   buildcommands {"make -f Makefile1 DEBUG=y"}
		   rebuildcommands {"make -B -f Makefile1 DEBUG=y"}
		   cleancommands {"make clean"}
		   
	  configuration "Release"
		   defines { "NDEBUG"}
		   buildcommands {"make -f Makefile1"}
		   rebuildcommands {"make -B -f Makefile1"}
		   cleancommands {"make clean"}		   
		   
		  