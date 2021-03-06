import QtQuick 2.6
import Sailfish.Silica 1.0

import "../components"

Item {
    id: root

    QtObject {
        id: internal

        property bool swipeEnabled: false
    }

    Rectangle {
        anchors.fill: parent
        color: "#36aa45"

        Label {
            anchors.right: parent.right
            anchors.rightMargin: Theme.horizontalPageMargin
            anchors.bottom: parent.bottom
            anchors.bottomMargin: Theme.horizontalPageMargin
            rotation: -45
            text: "E21"
            font.bold: true
            font.pointSize: Theme.fontSizeExtraLarge
        }
    }

    Rectangle {
        id: gameFieldBorders

        anchors.centerIn: game
        height: game.height + Theme.horizontalPageMargin
        width: game.width + Theme.horizontalPageMargin

        color: "transparent"
        border.color: "black"
        border.width: 4
    }

    Image {
        id: leftBorderImage

        anchors.right: gameFieldBorders.left
        anchors.rightMargin: Theme.paddingMedium
        anchors.verticalCenter: gameFieldBorders.verticalCenter
        source: "../images/border.png"
    }

    Image {
        id: rightBorderImage

        anchors.left: gameFieldBorders.right
        anchors.leftMargin: Theme.paddingMedium
        anchors.verticalCenter: gameFieldBorders.verticalCenter
        source: "../images/border.png"
    }

    Item {
        id: game

        anchors.horizontalCenter: root.horizontalCenter
        anchors.top: root.top
        anchors.topMargin: Theme.paddingMedium

        width: gameField.width + gameInfo.width
        height: gameField.height

        Loader {
            id: gameField

            source: settingsEnabled ? "../components/Settings.qml":"../components/GameField.qml"
        }

        GameInfo {
            id: gameInfo

            anchors.top: gameField.top
            anchors.left: gameField.right
            height: gameField.height

            gameHightScore: hightScore
        }
    }
    Loader {
        anchors.top: game.bottom
        anchors.left: root.left
        anchors.right: root.right
        anchors.bottom: root.bottom
        source: internal.swipeEnabled
                ?"../components/GameControlsSwipe.qml"
                :"../components/GameControls.qml"
    }

    Row {
        id: gameButtonsRow

        anchors.top: game.bottom
        anchors.right: root.right
        anchors.rightMargin: Theme.horizontalPageMargin

        spacing: Theme.paddingLarge


//        GameButton {
//            width: Theme.buttonWidthSmall * 0.2
//            buttonColor: "yellow"
//            title: "Swipe"

//            onClicked: {
//                internal.swipeEnabled = !internal.swipeEnabled
//            }
//        }

        GameButton {
            width: Theme.buttonWidthSmall * 0.2
            buttonColor: "yellow"
            title: "Settings"
            visible: false
            enabled: false

            onClicked: {
                settingsEnabled = !settingsEnabled
                if (settingsEnabled) {
                    arkanoidGame.pause(true)
                }
            }
        }

        GameButton {
            width: Theme.buttonWidthSmall * 0.2
            buttonColor: "yellow"
            title: "Start/\nPause"

            onClicked: {
                if (settingsEnabled) {
                    return
                }

                if (!arkanoidGame.started) {
                    arkanoidGame.start()
                } else {
                    if (arkanoidGame.paused) {
                        arkanoidGame.pause(false)
                    } else {
                        arkanoidGame.pause(true)
                    }
                }
            }
        }
        GameButton {
            width: Theme.buttonWidthSmall * 0.2
            buttonColor: "yellow"
            title: "Reset"

            onClicked: {
                arkanoidGame.reset()
            }
        }
        GameButton {
            width: Theme.buttonWidthSmall * 0.2
            buttonColor: "yellow"
            title: "Mute"

            onClicked: {
                arkanoidGame.mute()
            }
        }
    }
}
