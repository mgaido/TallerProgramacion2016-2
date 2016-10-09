@echo off

set PATH=%PATH%;%programfiles(x86)%\Microsoft Visual Studio 14.0\VC\bin

set INCLUDE=..\SDL2-2.0.4\include;%programfiles(x86)%\Microsoft Visual Studio 14.0\VC\include;%programfiles(x86)%\Microsoft Visual Studio 14.0\VC\atlmfc\include;%programfiles(x86)%\Windows Kits\10\Include\10.0.10240.0\ucrt;%programfiles(x86)%\Windows Kits\10\Include\10.0.10150.0\ucrt;%programfiles(x86)%\Windows Kits\8.1\Include\um;%programfiles(x86)%\Windows Kits\8.1\Include\shared

set LIB=..\SDL2-2.0.4\lib\x86;%programfiles(x86)%\Microsoft Visual Studio 14.0\VC\lib;%programfiles(x86)%\Microsoft Visual Studio 14.0\VC\atlmfc\lib;%programfiles(x86)%\Windows Kits\10\lib\10.0.10240.0\ucrt\x86;%programfiles(x86)%\Windows Kits\10\Lib\10.0.10150.0\ucrt\x86;%programfiles(x86)%\Windows Kits\8.1\lib\winv6.3\um\x86;%programfiles(x86)%\Windows Kits\NETFXSDK\4.6.1\Lib\um\x86

md obj

cl /O2 /EHs /MD /MP4 /Fo.\obj\ src\*.cpp /link /out:tp2.exe ws2_32.lib SDL2.lib

rmdir /S /Q obj

pause