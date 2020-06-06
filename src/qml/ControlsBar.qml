import QtQuick 2.11
import QtQuick.Controls 2.4

MenuBar {
    height: menuBarHeight
    property alias isDark: themeBtn.checked
//    property alias isLinesEnabled: switchShowLines.checked
//    property alias isNamesEnabled: switchShowPointsNames.checked

    property alias pointsSwitch: switchShowPointsNames
    property alias linesSwitch: switchShowLines
    property alias lockSwitch: switchPointsLock
    property alias invertSwitch: switchInvert


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

        Action {
            id: clearBtn
            text: qsTr("Delete points")
            enabled: workZone.isReady
            onTriggered: {
                backEnd.clear()
                message.send(qsTr("Points deleted"))
            }
        }

        Action {
            id: clearAllBtn
            enabled: workZone.isReady
            text: qsTr("Clear all")
            onTriggered: {
                backEnd.clearAll()
                message.send(qsTr("Area cleared"))
            }
        }

        CustomMenuSeparator {}

        Action {
            id: switchPointsLock
            text: qsTr("Lock points")
            shortcut: "Ctrl+G"
            checkable: true
            checked: true
            onTriggered: {
                workZone.changeLock(checked)
            }
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

        Action {
            id: switchShowLines
            text: qsTr("Show lines")
            shortcut: "Ctrl+L"
            checkable: true
            checked: true
            onTriggered: {
                workZone.showLines(checked)
            }
        }

        CustomMenuSeparator {}

        Action {
            id: themeBtn
            text: qsTr("Dark theme")
            checkable: true
            checked: true
        }

        CustomMenuSeparator {}

        Action {
            id: resultsBtn
            text: qsTr("Show results")
            shortcut: "Alt+R"
            checkable: true
            onTriggered: {
                if (checked) {
                    resultsZone.expand()
                }
                else {
                    resultsZone.collapse()
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

