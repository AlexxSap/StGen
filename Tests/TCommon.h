#ifndef TCOMMON_H
#define TCOMMON_H

#include <QObject>

class TCommon : public QObject
{
    Q_OBJECT
public:
    explicit TCommon(QObject *parent = nullptr);

private slots:
    void TestConv();
};

#endif // TCOMMON_H
