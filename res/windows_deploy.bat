REM VaDoR - VAnishing DOmino pRroblem

REM this file is configured for specific build directories
REM it might not work in every case

REM debug
call COPY /Y /B ..\build_qt481_msvc2010_debug\debug\vador.exe ..\bin\windows\VanishingDominoProblem_debug\vador.exe

REM release
call COPY /Y /B ..\build_qt481_msvc2010_release\release\vador.exe ..\bin\windows\VanishingDominoProblem\vador.exe

exit /B 0
