ifndef OS_RM
 	OS_RM    =rm -f -r
  	OS_RD    =rm -d -r -f
endif
kill: clean
	-$(OS_RM) *.vcproj *.sln *.vcxproj *.db output output.264 

clean:
	-$(OS_RM) *.abs *.elf *.ncb *.map *.elf *.suo *.user *.filters *.db .* *.opendb  $(TARGET) $(TARGET_X86) *.dump
	-$(OS_RD) Debug Release bin obj obj0 obj1 .vs backup
