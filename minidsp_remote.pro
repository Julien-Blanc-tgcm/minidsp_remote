# NOTICE:
#
# Application name defined in TARGET has a corresponding QML filename.
# If name defined in TARGET is changed, the following needs to be done
# to match new name:
#   - corresponding QML filename must be changed
#   - desktop icon filename must be changed
#   - desktop filename must be changed
#   - icon definition filename in desktop file must be changed
#   - translation filenames have to be changed

# The name of your application
TARGET = harbour-minidsp_remote

CONFIG += sailfishapp c++11

QMAKE_CXXFLAGS += -Werror

SOURCES += src/minidsp_remote.cpp \
    minidsplib/query.cpp \
    minidsplib/reply.cpp \
    src/nanoavrbasicsettings.cpp \
    src/nanoavrclient.cpp

DISTFILES += \
    harbour-minidsp_remote.desktop \
    qml/cover/CoverPage.qml \
    qml/harbour-minidsp_remote.qml \
    qml/pages/Client.qml \
    qml/pages/Settings.qml \
    rpm/harbour-minidsp_remote.changes.in \
    rpm/harbour-minidsp_remote.changes.run.in \
    rpm/harbour-minidsp_remote.yaml \
    translations/*.ts

SAILFISHAPP_ICONS = 86x86 108x108 128x128 172x172

# to disable building translations every time, comment out the
# following CONFIG line
CONFIG += sailfishapp_i18n

TRANSLATIONS += translations/harbour-minidsp_remote-de.ts \
    translations/harbour-minidsp_remote-fr.ts

INCLUDEPATH += minidsplib

HEADERS += \
    minidsplib/constants.h \
    minidsplib/query.h \
    minidsplib/reply.h \
    src/nanoavrbasicsettings.h \
    src/nanoavrclient.h
