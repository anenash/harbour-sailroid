#include "gameboard.h"



QmlPiece::QmlPiece(bool value): m_active(value)
{

}

GameBoard::GameBoard(QObject *parent) : QObject(parent)
{
//    m_currentDirection = GameBoard::Undefined;
    m_score = 0;
    m_level = 1;
    m_started = false;
    m_paused = false;
    m_muted = false;
    m_targets = 0;

    emit startedChanged(m_started);
    emit pausedChanged(m_paused);
    emit mutedChanged(m_muted);

    emit levelChanged(m_level);
    emit scoreChanged(m_score);
    emit targetsChanged(m_targets);
}

void GameBoard::start()
{
//    m_currentDirection = GameBoard::Up;
//    QPoint startPoint;
//    int x = BoardWidth / 2;
//    int y = BoardHeight / 2;
//    for (int i = 2; i >= 0; i--)
//    {
//        startPoint.setX(x);
//        startPoint.setY(y + i);
//        m_snake.add(startPoint);
//    }
//    m_timer.start(timeoutTime(), this);
    m_started = true;
    emit startedChanged(m_started);
    emit pausedChanged(m_paused);
}

void GameBoard::pause(bool pause)
{
    m_paused = pause;
//    if (m_paused)
//    {
//        m_timer.stop();
//    }
//    else
//    {
//        m_timer.start(timeoutTime(), this);
//    }
    emit pausedChanged(m_paused);
}

void GameBoard::reset()
{
    m_score = 0;
    m_level = 1;
    m_targets = 0;
    emit scoreChanged(m_score);
    emit levelChanged(m_level);
    emit targetsChanged(m_targets);

    start();
}

void GameBoard::mute()
{
    m_muted = !m_muted;
    emit mutedChanged(m_muted);
}

void GameBoard::moveLeft()
{

}

void GameBoard::moveRight()
{

}
