import QtQuick 2.11
import QtQuick.Controls 2.4

TabButton {
    id: tabButton
    implicitWidth: 70
    height: 20

    contentItem: Text {
        anchors.fill: tabButton
        text: tabButton.text
        font: tabButton.font
        opacity: enabled ? 1.0 : 0.3
        color: tabButton.checked ? mainWindow.hTextColor : mainWindow.textColor
        clip: true
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        implicitWidth: 100
        implicitHeight: 40
        color: tabButton.checked ? mainWindow.backgroundColor : mainWindow.controlColor
    }
}
