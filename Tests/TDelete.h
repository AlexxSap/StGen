#ifndef TDELETE_H
#define TDELETE_H

#include <QObject>

class TDelete : public QObject
{
    Q_OBJECT
public:
    explicit TDelete(QObject *parent = nullptr);

private slots:
    void TestSimpleDelete();
    void TestSimpleDeleteWithWhere();
    void TestSimpleDeleteFromBase();
};

#endif // TDELETE_H
