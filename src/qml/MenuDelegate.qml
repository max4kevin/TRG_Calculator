import QtQuick 2.12
import QtQuick.Controls 2.12

MenuItem {
    id: menuItem
    implicitWidth: 200
    implicitHeight: 25
    padding: 0

    arrow: Canvas {
        x: parent.width - width - 5
        anchors.verticalCenter: parent.verticalCenter
        height: 7
        width: 7
        visible: menuItem.subMenu
        property color color: menuItem.highlighted ? mainWindow.hTextColor : mainWindow.textColor
        onColorChanged: requestPaint()

        onPaint: {
            var ctx = getContext("2d")
            ctx.fillStyle = color
            ctx.moveTo(0, 0)
            ctx.lineTo(0, height)
            ctx.lineTo(width, height/2)
            ctx.closePath()
            ctx.fill()
        }
    }

    indicator: Item {
        implicitWidth: 20
        implicitHeight: 20
        anchors.verticalCenter: parent.verticalCenter
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
        anchors.verticalCenter: parent.verticalCenter
        Text {
            id: itemText
            anchors.verticalCenter: parent.verticalCenter
            leftPadding: menuItem.indicator.width
            rightPadding: menuItem.arrow.width
            text: menuItem.action ? menuItem.action.text : menuItem.text
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
            text: menuItem.action ? (menuItem.action.shortcut ? menuItem.action.shortcut : "") : ""
            font: menuItem.font
            opacity: itemText.opacity
            color: itemText.color
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
        }
    }
}
