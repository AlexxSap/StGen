#include <StGenGlobal.h>

AbstractBuilder sqlBuilder;

CreateTableQuery StGenGlobal::createTable(QString name)
{
    return sqlBuilder->createTable(std::move(name));
}

QString StGenGlobal::bind(const QString &id)
{
    return ":" + id;
}

AbsExprPointer StGenGlobal::andE(AbsExprPointer a, AbsExprPointer b)
{
    return ExprPointer::create(AbstractExpression::Operation::andE, std::move(a), std::move(b));
}

AbsExprPointer StGenGlobal::orE(AbsExprPointer a, AbsExprPointer b)
{
    return ExprPointer::create(AbstractExpression::Operation::orE, std::move(a), std::move(b));
}

void StGenGlobal::setBuilder(AbstractBuilder builder)
{
    sqlBuilder = builder;
}

AbstractBuilder StGenGlobal::getBuilder()
{
    return sqlBuilder;
}

QString StGenGlobal::distinct(const QString &column)
{
    return "distinct " + column;
}

QStringList StGenGlobal::autoincrement()
{
    return QStringList() << "autoincrement";
}

QStringList StGenGlobal::unique()
{
    return QStringList() << "unique";
}

QStringList StGenGlobal::notNull()
{
    return QStringList() << "not null";
}

QStringList StGenGlobal::defaultValue(const Default value)
{
    QString strValue;
    switch (value) {
    case Default::Date: strValue = "current_date"; break;
    }

    return QStringList() << QString("default %1").arg(strValue);
}

QStringList StGenGlobal::defaultValue(const QString &value)
{
    return QStringList() << QString("default '%1'").arg(value);
}

QString StGenGlobal::alias(const QString &name, const QString &newName)
{
    if(name.isEmpty() || newName.isEmpty())
    {
        return name;
    }
    return name + " as " + newName;
}

QString StGenGlobal::column(const QString &table,
                            const QString &column,
                            const QString &newName)
{
    return table + "." + column + (newName.isEmpty() ? "" : " as " + newName);
}

AbsExprPointer StGenGlobal::in(QString name,
                               QVariantList values)
{
    return InExprPointer::create(std::move(name),
                                 std::move(values),
                                 false);
}

AbsExprPointer StGenGlobal::notIn(QString name, QVariantList values)
{
    return InExprPointer::create(std::move(name),
                                 std::move(values),
                                 true);
}

QString StGenGlobal::count(const QString &column)
{
    return QString("count(%1)").arg(column);
}
