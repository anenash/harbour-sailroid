#include "gameboard.h"

#include <QDebug>

QmlPiece::QmlPiece(bool value): m_active(value)
{

}

GameBoard::GameBoard(QObject *parent) : QObject(parent)
{
    m_currentDirection = GameBoard::Undefined;
    m_score = 0;
    m_level = 1;
    m_started = false;
    m_paused = false;
    m_muted = false;
    m_targets = 0;
    m_trampolineMoved = false;

    emit startedChanged(m_started);
    emit pausedChanged(m_paused);
    emit mutedChanged(m_muted);

    emit levelChanged(m_level);
    emit scoreChanged(m_score);
    emit targetsChanged(m_targets);
    clearBoard();
}

void GameBoard::clearBoard()
{
    m_gameBoard.clear();
    m_trampoline.clear();
    for (int i = 0; i < BoardHeight * BoardWidth; ++i)
    {
        m_gameBoard.append(new QmlPiece(false));
    }
    emit gameBoardChanged(m_gameBoard);
}

void GameBoard::start()
{
    m_currentDirection = GameBoard::UpRight;
    QPoint trampolineStartPoint;
    m_trampoline.clear();
    m_trampolineMoved = false;
    int x = BoardWidth / 4 + 1;
    for (int i = 0; i < 4; i++)
    {
        trampolineStartPoint.setY(BoardHeight - 1);
        trampolineStartPoint.setX(x + i);
        m_trampoline.addPoint(trampolineStartPoint);
    }
    QPoint ballStartPosition(BoardWidth / 2, BoardHeight - 2);
    m_ball.setStartPosition(ballStartPosition);

    generateTargets();
    m_started = true;
    emit startedChanged(m_started);
    emit pausedChanged(m_paused);
    updateBoard();
    m_timer.start(timeoutTime(), this);
}

void GameBoard::generateTargets()
{
    m_target.clear();
    for (int j = 0; j < 5; j++)
    {
        for (int k = 0; k < BoardWidth; k++)
        {
            m_target.addTarget(QPoint(k, j));
        }
    }
}

void GameBoard::updateBoard()
{
    QPoint point;
    for (int x = 0; x < BoardWidth; x++)
    {
        for (int y = 0; y < BoardHeight; y++)
        {
            int pos = getShapeIndex(x, y);
            point.setX(x);
            point.setY(y);
            bool res = m_trampoline.contains(point)
                    || m_ball.contains(point)
                    || m_target.contains(point);
            m_gameBoard.replace(pos, new QmlPiece(res));
        }
    }

    emit gameBoardChanged(m_gameBoard);
}

void GameBoard::pause(bool pause)
{
    m_paused = pause;
    if (m_paused)
    {
        m_timer.stop();
    }
    else
    {
        m_timer.start(timeoutTime(), this);
    }
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

    clearBoard();
    start();
}

void GameBoard::mute()
{
    m_muted = !m_muted;
    emit mutedChanged(m_muted);
}

void GameBoard::moveLeft()
{
    if(m_paused || !m_started)
    {
        return;
    }
    m_mutex.lock();
    QPoint t = m_trampoline.getLeftPoint();
    if (t.x() > 0)
    {
        t.setX(t.x() - 1);
        m_trampoline.moveLeft(t);
        QPoint n = calculateNextPoint(m_currentDirection);
        if(m_trampoline.contains(n))
        {
            m_trampolineMoved = true;
            QPoint b = m_ball.getPoint();
            if (b.x() > 1)
            {
                b.setX(b.x() - 1);
//                qDebug() << "b.x()" << b.x() << m_currentDirection;
                if (b.x() <= 0)
                {
                    m_currentDirection = GameBoard::UpRight;
                }
            }
            m_ball.moveBall(b);
        }
    }
    updateBoard();
    m_mutex.unlock();
}

void GameBoard::moveRight()
{
    if(m_paused || !m_started)
    {
        return;
    }
    m_mutex.lock();
    QPoint t = m_trampoline.getRightPoint();
    if (t.x() < BoardWidth - 1)
    {
        t.setX(t.x() + 1);
        m_trampoline.moveRight(t);
        QPoint n = calculateNextPoint(m_currentDirection);
        if(m_trampoline.contains(n))
        {
            m_trampolineMoved = true;
            QPoint b = m_ball.getPoint();
            if (b.x() < BoardWidth - 2)
            {
                b.setX(b.x() + 1);
//                qDebug() << "b.x()" << b.x() << m_currentDirection;
                if (b.x() >= BoardWidth - 1)
                {
                    m_currentDirection = GameBoard::UpLeft;
                }
            }
            m_ball.moveBall(b);
        }
    }
    updateBoard();
    m_mutex.unlock();
}

void GameBoard::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_timer.timerId())
    {
        m_mutex.lock();
        QPoint nextPoint = calculateNextPoint(m_currentDirection);
//        qDebug() << "nextPoint" << nextPoint << m_currentDirection;
        if (tryMove(nextPoint))
        {
//            qDebug() << "tryMove" << nextPoint << m_currentDirection;
            if (m_trampoline.contains(nextPoint) /*&& !m_trampolineMoved*/)
            {
                m_trampolineMoved = false;
                nextPoint = calculateNextPoint(m_currentDirection);
//                qDebug() << "m_trampoline" << nextPoint << m_currentDirection;
            }
            m_ball.moveBall(nextPoint);
            emit ballMove();
        }
        else
        {
            m_timer.stop();
            m_started = false;
            emit startedChanged(m_started);
            emit gameOver();
        }

        updateBoard();
        m_mutex.unlock();
    }
}

bool GameBoard::tryMove(QPoint &point)
{
    if (checkTrampoline(point))
    {
        return true;
    }
    if (checkTargets(point))
    {
        return true;
    }
    if (point.x() <= 0
            || point.x() >= BoardWidth - 1
            || point.y() <= 0)
    {
        changeCurrentDirection(m_currentDirection, point);
        return true;
    }
    if (point.y() > BoardHeight - 1)
    {
        return false;
    }
    return true;
}

bool GameBoard::checkTrampoline(QPoint &point)
{
    if (m_trampoline.contains(point))
    {
        if (m_target.empty())
        {
            generateTargets();
        }
        switch (m_currentDirection) {
        case GameBoard::DownLeft:
            m_currentDirection = GameBoard::UpLeft;
            break;
        case GameBoard::DownRight:
            m_currentDirection = GameBoard::UpRight;
            break;
        default:
            break;
        }
        return true;
    }
    return false;
}

bool GameBoard::checkTargets(QPoint &point)
{
    if (m_target.contains(point))
    {
        m_target.removeTarget(point);
        m_targets++;
        m_score += 200;
        switch (m_currentDirection) {
        case GameBoard::DownLeft:
            m_currentDirection = GameBoard::UpLeft;
            break;
        case GameBoard::DownRight:
            m_currentDirection = GameBoard::UpRight;
            break;
        case GameBoard::UpLeft:
            if (point.x() == 0)
            {
                m_currentDirection = GameBoard::DownRight;
            }
            else
            {
                m_currentDirection = GameBoard::DownLeft;
            }
            break;
        case GameBoard::UpRight:
            if (point.x() == BoardWidth - 1)
            {
                m_currentDirection = GameBoard::DownLeft;
            }
            else
            {
                m_currentDirection = GameBoard::DownRight;
            }
            break;
        default:
            break;
        }
        emit scoreChanged(m_score);
        if ((9 >= m_level) && (m_score - (m_level * 5000) >= 0))
        {
            m_level++;
            m_timer.start(timeoutTime(), this);
            emit levelChanged(m_level);
        }
        emit levelChanged(m_level);
        emit targetsChanged(m_targets);
        return true;
    }
    return false;
}

QPoint GameBoard::calculateNextPoint(int direction)
{
    QPoint point = m_ball.getPoint();
    int x;
    int y;
    switch (direction) {
    case GameBoard::UpRight:
        x = point.x() + 1;
        y = point.y() - 1;
        if (x < BoardWidth)
        {
            point.setX(x);
        }
        if (y >= 0)
        {
            point.setY(y);
        }
        break;
    case GameBoard::DownLeft:
        x = point.x() - 1;
        y = point.y() + 1;
        if (x >= 0)
        {
            point.setX(x);
        }
        point.setY(y);
        break;
    case GameBoard::UpLeft:
        x = point.x() - 1;
        y = point.y() - 1;
        if (x >= 0)
        {
            point.setX(x);
        }
        if (y >= 0)
        {
            point.setY(y);
        }
        break;
    case GameBoard::DownRight:
        x = point.x() + 1;
        y = point.y() + 1;
        if (x < BoardWidth)
        {
            point.setX(x);
        }
        point.setY(y);
        break;
    default:
        break;
    }
    return point;
}

void GameBoard::changeCurrentDirection(int direction, QPoint &point)
{
    switch (direction) {
    case GameBoard::UpRight:
        if (point.y() == 0)
        {
            if (point.x() >= BoardWidth - 1)
            {
                m_currentDirection = GameBoard::DownLeft;
            }
            else
            {
                m_currentDirection = GameBoard::DownRight;
            }
        }
        else
        {
            if (point.x() > 0)
            {
                m_currentDirection = GameBoard::UpLeft;
            }
        }
        break;
    case GameBoard::DownLeft:
        m_currentDirection = GameBoard::DownRight;
        break;
    case GameBoard::UpLeft:
        if (point.y() == 0)
        {
            if (point.x() <= 0)
            {
                m_currentDirection = GameBoard::DownRight;
            }
            else
            {
                m_currentDirection = GameBoard::DownLeft;
            }
        }
        else
        {
            if (point.x() < BoardWidth - 1)
            {
                m_currentDirection = GameBoard::UpRight;
            }
        }
        break;
    case GameBoard::DownRight:
        m_currentDirection = GameBoard::DownLeft;
        break;
    default:
        m_currentDirection = GameBoard::Undefined;
        break;
    }
}
