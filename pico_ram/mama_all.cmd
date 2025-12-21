call ../../SetEnv.bat



rem echo ------------------------------------
rem set
rem echo ------------------------------------
rem pause

call mama_clear.cmd

rem cmake /?
rem pause


rem cmake -S src -B . -G "Ninja" -D CMAKE_BUILD_TYPE=Debug
rem ninja

cmake -S src -B build -G "Unix Makefiles" -D CMAKE_BUILD_TYPE=Debug
rem make --makefile=build_pico\Makefile
cd build
make

pause


