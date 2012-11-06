REM VaDoR - VAnishing DOmino pRroblem
REM resources copying

IF "%1"=="" GOTO Invalid

:Loop

IF "%1"=="" GOTO Continue

call rmdir %1\pictures /s /q
call mkdir %1\pictures
call xcopy ..\src\pictures %1\pictures /v /f /s /y
call xcopy problems %1 /v /f /s /y

SHIFT

GOTO Loop

:Continue

exit /B 0

:Invalid

REM to copy resources to another directory, provide the path in the first argument
REM working dir of this script must be identical to its location

exit /B 1
