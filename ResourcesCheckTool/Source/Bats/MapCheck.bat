cd %~dp0
set ProjectRoot=%~dp0../../../../../
set EngineRoot=%~dp0../../../../../../UE4181/

set EnginePath=%EngineRoot%Engine/Binaries/Win64/UE4Editor.exe
set Projectpath=%ProjectRoot%ShadowTrackerExtra.uproject

@REM cd %ProjectRoot%
@REM dir

call %EnginePath% %Projectpath% -Run=MapCheck -stdout