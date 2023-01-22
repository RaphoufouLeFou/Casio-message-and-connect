@echo off
rem Do not edit! This batch file is created by CASIO fx-9860G SDK.


if exist TEST.G1A  del TEST.G1A

cd debug
if exist FXADDINror.bin  del FXADDINror.bin
"D:\CASIO\casio\OS\SH\Bin\Hmake.exe" Addin.mak
cd ..
if not exist debug\FXADDINror.bin  goto error

"D:\CASIO\casio\Tools\MakeAddinHeader363.exe" "D:\DOCUMENTS\Raphael\casio\code\SDK\projects\ConnectFX"
if not exist TEST.G1A  goto error
echo Build has completed.
goto end

:error
echo Build was not successful.

:end

