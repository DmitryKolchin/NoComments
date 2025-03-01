:: VisualStudio
RMDIR /s /q .vs
RMDIR /s /q .idea
DEL /s /q *.sln
DEL /s /q *.vsconfig
DEL /s /q *.user

:: Unreal Project
RMDIR /s /q Binaries
RMDIR /s /q DerivedDataCache
RMDIR /s /q Intermediate
RMDIR /s /q Script


:: Saving folders Saved\Config\WindowsEditor in order to keep the editor settings intact.
IF EXIST Saved\Config\WindowsEditor\ (
    XCOPY /E /I /Y Saved\Config\WindowsEditor Temp\WindowsEditor
)

RMDIR /s /q Saved

IF EXIST Temp\WindowsEditor\ (
    XCOPY /E /I /Y Temp\WindowsEditor Saved\Config\WindowsEditor
)

RMDIR /s /q Temp



:: Removing temporary files from plugins.
for /d %%d in (Plugins\*) do (
    if exist "%%d\Binaries" (
        rmdir /s /q "%%d\Binaries"
    )
    
    if exist "%%d\Intermediate" (
        rmdir /s /q "%%d\Intermediate"
    )
)