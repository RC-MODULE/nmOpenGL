#!lua

-- A solution contains projects, and defines the available configurations
  solution "test-x86"
	configurations { "Debug", "Release" }
	project "test-x86"
      kind "ConsoleApp"
      language "C++"
      files { "../*.cpp", "../../../../src_proc0/pc/*.cpp","../../../../src_proc0/pc/*.c","../../../../src_proc1/pc/*.cpp", "../../../../src_proc1/pc/*.c" }
	  links { "nmopengl-x86.lib", "nmpp-x86.lib", "hal-virtual-x86.lib", "tinyxml.lib" }
	  libdirs { "$(NMPP)/lib", "$(HAL)/lib", "$(TINYXML)", "../../../../lib"}
	  includedirs { "../../../../include", "$(NMPP)/include", "$(HAL)/include", "$(TINYXML)", "../../../../utilities/include", "../../../../context/include"}

      configuration "Debug"
         defines { "DEBUG" }
         symbols  "On"



      configuration "Release"
         defines { "NDEBUG" }
         symbols  "Off"
