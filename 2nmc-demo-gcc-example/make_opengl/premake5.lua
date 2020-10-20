#!lua


solution "opengl"
   configurations { "Debug", "Release" }
   --platforms{"x64","x86"}

   -- A project defines one build target
   project "opengl"
      kind "ConsoleApp"
	  language "C++"
      files { "../../nmglvs_opengl/*.*", "../src_target0/*.cpp" }
	  libdirs { "$(NMPP)/lib","$(HAL)/lib", "$(GLFW)/build/src/Debug", "$(GLEW)/lib/Release/Win32"}
	  includedirs { "$(MC12101)/include","$(HAL)/include","$(NMPP)/include","../../nmglvs_opengl", "$(GLFW)/include","$(GLEW)/include"}
	  links { "nmpp-x86.lib","hal-virtual-x86.lib", "glfw3.lib", "glew32s.lib", "opengl32.lib"}
	  
	 
      configuration "Debug"
         defines { "DEBUG" }
         symbols  "On" 

      configuration "Release"
         defines { "NDEBUG" }
         symbols  "Off" 
