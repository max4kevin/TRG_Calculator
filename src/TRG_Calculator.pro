QT += quick
CONFIG += c++11
QMAKE_LFLAGS += -static -static-libgcc

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
    backend.cpp \
    calculation/calculationbase.cpp \
    calculation/calculationmapo.cpp \
    geometry.cpp

RESOURCES += resources.qrc
win32:RC_ICONS += trg.ico
win32:RC_FILE += resources.rc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    backend.h \
    calculation/calculationbase.h \
    calculation/calculationmapo.h \
    geometry.h \
    resources.rc

DISTFILES += \
    qml/CustomButton.qml \
    qml/ParametersWindow.qml \
    qml/PointItem.qml \
    qml/LineItem.qml \
    qml/MenuDelegate.qml \
    qml/WorkZone.qml \
    qml/ControlsBar.qml \
    qml/ResultsZone.qml \
    qml/CustomMenuSeparator.qml \
    qml/ControlBackground.qml

TRANSLATIONS += QtLanguage_ru.ts
