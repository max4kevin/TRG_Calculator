import QtQuick 2.11
import QtQuick.Controls 2.4

MenuBar {
    height: menuBarHeight
    property alias isDark: themeBtn.checked
//    property alias isLinesEnabled: switchShowLines.checked
//    property alias isNamesEnabled: switchShowPointsNames.checked

    property alias pointsSwitch: switchShowPointsNames
//    property alias linesSwitch: switchShowLines
    property alias invertSwitch: switchInvert
    property alias panelSwitch: switchPanel

    background: ControlBackground {}

    delegate: MenuBarItem {
        id: menuBarItem
        height: menuBarHeight
        contentItem: Text {
            text: menuBarItem.text
            font: menuBarItem.font
            opacity: enabled ? 1.0 : 0.3
            color: menuBarItem.highlighted ? mainWindow.hTextColor : mainWindow.textColor
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
        }
        background: Rectangle {
            anchors.fill: parent
            color: highlighted ? mainWindow.hControlColor : mainWindow.controlColor
        }
    }
    Menu {
        id: fileMenu
        title: qsTr("File")
        delegate: MenuDelegate {}
        background: ControlBackground {}

        Action {
            id: openBtn
            text: qsTr("Open...")
            shortcut: "Ctrl+O"
            onTriggered: openDialog.open()
        }
        Action {
            id: saveBtn
            text: qsTr("Save")
            shortcut: "Ctrl+S"
            enabled: workZone.isReady
            onTriggered: {
                if (backEnd.getFileType() === "trg")
                {
                    backEnd.saveFile();
                    message.send(qsTr("File saved"))
                }
                else
                {
                    saveAsBtn.trigger()
                }
            }
        }
        Action {
            id: saveAsBtn
            text: qsTr("Save as...")
            enabled: workZone.isReady
            onTriggered: {
                saveDialog.folder = "file:///"+backEnd.getFilePath()
                saveDialog.currentFile = "file:///"+backEnd.getFileName()
                saveDialog.open()
            }
        }

        CustomMenuSeparator {}

        Action {
            id: closeBtn
            text: qsTr("Close")
            shortcut: "Ctrl+W"
            enabled: workZone.isReady
            onTriggered: workZone.closeImage()
        }

        CustomMenuSeparator {}

        Action {
            text: qsTr("Quit")
            onTriggered: mainWindow.close()
        }
    }
    Menu {
        title: qsTr("Edit")
        delegate: MenuDelegate{}
        background: ControlBackground {implicitWidth: 250}

        Action {
            id: undoBtn
            text: qsTr("Undo")
            shortcut: "Ctrl+Z"
            enabled: false
            onTriggered: {
                backEnd.undo()
                message.send(qsTr("Undo"))
            }
        }

        Action {
            id: redoBtn
            text: qsTr("Redo")
            shortcut: "Ctrl+Y"
            enabled: false
            onTriggered: {
                backEnd.redo()
                message.send(qsTr("Redo"))
            }
        }

        CustomMenuSeparator {}

        Menu {
            id: methodsBtn
            title: qsTr("Calculation method")
            delegate: MenuDelegate {
                id: menuItem
                indicator: Item {
                    anchors.verticalCenter: parent.verticalCenter
                    implicitHeight: 20
                    implicitWidth: 20
                    visible: menuItem.checkable
                    opacity: menuItem.checked ? 1 : 0

                    Rectangle {
                        width: 7
                        height: 7
                        radius: 4
                        anchors.centerIn: parent
                        color: menuItem.highlighted ? mainWindow.hTextColor : mainWindow.textColor
                    }
                }
            }
            background: ControlBackground {implicitWidth: 150}

            function setMethod(method) {
                for (var i = 0; i < count; ++i) {
                    if (itemAt(i).text !== method) {
                        itemAt(i).checked = false
                    }
                }
                rightPanel.resetSelections()
                backEnd.setMethod(method)
            }

            Component.onCompleted: {
                var method = backEnd.getConfig("method")
                for (var i = 0; i < count; ++i) {
                    if (itemAt(i).text === method) {
                        itemAt(i).checked = true
                        return
                    }
                }
            }

            Action {
                id: mapoBtn
                text: "MAPO"
                checkable: true
                checked: true
                onTriggered: {
                    if (checked) {
                         methodsBtn.setMethod(text)
                    }
                    else {
                        checked = true
                    }
                }
            }

//            Action {
//                id: faceBtn
//                text: "SOME_METHOD"
//                checkable: true
//                checked: false
//                onTriggered: {
//                    if (checked) {
//                         methodsBtn.setMethod(text)
//                    }
//                    else {
//                        checked = true
//                    }
//                }
//            }
        }

        CustomMenuSeparator {}

        Action {
            id: paramsBtn
            text: qsTr("Parameters")
            shortcut: "Ctrl+Shift+P"
            onTriggered: parametersWindow.show()
        }

    }
    Menu {
        title: qsTr("View")
        delegate: MenuDelegate{}
        background: ControlBackground {implicitWidth: 280}

        Action {
            id: zoomBtn
            text: qsTr("Zoom in")
            shortcut: "Ctrl++"
            enabled: workZone.isReady
            onTriggered: workZone.zoom()
        }

        Action {
            id: unzoomBtn
            text: qsTr("Zoom out")
            shortcut: "Ctrl+-"
            enabled: workZone.isReady
            onTriggered: workZone.unzoom()
        }

        Action {
            id: switchInvert
            text: qsTr("Invert image")
            shortcut: "Ctrl+I"
            checkable: true
            enabled: workZone.isReady
            onTriggered: workZone.invertImage()
        }

        CustomMenuSeparator {}

        Action {
            id: switchShowPointsNames
            text: qsTr("Show points names")
            shortcut: "Ctrl+P"
            checkable: true
            checked: false
            onTriggered: {
                workZone.showPointsNames(checked)
            }
        }

        CustomMenuSeparator {}

        Action {
            id: themeBtn
            text: qsTr("Dark theme")
            checkable: true
            checked: true

            onTriggered: {
                if (checked) {
                    backEnd.setConfig("theme", "dark")
                }
                else {
                    backEnd.setConfig("theme", "light")
                }
            }

            Component.onCompleted: {
               var theme = backEnd.getConfig("theme")
               if (theme === "light") {
                   checked = false
               }
            }
        }

        CustomMenuSeparator {}

        Action {
            id: switchPanel
            text: qsTr("Show work panel")
            shortcut: "Alt+R"
            checkable: true
            checked: false
            onTriggered: {
                if (checked) {
                    rightPanel.expand()
                }
                else {
                    rightPanel.collapse()
                }
            }
        }
    }
    Menu {
        delegate: MenuDelegate{}
        background: ControlBackground {implicitWidth: 150}
        title: qsTr("Help")

        Action {
            id: aboutBtn
            text: qsTr("About")
            shortcut: "F1"
            onTriggered: {
                aboutWindow.show()
            }
        }
    }

    Connections {
        target: backEnd
        onUndoStateChanged: {
            undoBtn.enabled = isEnabled
        }

        onRedoStateChanged: {
            redoBtn.enabled = isEnabled
        }
    }
}

