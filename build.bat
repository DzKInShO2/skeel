@echo off
setlocal enabledelayedexpansion

set "files="
for /r %%f in (src\*.c) do (
    set "files=!files! %%f"
)

cl /W4 /Feskeel.exe %files% User32.lib && del *.obj
