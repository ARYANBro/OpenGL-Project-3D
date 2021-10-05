@echo off
pushd %~dp0\..\

set Defines=-DCMAKE_BUILD_TYPE=Release -DASSIMP_BUILD_ASSIMP_TOOLS=OFF -DASSIMP_BUILD_TESTS=OFF -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF

call cmake %Defines% CMakeLists.txt -G"MinGW Makefiles" -S . -B Bin
popd
