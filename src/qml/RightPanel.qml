import QtQuick 2.11
import QtQuick.Controls 2.4

Rectangle {
    id: rightPanel
    color: mainWindow.backgroundColor
    clip: true
    width: expandedWidth
    property real expandedWidth: 450
    property real animationDuration: 100
    property real spacingValue: 10
    property real spacing: width > spacingValue ? spacingValue : 0

    function expand() {
        collapseAnimation.stop()
        expandAnimation.start()
    }

    function collapse() {
        expandAnimation.stop()
        collapseAnimation.start()
    }

    PropertyAnimation {
        id: expandAnimation
        target: rightPanel
        property: "width"
        to: expandedWidth
        duration: rightPanel.animationDuration
        onStarted: rightPanel.visible = true
    }

    PropertyAnimation {
        id: collapseAnimation
        target: rightPanel
        property: "width"
        to: 1
        duration: rightPanel.animationDuration
        onStopped: rightPanel.visible = false
    }

    CustomButton {
        id: hideBtn
        width: parent.width
        height: 20
        text: qsTr("Hide panel")
        onClicked: {
            parent.collapse()
            controls.panelSwitch.checked = false
        }
    }

    //TODO: delegate
    TabBar {
        id: tabBar
        width: parent.width
        height: 20
        contentHeight: height
        currentIndex: 0
        anchors.top: hideBtn.bottom
        onCurrentIndexChanged: swipeView.setCurrentIndex(currentIndex)
        background: Rectangle {color: mainWindow.backgroundColor}

        CustomTabButton {
            text: qsTr("Results")
        }
        CustomTabButton {
            text: qsTr("Points")
        }
        CustomTabButton {
            text: qsTr("Lines")
        }
    }

    SwipeView {
        id: swipeView
        anchors.topMargin: 40
        anchors.fill: parent
        currentIndex: 0
        onCurrentIndexChanged: tabBar.setCurrentIndex(currentIndex)

        ResultsZone {
            id: resultsZone
        }
        Rectangle {
            id: pointsZone
            color: "red"
        }
        Rectangle {
            id: linesZone
            color: "blue"
        }
    }
}

