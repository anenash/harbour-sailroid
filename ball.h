#ifndef BALL_H
#define BALL_H

#include <QObject>

class Ball : public QObject
{
    Q_OBJECT
public:
    explicit Ball(QObject *parent = nullptr);

signals:


public slots:
    void upLeft();
    void upRight();
    void downLeft();
    void downRight();
};

#endif // BALL_H
