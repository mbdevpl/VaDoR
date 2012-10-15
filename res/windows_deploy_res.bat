REM VaDoR - VAnishing DOmino pRroblem

REM copies all currently defined resources to /bin

REM debug
call rmdir ..\bin\windows\VanishingDominoProblem_debug\pictures /s /q
call mkdir ..\bin\windows\VanishingDominoProblem_debug\pictures
call xcopy ..\src\pictures ..\bin\windows\VanishingDominoProblem_debug\pictures /v /f /s /y
call COPY /Y /B problem1.xml ..\bin\windows\VanishingDominoProblem_debug\problem1.xml

REM release
call rmdir ..\bin\windows\VanishingDominoProblem\pictures /s /q
call mkdir ..\bin\windows\VanishingDominoProblem\pictures
call xcopy ..\src\pictures ..\bin\windows\VanishingDominoProblem\pictures /v /f /s /y
call COPY /Y /B problem1.xml ..\bin\windows\VanishingDominoProblem\problem1.xml

exit /B 0
