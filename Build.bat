@ECHO OFF
if not exist Build mkdir Build

pushd  Build
set PROGRAM=Render
Rem set COMPILE_FLAGS=-MD -Zi
set COMPILE_FLAGS=-MD
set INCLUDE_FLAGS=
set LINK_FLAGS=-LIBPATH:..\External\lib  glfw3.lib user32.lib gdi32.lib shell32.lib -SUBSYSTEM:CONSOLE

cl %COMPILE_FLAGS% -Fe%PROGRAM% ..\Code\*.cpp  ..\Code\glad\gl.c ..\Code\imgui\*.cpp  -link%LINK_FLAGS%

popd