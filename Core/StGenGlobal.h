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

    QString bind(const QString &id);
    QString distinct(const QString &column);

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
