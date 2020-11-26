#!lua
solution "nmopengl-nmc4f"
   configurations { "Debug", "Release" }

	project "nmopengl-nmc4f"
      kind "Makefile"
	  
      files { "../../include/*.h", "Makefile" , "../../src_proc0/nm/*.asm", "../../src_proc0/nmgl/*.cpp",  "../../src_proc0/common/*.cpp" }
	 
	  configuration "Debug"
		   buildcommands {"make DEBUG=y -f Makefile -j4"}
		   rebuildcommands {"make -B DEBUG=y -f Makefile -j4"}
		   cleancommands {"make clean"}
		   
	  configuration "Release"
		   buildcommands {"make -f Makefile -j4"}
		   rebuildcommands {"make -B -f Makefile -j4"}
		   cleancommands {"make clean"}		   
		
		
		