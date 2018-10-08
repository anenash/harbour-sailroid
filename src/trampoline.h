#ifndef TRAMPOLINE_H
#define TRAMPOLINE_H

#include <QObject>
#include <QVector>
#include <QPoint>

const int trampolineWidth = 4;

class Trampoline: public QObject
{
    Q_OBJECT
public:
    explicit Trampoline(QObject *parent = nullptr);

    void addPoint(QPoint point);
    bool contains(QPoint point);
    QPoint getLeftPoint() const;
    QPoint getRightPoint() const;
    void moveLeft(QPoint point);
    void moveRight(QPoint point);

    void clear() { m_trampoline.clear(); }

private:
    QVector<QPoint> m_trampoline;

};

#endif // TRAMPOLINE_H
