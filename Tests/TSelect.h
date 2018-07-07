#ifndef TSELECT_H
#define TSELECT_H

#include <QObject>

class TSelect : public QObject
{
    Q_OBJECT
public:
    explicit TSelect(QObject *parent = nullptr);

private:
    void init();

private slots:
    void TestSimpleSelect();
    void TestSimpleSelectColumns();
    void TestSimpleSelectColumnsWithAliases();
    void TestSimpleSelectFromBase();
    void TestSimpleWhere();
    void TestSimpleWhereWithBind();
    void TestSimpleWhereWithBindFromBase();
    void TestComplexWhere();
    void TestDistinct();
};

#endif // TSELECT_H
