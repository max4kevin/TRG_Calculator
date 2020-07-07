import QtQuick 2.11
import QtQuick.Controls 2.4

MenuItem {
    id: menuItem
    implicitWidth: 200
    implicitHeight: 20
    padding: 0
    indicator: Item {
        implicitWidth: 20
        implicitHeight: 20
        Rectangle {
           width: 13
           height: 13
           anchors.centerIn: parent
           visible: menuItem.checkable
           opacity: enabled ? 1.0 : 0.3
           color: menuItem.highlighted ? mainWindow.hControlColor : mainWindow.controlColor
           border.color: menuItem.highlighted ? mainWindow.hTextColor : mainWindow.textColor
           radius: 3
           Rectangle {
               width: 5
               height: 5
               anchors.centerIn: parent
               visible: menuItem.checked
               opacity: enabled ? 1.0 : 0.3
               color: menuItem.highlighted ? mainWindow.hTextColor : mainWindow.textColor
               radius: 2
           }
        }
    }
    background: ControlBackground {
        color: highlighted ? mainWindow.hControlColor : mainWindow.controlColor
    }
    contentItem: Item {
        Text {
            id: itemText
            anchors.verticalCenter: parent.verticalCenter
            leftPadding: menuItem.indicator.width
            rightPadding: menuItem.arrow.width
            text: menuItem.action.text
            font: menuItem.font
            opacity: enabled ? 1.0 : 0.3
            color: menuItem.highlighted ? mainWindow.hTextColor : mainWindow.textColor
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
        }

        Text {
             //the shortcut
            anchors.verticalCenter: parent.verticalCenter
            leftPadding: menuItem.width*2/3
            rightPadding: menuItem.arrow.width
            text: menuItem.action.shortcut ? menuItem.action.shortcut : ""
            font: menuItem.font
            opacity: itemText.opacity
            color: itemText.color
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
        }
    }
}
