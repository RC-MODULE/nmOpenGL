#!lua

-- A solution contains projects, and defines the available configurations
solution "demo3d-mc12101"
   configurations { "Debug", "Release" }
   startproject "mc12101-host"
   platforms{"x86","x64"}

   -- A project defines one build target
   project "mc12101-host"
		kind "ConsoleApp"
		language "C++"
		files { "../../include/*.h","../src_host/*.cpp","../src_host/*.c", "../../nmglvs_mc12101-gcc/src_host/*.cpp"}
		libdirs { "$(MC12101)/lib","$(VSHELL)/lib","$(NMPP)/lib","$(HAL)/lib"}
		includedirs { "$(MC12101)/include","$(HAL)/include","$(VSHELL)/include","$(NMPP)/include","../../include", "../include"}
		configuration {"Debug","x86"}
			links { "mc12101load.lib","vshell.lib","nmpp-x86d.lib","hal-mc12101-x86d.lib","mc12101-nmc4-0","mc12101-nmc4-1"}
			targetsuffix ("-x86d")
			architecture "x32"
			defines { "DEBUG"}
			symbols  "On" 
		configuration {"Release","x86"}
			links { "mc12101load.lib","vshell.lib","nmpp-x86.lib","hal-mc12101-x86.lib","mc12101-nmc4-0","mc12101-nmc4-1"}
			targetsuffix ("-x86")
			architecture "x32"
			defines { "NDEBUG"}
			symbols  "Off" 
		configuration {"Debug","x64"}
			links { "mc12101load.lib","vshell.lib","nmpp-x64d.lib","hal-mc12101-x64d.lib","mc12101-nmc4-0","mc12101-nmc4-1"}
			targetsuffix ("-x64d")
			architecture "x64"
			defines { "DEBUG"}
			symbols  "On" 
		configuration {"Release","x64"}
			links { "mc12101load.lib","vshell.lib","nmpp-x64.lib","hal-mc12101-x64.lib","mc12101-nmc4-0","mc12101-nmc4-1"}
			targetsuffix ("-x64")
			architecture "x64"
			defines { "NDEBUG"}
			symbols  "Off" 
	 
	project "mc12101-nmc4-0"
      kind "Makefile"
      files { "../../include/*.h","../src_target0/*.*", "../../nmglvs_mc12101-gcc/src_nmc0/*.*", "mc12101brd-nmc0.cfg", "Makefile0" }
	  includedirs { "$(MC12101)/include","$(HAL)/include","$(NMPP)/include","../../include","../include"}
	 
	  configuration "Debug"
		   buildcommands {"make DEBUG=y -f Makefile0"}
		   rebuildcommands {"make -B DEBUG=y -f Makefile0"}
		   cleancommands {"make clean"}
		   
	  configuration "Release"
		   buildcommands {"make -f Makefile0"}
		   rebuildcommands {"make -B -f Makefile0"}
		   cleancommands {"make clean"}		   
		   
	project "mc12101-nmc4-1"
      kind "Makefile"
      files { "../../include/*.h","../src_target1/*.*", "../../nmglvs_mc12101-gcc/src_nmc1/*.*","mc12101brd-nmc1.cfg", "Makefile1" }
	  includedirs { "$(MC12101)/include","$(HAL)/include","$(NMPP)/include","../../include"}
	 
	  configuration "Debug"
		   buildcommands {"make DEBUG=y -f Makefile1"}
		   rebuildcommands {"make -B DEBUG=y -f Makefile1"}
		   cleancommands {"make clean"}
		   
	  configuration "Release"
		   buildcommands {"make -f Makefile1"}
		   rebuildcommands {"make -B -f Makefile1"}
		   cleancommands {"make clean"}		   
		   
		  