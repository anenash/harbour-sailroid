#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QObject>
#include <QQmlListProperty>
#include <QBasicTimer>
#include <QTimerEvent>
#include <QList>
#include <QPoint>
#include <QMutex>

#include "ball.h"
#include "trampoline.h"
#include "target.h"

const int BoardWidth = 10;
const int BoardHeight = 20;


class QmlPiece: public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool active READ active NOTIFY activeChanged)

public:
    explicit QmlPiece(bool value);

    bool active() { return m_active; }

signals:
    void activeChanged();

private:
    bool m_active;
};

class GameBoard : public QObject
{
    Q_OBJECT


    Q_PROPERTY(bool started READ started NOTIFY startedChanged)
    Q_PROPERTY(bool paused READ paused NOTIFY pausedChanged)
    Q_PROPERTY(bool muted READ muted NOTIFY mutedChanged)
    Q_PROPERTY(QString score READ score NOTIFY scoreChanged)
    Q_PROPERTY(int level READ level NOTIFY levelChanged)
    Q_PROPERTY(int targets READ targets NOTIFY targetsChanged)
    Q_PROPERTY(QQmlListProperty<QmlPiece> gameBoard READ gameBoard NOTIFY gameBoardChanged)

public:
    explicit GameBoard(QObject *parent = nullptr);

    QQmlListProperty<QmlPiece> gameBoard() { return QQmlListProperty<QmlPiece>(this, m_gameBoard); }
    QString score() { return QString::number(m_score); }
    int targets() { return m_targets; }
    int level() { return m_level; }
    bool started() { return m_started; }
    bool paused() { return m_paused; }
    bool muted() { return m_muted; }


    enum BallDirection {
        Undefined = 0,
        UpLeft,
        DownLeft,
        UpRight,
        DownRight
    };
    Q_ENUM(BallDirection)

    Q_INVOKABLE void start();
    Q_INVOKABLE void pause(bool pause);
    Q_INVOKABLE void reset();
    Q_INVOKABLE void mute();

    Q_INVOKABLE void moveLeft();
    Q_INVOKABLE void moveRight();

protected:
    void timerEvent(QTimerEvent *event) override;

signals:
    void gameOver();
    void ballMove();
    void gameBoardChanged(QList<QmlPiece *> data);
    void scoreChanged(int score);
    void levelChanged(int level);
    void targetsChanged(int targets);
    void startedChanged(bool started);
    void pausedChanged(bool paused);
    void mutedChanged(bool muted);

public slots:

private:
    QList<QmlPiece *> m_gameBoard;
    QBasicTimer m_timer;

    Ball m_ball;
    Trampoline m_trampoline;
    Target m_target;

    int m_currentDirection;
    int m_score;
    int m_level;
    int m_targets;

    bool m_started;
    bool m_paused;
    bool m_muted;

    QMutex m_mutex;
    bool m_trampolineMoved;

private:
    void clearBoard();
    void updateBoard();
    void generateTargets();
    bool checkTrampoline(QPoint &point);
    bool checkTargets(QPoint &point);
    void changeCurrentDirection(int direction, QPoint &point);
    QPoint calculateNextPoint(int direction);
    bool tryMove(QPoint &point);
    inline int getShapeIndex(int x, int y) { return (y * BoardWidth) + x; }

    inline int timeoutTime() { return 500 - ((m_level - 1) * 100); }
};

#endif // GAMEBOARD_H
