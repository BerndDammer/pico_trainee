call ../../SetEnv.bat

rmdir /S /Q CMakeFiles
rmdir _deps /S /Q
rmdir generated /S /Q
rmdir pico-sdk /S /Q
rmdir pioasm /S /Q
rmdir picotool /S /Q
rmdir pioasm-install /S /Q
rmdir ninja /S /Q
rmdir build /S /Q


del CMakeCache.txt
del .ninja*.* /S /Q
del build.ninja /S /Q
del *.elf /S /Q
del cmake_install.cmake /S /Q
del *.ld /S /Q

del *.dis /S /Q
del *.map /S /Q
del *.hex /S /Q
del *.uf2 /S /Q
del *.bin /S /Q
del makefile


