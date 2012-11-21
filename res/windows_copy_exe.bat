REM VaDoR - VAnishing DOmino pRroblem
REM copies exe from dir1 to dir2

IF "%1"=="" GOTO Invalid

:Loop

IF "%1"=="" GOTO Continue

IF "%2"=="" GOTO Invalid

call COPY /Y /B %1\vador.exe %2\vador.exe

SHIFT
SHIFT

GOTO Loop

:Continue

exit /B 0

:Invalid

REM to copy exe from one to another directory, provide the paths in two arguments
REM working dir of this script must be identical to its location

exit /B 1
