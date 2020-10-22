To build without errors:

mc12101. Replace in make_mc12101_nmpu0_gcc/Makefile to:

LIB_DIRS         = -L "$(NMC_GCC_TOOLPATH)/nmc4-ide/lib" -L"$(NMPP)/lib" -L"$(HAL)/lib" -L"../../../../lib"
LIBS             = -lnmpp-nmc4f -lhal-mc12101 -lnmopengl-nmc4f

x86. Replace in make_x86/Makefile to:

links { "nmpp-x86.lib", "hal-virtual-x86.lib", "tinyxml.lib","nmopengl-x86" }
libdirs { "$(NMPP)/lib", "$(HAL)/lib", "$(TINYXML)", "../../../../lib/"}
	

