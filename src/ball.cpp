#include "ball.h"

Ball::Ball(QObject *parent) : QObject(parent)
{

}

void Ball::setStartPosition(QPoint point)
{
    m_ball = point;
}

void Ball::moveBall(QPoint point)
{
    if (m_ball != point)
    {
        m_ball = point;
    }
}

bool Ball::contains(QPoint point)
{
    return m_ball == point;
}
