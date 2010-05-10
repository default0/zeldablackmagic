INCLUDEPATH += ../SFML/include

TEMPLATE += app

HEADERS += src/QSFMLCanvas.hpp \
    src/preferences.h \
    src/mainwindow.h \
    src/canvas.h
SOURCES += src/QSFMLCanvas.cpp \
    src/preferences.cpp \
    src/mainwindow.cpp \
    src/main.cpp \
    src/canvas.cpp

LIBS += ../SFML/lib/mingw/libsfml-system-d.a \
    ../SFML/lib/mingw/libsfml-window-d.a \
    ../SFML/lib/mingw/libsfml-graphics-d.a
