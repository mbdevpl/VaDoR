REM VaDoR - VAnishing DOmino pRroblem

REM this just copies resources to build dirs, for testing

REM this file is configured for specific build directories
REM it might not work in every case

REM debug
call rmdir ..\build_qt481_msvc2010_debug\pictures /s /q
call mkdir ..\build_qt481_msvc2010_debug\pictures
call xcopy ..\src\pictures ..\build_qt481_msvc2010_debug\pictures /v /f /s /y
call COPY /Y /B problem1.xml ..\build_qt481_msvc2010_debug\problem1.xml
call COPY /Y /B problem2.txt ..\build_qt481_msvc2010_debug\problem2.txt
call COPY /Y /B problem3.xml ..\build_qt481_msvc2010_debug\problem3.xml

REM release
call rmdir ..\build_qt481_msvc2010_release\pictures /s /q
call mkdir ..\build_qt481_msvc2010_release\pictures
call xcopy ..\src\pictures ..\build_qt481_msvc2010_release\pictures /v /f /s /y
call COPY /Y /B problem1.xml ..\build_qt481_msvc2010_release\problem1.xml
call COPY /Y /B problem2.txt ..\build_qt481_msvc2010_release\problem2.txt
call COPY /Y /B problem3.xml ..\build_qt481_msvc2010_release\problem3.xml

exit /B 0
