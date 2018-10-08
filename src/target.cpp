#include "target.h"

Target::Target(QObject *parent) : QObject(parent)
{

}

void Target::addTarget(QPoint point)
{
    m_target.append(point);
}

bool Target::contains(QPoint point)
{
    return m_target.contains(point);
}

void Target::removeTarget(QPoint point)
{
    int i = m_target.indexOf(point);
    m_target.removeAt(i);
    emit hit(point);
}

bool Target::empty()
{
    return m_target.empty();
}
