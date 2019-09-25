#!lua

-- A solution contains projects, and defines the available configurations
solution "triangulisation"
   configurations { "Debug", "Release" }

   -- A project defines one build target
   project "triangulisation"
      kind "ConsoleApp"
      language "C++"
      files {"*.cpp", "*.h"}
	  includedirs {"."}
  
      configuration "Debug"
         defines { "DEBUG" }
         symbols  "On" 
		 

      configuration "Release"
         defines { "NDEBUG" }
         symbols  "Off" 
	
		 
		 

		   