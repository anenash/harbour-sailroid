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

public slots:
    void moveLeft();
    void moveRight();

private:
    QVector<QPoint> m_trampoline;

};

#endif // TRAMPOLINE_H
