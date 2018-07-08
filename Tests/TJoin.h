#ifndef TJOIN_H
#define TJOIN_H

#include <QObject>

class TJoin : public QObject
{
    Q_OBJECT
public:
    explicit TJoin(QObject *parent = nullptr);

private slots:
    void TestSimpleInnerJoin();
};

#endif // TJOIN_H
