#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QObject>
#include <QQmlListProperty>

#include "ball.h"
#include "trampoline.h"

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

    Q_INVOKABLE void start();
    Q_INVOKABLE void pause(bool pause);
    Q_INVOKABLE void reset();
    Q_INVOKABLE void mute();

    Q_INVOKABLE void moveLeft();
    Q_INVOKABLE void moveRight();

signals:
    void gameOver();
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

    int m_currentDirection;
    int m_score;
    int m_level;
    int m_targets;

    bool m_started;
    bool m_paused;
    bool m_muted;
};

#endif // GAMEBOARD_H
