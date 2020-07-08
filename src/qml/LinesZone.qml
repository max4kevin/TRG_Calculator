import QtQuick 2.11
import QtQuick.Controls 2.4


//TODO: List padding
Item {
    id: linesZone

    property alias isNewVisible: showNewSwitch.checked

    function deselectLine() {
        linesList.isSelected = false
        workZone.deselectLine()
    }

    //FIXME: Megacrutch (because of dynamic item to delegate signals bag - not all signals catch!)
    function addLine(pointName1, pointName2) {
        for (var i = 0; i < lines.count; ++i) {
            if (lines.get(i).pointName1 === pointName1 && lines.get(i).pointName2 === pointName2) {
                return
            }
            if (lines.get(i).pointName1.charAt(0) > pointName1.charAt(0)) {
                lines.insert(i, {pointName1: pointName1, pointName2: pointName2, isVisible: isNewVisible})
                return
            }
        }
        if (i === lines.count) {
            lines.append({pointName1: pointName1, pointName2: pointName2, isVisible: isNewVisible})
        }
    }

    //FIXME: Can't move the signal to delegate because of catching delegate signal bug
    signal lineVisibilityChanged(var pointName1, var pointName2, var isEnabled)

    ListModel {id: lines}

    ListView {
        id: linesList
        width: parent.width
        anchors.top: parent.top
        anchors.bottom: showNewSwitch.top
        orientation: Qt.Vertical
        spacing: 0
        clip: true
        highlightMoveDuration: 0
        boundsBehavior: Flickable.StopAtBounds

        property bool isSelected: false

        highlight: Rectangle {
            id: selection
            width: linesList.width
            visible: true
            color: "transparent"
            SequentialAnimation on color{
                running: linesList.isSelected
                loops: Animation.Infinite
                onRunningChanged: {
                    if (!running)
                    {
                        selection.color = "transparent"
                    }
                }

                ColorAnimation { from: mainWindow.controlColor; to: mainWindow.hControlColor; duration: 1000 }
                ColorAnimation { from: mainWindow.hControlColor; to: mainWindow.controlColor;  duration: 1000 }
            }
        }

        delegate: MouseArea {
            id: lineRow
            height: 40
            anchors.left: parent.left
            anchors.right: parent.right

            onClicked: {
                if (linesList.currentIndex === index) {
                    if (linesList.isSelected) {
                        linesZone.deselectLine()
                        return
                    }
                }
                else {
                    workZone.deselectLine()
                    linesList.currentIndex = index
                }
                if (lines.get(index).isVisible) {
                    linesList.isSelected = true
                    workZone.focusOnLine(nameText.name1, nameText.name2)
                }
            }

            Text {
                id: nameText

                height: 40
                anchors.left: hideBtn.right
                anchors.right: parent.right
                anchors.leftMargin: rightPanel.spacing
                anchors.rightMargin: rightPanel.spacing
                clip: true
                color: (pointsZone.currentIndex === index) && (pointsZone.isSelected) ? mainWindow.hTextColor : mainWindow.textColor
                verticalAlignment: Text.AlignVCenter
                text: name1+"-"+name2

                property string name1: pointName1
                property string name2: pointName2
            }

            CheckBox {
                id: hideBtn
                checked: isVisible
                width: 20
                height: 20
                anchors.verticalCenter: nameText.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: rightPanel.spacing
                anchors.rightMargin: rightPanel.spacing
                display: AbstractButton.IconOnly

                indicator: Item {
                    implicitWidth: 20
                    implicitHeight: 20
                    Rectangle {
                        width: 13
                        height: 13
                        anchors.centerIn: parent
                        opacity: enabled ? 1.0 : 0.3
                        color: hideBtn.highlighted ? mainWindow.hControlColor : mainWindow.controlColor
                        border.color: hideBtn.highlighted ? mainWindow.hTextColor : mainWindow.textColor
                        radius: 3
                        Rectangle {
                           width: 5
                           height: 5
                           anchors.centerIn: parent
                           visible: hideBtn.checked
                           opacity: enabled ? 1.0 : 0.3
                           color: hideBtn.highlighted ? mainWindow.hTextColor : mainWindow.textColor
                           radius: 2
                        }
                    }
                }

                onCheckStateChanged: {
                    if (lines.get(index).isVisible !== checked)
                    {
                        lines.setProperty(index, "isVisible", checked)
                        linesZone.lineVisibilityChanged(nameText.name1, nameText.name2, checked)
                        if (!checked) {
                            linesZone.deselectLine()
                        }
                    }
                }
            }
        }
        model: lines

        ScrollBar.vertical: CustomScrollBar {
               visible: parent.width !== 0
        }
    }

    CheckBox {
        id: showNewSwitch
        height: 25
        width: implicitWidth
        checked: true
        text: qsTr("Show new lines")
        anchors.bottom: buttons.top
        anchors.horizontalCenter: parent.horizontalCenter

        indicator: Item {
            implicitWidth: 25
            implicitHeight: 25
            Rectangle {
                width: 13
                height: 13
                anchors.centerIn: parent
                opacity: enabled ? 1.0 : 0.3
                color: "transparent"
                border.color: showNewSwitch.checked? "green" : "red"
                radius: 3
                Rectangle {
                    width: 7
                    height: 7
                   anchors.centerIn: parent
                   visible: showNewSwitch.checked
                   opacity: enabled ? 1.0 : 0.3
                   color: showNewSwitch.checked? "green" : "red"
                   radius: 2
                }
            }
        }

        contentItem: Text {
            text: showNewSwitch.text
            font: showNewSwitch.font
            opacity: enabled ? 1.0 : 0.3
            color: showNewSwitch.checked? "green" : "red"
            clip: true
            verticalAlignment: Text.AlignVCenter
            leftPadding: showNewSwitch.indicator.width
        }
    }

    Row {
        id: buttons
        height: 20
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
//        anchors.bottom: parent.bottom

        CustomButton {
            id: showAllBtn
            height: parent.height
            width: parent.width/2
            text: qsTr("Show all")
            enabled: lines.count > 0
            onClicked: {
                for (var i = 0; i < lines.count; ++i)
                {
                    lines.setProperty(i, "isVisible", true)
                    linesZone.lineVisibilityChanged(lines.get(i).pointName1, lines.get(i).pointName2, true)
                }
            }
        }

        CustomButton {
            id: hideAllBtn
            height: parent.height
            width: parent.width/2
            text: qsTr("Hide all")
            enabled: lines.count > 0
            onClicked: {
                for (var i = 0; i < lines.count; ++i)
                {
                    lines.setProperty(i, "isVisible", false)
                    linesZone.lineVisibilityChanged(lines.get(i).pointName1, lines.get(i).pointName2, false)
                }
                linesZone.deselectLine()
            }
        }
    }

    //FIXME: Megacrutch (because of dynamic item to delegate signals bag - not all signals catch!)
    Connections {
        target: backEnd
        onPointUpdated: {
            if (!status) {
                for (var i = 0; i < lines.count; ++i) {
                    if (pointName === lines.get(i).pointName1 || pointName === lines.get(i).pointName2) {
                        lines.remove(i)
                    }
                }
            }
        }

        onPointsConnected: {
            addLine(pointName1, pointName2)
        }
    }
}
