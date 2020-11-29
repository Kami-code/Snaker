QT += widgets
QT+= multimedia

HEADERS       = \
    DesktopWindow.h \
    GameOverWindow.h \
    GameWindow.h \
    LinkList.h \
    SettingWindow.h \
    game.h
SOURCES       = \
                DesktopWindow.cpp \
                GameOverWindow.cpp \
                GameWindow.cpp \
                LinkList.cpp \
                SettingWindow.cpp \
                game.cpp \
                main.cpp

QMAKE_PROJECT_NAME = widgets_analogclock

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/widgets/analogclock
INSTALLS += target

DISTFILES +=

RESOURCES += \
    source.qrc
