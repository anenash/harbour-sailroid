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
TARGET = harbour-sailroid

CONFIG += sailfishapp

SOURCES += src/harbour-sailroid.cpp \
    src/ball.cpp \
    src/gameboard.cpp \
    src/target.cpp \
    src/trampoline.cpp

DISTFILES += qml/harbour-sailroid.qml \
    qml/cover/CoverPage.qml \
    qml/pages/* \
    qml/components/* \
    qml/sounds/* \
    qml/images/* \
    rpm/harbour-sailroid.changes.in \
    rpm/harbour-sailroid.changes.run.in \
    rpm/harbour-sailroid.spec \
    rpm/harbour-sailroid.yaml \
    translations/*.ts \
    harbour-sailroid.desktop

SAILFISHAPP_ICONS = 86x86 108x108 128x128 172x172

# to disable building translations every time, comment out the
# following CONFIG line
#CONFIG += sailfishapp_i18n

# German translation is enabled as an example. If you aren't
# planning to localize your app, remember to comment out the
# following TRANSLATIONS line. And also do not forget to
# modify the localized app name in the the .desktop file.
#TRANSLATIONS += translations/harbour-sailroid-de.ts

HEADERS += \
    src/ball.h \
    src/gameboard.h \
    src/target.h \
    src/trampoline.h
