#!lua

-- A solution contains projects, and defines the available configurations
  solution "test-x86"
	configurations { "Debug", "Release" }
	project "test-x86"
      kind "ConsoleApp"
      language "C++"
      files { "../*.cpp", }
	  links { "nmopengl-x86d.lib", "nmpp-x86.lib", "hal-virtual-x86.lib", "tinyxml.lib" }
	  libdirs { "$(NMPP)/lib", "$(HAL)/lib", "../../../../lib", "$(TINYXML)"}
	  includedirs { "../../../../include", "$(NMPP)/include", "$(HAL)/include", "$(TINYXML)"}

      configuration "Debug"
         defines { "DEBUG" }
         symbols  "On"



      configuration "Release"
         defines { "NDEBUG" }
         symbols  "Off"
