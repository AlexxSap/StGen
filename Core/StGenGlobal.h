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

    template <typename... Args>
    QString sum(Args ... args)
    {
        return convVar(args...).toStringList().join(" + ");
    }


    QString bind(const QString &id);
    QString alias(const QString &name,
                  const QString& newName);
    QString column(const QString &table,
                   const QString& column,
                   const QString& newName = QString());
    QString distinct(const QString &column);
    QStringList autoincrement();
    QStringList unique();
    QStringList notNull();

    class Sensitive
    {
    public:
        enum Type
        {
            NotSet,
            All,
            Distinct
        };

        static QString typeToString(const Type& t);
    };

    QString count(const QString &column,
                  Sensitive::Type sensitive = Sensitive::NotSet);

    QString max(const QString &expr,
                Sensitive::Type sensitive = Sensitive::NotSet);
    QString min(const QString &expr,
                Sensitive::Type sensitive = Sensitive::NotSet);

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

    AbsExprPointer in(QString name, QVariantList values);
    AbsExprPointer notIn(QString name, QVariantList values);
    AbsExprPointer andE(AbsExprPointer a, AbsExprPointer b);
    AbsExprPointer orE(AbsExprPointer a, AbsExprPointer b);
}

#endif // STGENGLOBAL_H
