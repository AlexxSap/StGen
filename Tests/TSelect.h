#ifndef TSELECT_H
#define TSELECT_H

#include <QObject>

class TSelect : public QObject
{
    Q_OBJECT
public:
    explicit TSelect(QObject *parent = nullptr);

private slots:
    void TestSimpleSelect();
    void TestSimpleSelectColumns();
    void TestSimpleSelectFromBase();
    void TestSimpleWhere();
};

#endif // TSELECT_H
