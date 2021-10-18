import QtQuick 2.11
import QtQuick.Controls 2.12

Button {
    id: button
    implicitWidth: 70
    height: 20

    contentItem: Text {
        anchors.fill: button
        text: button.text
        font: button.font
        opacity: enabled ? 1.0 : 0.3
        color: button.down ? mainWindow.hTextColor : mainWindow.textColor
        clip: true
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        implicitWidth: 100
        implicitHeight: 40
        color: button.down ? mainWindow.hControlColor : mainWindow.controlColor
        border.color: mainWindow.hControlColor
        border.width: 1
        radius: 2
    }
}
