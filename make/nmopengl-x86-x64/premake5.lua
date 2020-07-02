#!lua

-- A solution contains projects, and defines the available configurations
solution "nmopengl-x86"
   configurations { "Debug", "Release" }

	project "nmopengl-x86"
      kind "StaticLib"
      files { 	"../../src_proc0/common/*.*",
				"../../src_proc0/pc/*.*",
				"../../src_proc0/nmgl/*.*",
				"../../src_proc1/common/*.*",
				"../../src_proc1/pc/*.*",}
	  includedirs { "../../include", "$(NMPP)/include", "$(HAL)/include"}
	  targetdir ("../../lib")
	  
	  configuration "Debug"
		targetsuffix ("d")
        defines { "DEBUG"}
        symbols  "On" 
		 
      configuration "Release"
         defines { "NDEBUG"}
         symbols  "Off" 
		
	 