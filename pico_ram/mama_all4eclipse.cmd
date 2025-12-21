call ../../SetEnv.bat
call mama_clear.cmd

cmake -S src -B build -G "Eclipse CDT4 - Unix Makefiles" -D CMAKE_BUILD_TYPE=Debug
cd build
make

pause


