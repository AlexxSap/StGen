#ifndef STGENGLOBAL_H
#define STGENGLOBAL_H

#include "StGen.h"

#define SimpleTypeOperation(name) template <typename A, typename B> \
AbsExprPointer name(A a, B b, NOT_IS(AbstractExpression, A), NOT_IS(AbstractExpression, B)) \
{ \
    return ExprPointer::create(AbstractExpression::Operation::name, std::move(a), std::move(b)); \
}

namespace StGenGlobal
{
    AbstractBuilder getBuilder();
    void setBuilder(AbstractBuilder builder);

    template <typename... Args>
    SelectQuery select(Args ... args)
    {
        return getBuilder()->select(ColumnsQuery(conv(args...)));
    }

    template <typename... Args>
    InsertQuery insert(Args ... args)
    {
        return getBuilder()->insert(ColumnsQuery(conv(args...)));
    }

    QString bind(const QString &id);
    QString alias(const QString &name, const QString& newName);
    QString distinct(const QString &column);
    QStringList autoincrement();
    QStringList unique();
    QStringList notNull();

    enum class Default
    {
        Date
    };

    QStringList defaultValue(const Default value);
    QStringList defaultValue(const QString& value);

    CreateTableQuery createTable(QString name);

    SimpleTypeOperation(equal)
    SimpleTypeOperation(notEqual)
    SimpleTypeOperation(less)
    SimpleTypeOperation(lessEqual)
    SimpleTypeOperation(more)
    SimpleTypeOperation(moreEqual)

    AbsExprPointer andE(AbsExprPointer a, AbsExprPointer b);

    AbsExprPointer orE(AbsExprPointer a, AbsExprPointer b);
}

#endif // STGENGLOBAL_H
