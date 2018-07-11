#ifndef TUPDATE_H
#define TUPDATE_H

#include <QObject>

class TUpdate : public QObject
{
    Q_OBJECT
public:
    explicit TUpdate(QObject *parent = nullptr);

private slots:
    void TestSimpleUpdate();
    void TestUpdateWhere();
};

#endif // TUPDATE_H
