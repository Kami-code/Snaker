QT += widgets
QT+= multimedia

HEADERS       = \
    DesktopWindow.h \
    Game.h \
    GameOverWindow.h \
    GameWindow.h \
    LinkList.h \
    Resource.h \
    SettingWindow.h
SOURCES       = \
                DesktopWindow.cpp \
                Game.cpp \
                GameOverWindow.cpp \
                GameWindow.cpp \
                LinkList.cpp \
                Resource.cpp \
                SettingWindow.cpp \
                main.cpp

QMAKE_PROJECT_NAME = widgets_analogclock

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/widgets/analogclock
INSTALLS += target

DISTFILES +=

RESOURCES += \
    source.qrc
