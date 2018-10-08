import QtQuick 2.6
import QtMultimedia 5.6
import Sailfish.Silica 1.0

import arkanoid.Game 1.0

import "../components"

Page {
    id: root

    // The effective value will be restricted by ApplicationWindow.allowedOrientations
    allowedOrientations: Orientation.All

    property variant activeApp: Qt.application.state
    property variant appVersion: Qt.application.version

    property string hightScore: database.record
    property bool gameOver: false
    property bool settingsEnabled: false


    FontLoader {
        id: lcdFont

        source: "../pages/ds-digital-bold.ttf"
    }

    SoundEffect {
        id: soundStart

        source: "../sounds/select.wav"
        muted: arkanoidGame.muted
    }

    SoundEffect {
        id: soundTargetHit

        source: "../sounds/remove_line.wav"
        muted: arkanoidGame.muted
    }

    SoundEffect {
        id: soundMoved

        source: "../sounds/move.wav"
        muted: arkanoidGame.muted
    }

    SoundEffect {
        id: soundRotate

        source: "../sounds/turn.wav"
        muted: arkanoidGame.muted
    }

    SoundEffect {
        id: soundGameOver

        source: "../sounds/gameOver.wav"
        muted: arkanoidGame.muted
    }

    Database {
        id: database
    }

    Game {
        id: arkanoidGame

        onScoreChanged: {
            if(score > parseInt(hightScore)) {
                database.storeData("arkanoidGame", score)
            }
            database.readRecord("arkanoidGame")
        }

        onMutedChanged: {
            if(!soundStart.playing && !arkanoidGame.muted) {
                soundStart.play()
            }
        }

        onStartedChanged: {
            if(!soundStart.playing && arkanoidGame.started) {
                root.gameOver = false
                soundStart.play()
            }
        }

        onPausedChanged: {
            if(!soundStart.playing) {
                soundStart.play()
            }
        }

        onGameOver: {
            root.gameOver = true
            if(!soundGameOver.playing) {
                soundGameOver.play()
            }
        }

        onBallMove: {
            if(!soundMoved.playing) {
                soundMoved.play()
            }
        }

        onTargetsChanged: {
            app.targetsCount = targets
            if(!soundTargetHit.playing) {
                soundTargetHit.play()
            }
        }
    }

    onActiveAppChanged: {
        if (activeApp !== Qt.ApplicationActive) {
            app.coverLevel = arkanoidGame.level
            app.coverScore = arkanoidGame.score
            app.targetsCount = arkanoidGame.targets
            if (arkanoidGame.started)
            {
                arkanoidGame.pause(true)
            }
        }
    }

    onOrientationChanged: {
        if (orientation === Orientation.Portrait || orientation === Orientation.PortraitInverted ) {
            gameBoard.source = "GamePortrait.qml"
        } else if (orientation === Orientation.Landscape || orientation === Orientation.LandscapeInverted) {
            gameBoard.source = "GameLandscape.qml"
        }
    }

    Component.onCompleted: {
        console.log("App version", Qt.application.version)
        database.initDatabase()
        database.readRecord("arkanoidGame")
    }

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: root.height

        Loader {
            id: gameBoard

            anchors.fill: parent
            source: "GamePortrait.qml"
        }
    }
}
