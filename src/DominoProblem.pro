TARGET = vador

TEMPLATE = app
CONFIG += console
CONFIG += qt
QT += core gui
QT += xml


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
    binary_hash.h \
    toolkit/program_info.h \
    toolkit/program_args.h \
    toolkit/program_timer.h \
    half_elem.h \
    domino_elem_fwd.h \
    domino_elem.h \
    domino_elem_located_fwd.h \
    domino_elem_located.h \
    domino_problem.h \
    domino_problem_input.h \
    mainwindow.h \
    domino_problem_r.h \
    approximate_r.h \
    summary_window.h \
    domino_problem_solver.h \
    accurate_thread.h

SOURCES += \
    main.cpp \
    binary_hash.cpp \
    toolkit/program_info.cpp \
    toolkit/program_args.cpp \
    toolkit/program_timer.cpp \
    half_elem.cpp \
    domino_elem.cpp \
    domino_elem_located.cpp \
    domino_problem.cpp \
    domino_problem_input.cpp \
    mainwindow.cpp \
    domino_problem_r.cpp \
    approximate_r.cpp \
    summary_window.cpp \
    domino_problem_solver.cpp \
    accurate_thread.cpp

FORMS += \
    mainwindow.ui \
    summary_window.ui
