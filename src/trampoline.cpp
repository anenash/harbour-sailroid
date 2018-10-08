#include "trampoline.h"

Trampoline::Trampoline(QObject *parent) : QObject(parent)
{

}

void Trampoline::addPoint(QPoint point)
{
    if (m_trampoline.length() <= 4)
    {
        m_trampoline.append(point);
    }
}

void Trampoline::moveLeft(QPoint point)
{
    m_trampoline.push_front(point);
    m_trampoline.pop_back();
}

void Trampoline::moveRight(QPoint point)
{
    m_trampoline.push_back(point);
    m_trampoline.pop_front();
}

bool Trampoline::contains(QPoint point)
{
    return m_trampoline.contains(point);
}

QPoint Trampoline::getLeftPoint() const
{
    return m_trampoline.first();
}

QPoint Trampoline::getRightPoint() const
{
    return m_trampoline.last();
}
