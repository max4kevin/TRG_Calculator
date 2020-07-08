import QtQuick 2.11
import QtQuick.Controls 2.4

ScrollBar {
    id: control

    contentItem: Rectangle {
        id: indicator
        implicitWidth: 6
        implicitHeight: 6
        radius: width / 2
        color: control.pressed ?  mainWindow.hTextColor : mainWindow.textColor

        PropertyAnimation {
            target: indicator
            property: "opacity"
            from: 1
            to: 0
            duration: 1000
            easing.type: Easing.InCubic
            running: !control.active
        }

        PropertyAnimation {
            target: indicator
            property: "opacity"
            from: 0
            to: 1
            duration: 100
            running: control.active
        }
    }
}
