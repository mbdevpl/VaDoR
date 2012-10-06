
TARGET = vador

TEMPLATE = app
CONFIG += console
CONFIG -= qt

# platform specific defines
unix:DEFINES += LINUX
win32:DEFINES += WINDOWS

# debug/release defines
CONFIG(debug, debug|release): DEFINES += DEBUG
else:CONFIG(release, debug|release): DEFINES += RELEASE

# INCLUDEPATH += D:/Projects/Cpp/QtTools/src
# INCLUDEPATH += D:/Projects/Cpp/Libraries/Includes/rapidxml

HEADERS += \
    simple_list.h \
    rapidxml.hpp \
    half_elem.h \
    domino_elem_fwd.h \
    domino_elem.h \
    domino_elem_located.h \
    domino_problem.h \

SOURCES += \
    main.cpp \
    half_elem.cpp \
    domino_elem.cpp \
    domino_elem_located.cpp \
    domino_problem.cpp \
