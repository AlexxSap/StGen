#ifndef STGENGLOBAL_H
#define STGENGLOBAL_H

#include "StGen.h"
#include "stable.h"

#define SimpleTypeOperation(name) template <typename A, typename B> \
AbsExpr name(A a, B b, NOT_IS(AbstractExpression, A), NOT_IS(AbstractExpression, B)) \
{ \
    return Expr::create(AbstractExpression::Operation::name, std::move(a), std::move(b)); \
}


namespace StGenGlobal
{
    AbstractBuilder sqlBuilder = nullptr;

    template <typename... Args>
    SelectQuery select(Args ... args)
    {
        return sqlBuilder->select(ColumnsQuery(conv(args...)));
    }

    SimpleTypeOperation(equal)
    SimpleTypeOperation(notEqual)
    SimpleTypeOperation(less)
    SimpleTypeOperation(lessEqual)
    SimpleTypeOperation(more)
    SimpleTypeOperation(moreEqual)

}

#endif // STGENGLOBAL_H
