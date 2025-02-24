@echo off
setlocal enabledelayedexpansion

set "files="
for /r %%f in (*.c) do (
    set "files=!files! %%f"
)

set flags="/W4"

cl /Feskeel.exe %flags% %files% && del *.obj
