#ifndef TINSERT_H
#define TINSERT_H

#include <QObject>

class TInsert : public QObject
{
    Q_OBJECT
public:
    explicit TInsert(QObject *parent = nullptr);

private slots:
    void TestSimpleInsert();
    void TestInsertFromSelect();
};

#endif // TINSERT_H
