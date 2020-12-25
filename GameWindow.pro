QT += widgets
QT+= multimedia

HEADERS       = \
    header/DesktopWindow.h \
    header/Game.h \
    header/GameOverWindow.h \
    header/GameWindow.h \
    header/LinkList.h \
    header/Resource.h \
    header/Saver.h \
    header/SaverWindow.h \
    header/SettingWindow.h
SOURCES       = \
                src/Algorithm.cpp \
                src/DesktopWindow.cpp \
                src/Game.cpp \
                src/GameOverWindow.cpp \
                src/GameWindow.cpp \
                src/LinkList.cpp \
                src/Resource.cpp \
                src/Saver.cpp \
                src/SaverWindow.cpp \
                src/SettingWindow.cpp \
                main.cpp

QMAKE_PROJECT_NAME = Snaker

DISTFILES += \
    README.md

RESOURCES += \
    source.qrc
