#!lua

-- A solution contains projects, and defines the available configurations
solution "nmopengl-x86"
   configurations { "Debug", "Release" }
   platforms{"x64", "x86"}

	project "nmopengl"
      kind "StaticLib"
      files { 	"../src_proc0/common/*.*",
				"../src_proc0/pc/*.*",
				"../src_proc0/nmgl/*.*",
				"../src_proc1/common/*.*",
				"../src_proc1/pc/*.*",
				"../utilities/common/*.c*",
				"../utilities/common/pc/*.c*",
				"../utilities/fixed/*.c*",
				"../utilities/fixed/pc/*.c*",
				"../utilities/float/*.c*",
				"../utilities/float/pc/*.c*",}
	  includedirs { "../include", "$(NMPP)/include", "$(HAL)/include", "../utilities/include"}
	  targetdir ("../lib")
	  
	  configuration {"Debug","x86"}
			targetsuffix ("-x86d")
			architecture "x32"
			defines { "DEBUG", "EMULATION"}
			symbols  "On" 
		configuration {"Release","x86"}
			targetsuffix ("-x86")
			architecture "x32"
			defines { "NDEBUG", "EMULATION"}
			symbols  "Off" 
		configuration {"Debug","x64"}
			targetsuffix ("-x64d")
			architecture "x64"
			defines { "DEBUG", "EMULATION"}
			symbols  "On" 
		configuration {"Release","x64"}
			targetsuffix ("-x64")
			architecture "x64"
			defines { "NDEBUG", "EMULATION"}
			symbols  "Off" 
		
	 