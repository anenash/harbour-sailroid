#ifndef TARGET_H
#define TARGET_H

#include <QObject>
#include<QVector>
#include <QPoint>

class Target : public QObject
{
    Q_OBJECT

public:
    explicit Target(QObject *parent = nullptr);

    void addTarget(QPoint point, bool active);
    bool contains(QPoint point);

signals:
    void hit(QPoint point);

public slots:

private:
    QVector<QPair<QPoint, bool>> m_target;
};

#endif // TARGET_H
