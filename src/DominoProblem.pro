
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

#unix:QMAKE_CXXFLAGS += -save-temps
#win32:QMAKE_CXXFLAGS += -P

# INCLUDEPATH += D:/Projects/Cpp/QtTools/src
# INCLUDEPATH += D:/Projects/Cpp/Libraries/Includes/rapidxml

HEADERS += \
    rapidxml.hpp \
    simple_container.h \
    simple_list.h \
    simple_tree.h \
    half_elem.h \
    domino_elem_fwd.h \
    domino_elem.h \
    domino_elem_located_fwd.h \
    domino_elem_located.h \
    domino_problem.h \

SOURCES += \
    main.cpp \
    half_elem.cpp \
    domino_elem.cpp \
    domino_elem_located.cpp \
    domino_problem.cpp \
