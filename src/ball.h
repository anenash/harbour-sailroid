#ifndef BALL_H
#define BALL_H

#include <QObject>
#include <QPoint>

class Ball : public QObject
{
    Q_OBJECT
public:
    explicit Ball(QObject *parent = nullptr);

    void setStartPosition(QPoint point);
    void moveBall(QPoint point);

    bool contains(QPoint point);
    QPoint getPoint() { return m_ball; }

signals:


public slots:


private:
    QPoint m_ball;
};

#endif // BALL_H
