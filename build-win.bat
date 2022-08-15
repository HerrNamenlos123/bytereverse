@echo off

mkdir build 2>NUL
pushd build

cmake .. -A Win32
cmake --build . --config MinSizeRel

popd

Pause
