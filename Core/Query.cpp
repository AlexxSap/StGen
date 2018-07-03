#include "Query.h"


AbstractQuery::~AbstractQuery()
{

}

ColumnsQuery::ColumnsQuery()
    : AbstractQuery()
{

}

ColumnsQuery::ColumnsQuery(const QStringList &columns)
    : AbstractQuery(),
      columns_(columns)
{
}

int ColumnsQuery::size() const
{
    return columns_.size();
}

QString ColumnsQuery::toQueryString() const
{
    return columns_.isEmpty() ? "*" : columns_.join(", ");
}

FromQuery::FromQuery()
    : AbstractQuery()
{

}

FromQuery::FromQuery(const QString &tableName)
    : AbstractQuery(),
      table_(tableName)
{

}

FromQuery::FromQuery(const char *tableName)
    : AbstractQuery(),
      table_(tableName)
{

}

QString FromQuery::toQueryString() const
{
    return QString(" from %1").arg(table_);
}

bool FromQuery::isEmpty() const
{
    return table_.isEmpty();
}

SelectQuery::SelectQuery()
    : AbstractExecuteQuery(nullptr)
{

}

SelectQuery::SelectQuery(AbstractDataBaseInterface *base,
                         ColumnsQuery columns)
    : AbstractExecuteQuery(base),
      columns_(std::move(columns))
{

}

SelectQuery &SelectQuery::from(FromQuery table)
{
    from_ = std::move(table);
    return *this;
}

SelectQuery &SelectQuery::where(WhereCase whereCond)
{
    whereExpr_ = std::move(whereCond);
    return *this;
}

SelectQuery &SelectQuery::prepare()
{
    AbstractExecuteQuery::prepare();
    return *this;
}

void SelectQuery::bind(const QString &id, const QVariant &value)
{
    query_->bindValue(":" + id, value);
}

bool SelectQuery::isEmpty() const
{
    return from_.isEmpty();
}

QString SelectQuery::toQueryString() const
{
    QString result = "select "
            + columns_.toQueryString()
            + from_.toQueryString();

    if(!whereExpr_.isEmpty())
    {
        result += whereExpr_.toQueryString();
    }

    return result+ ";";
}

AbstractExecuteQuery::AbstractExecuteQuery(AbstractDataBaseInterface *base)
    : base_(base)
{

}

QueryResult AbstractExecuteQuery::exec()
{
    if(query_.isNull())
    {
        prepare();
    }
    if(!query_->exec())
    {
        qWarning() << query_->lastError();
    }
    return QueryResult(query_);
}

SqlQuery AbstractExecuteQuery::query() const
{
    return base_->query();
}

void AbstractExecuteQuery::prepare()
{
    query_ = this->query();
    if(!query_->prepare(toQueryString()))
    {
        qWarning() << query_->lastError();
    }
}

WhereCase::WhereCase()
{

}

WhereCase::WhereCase(AbsExprPointer expr)
    : expr_(expr)
{

}

QString WhereCase::toQueryString() const
{
    if(expr_.isNull())
    {
        return QString();
    }
    return " where " + expr_->toQueryString();
}

bool WhereCase::isEmpty() const
{
    return expr_.isNull() || expr_->isEmpty();
}

ValueExpression::ValueExpression(QVariant value)
    : value_(std::move(value))
{

}

QString ValueExpression::toQueryString() const
{
    if(value_.type() == QVariant::Date)
    {
        return "'" + value_.toDate().toString("yyyy-MM-dd") + "'";
    }
    return value_.toString();
}

bool ValueExpression::isEmpty() const
{
    return value_.isNull();
}

Expression::Expression(Operation type, QVariant a, QVariant b)
    : type_(type),
      a_(ValueExprPointer::create(a)),
      b_(ValueExprPointer::create(b))
{

}

Expression::Expression(AbstractExpression::Operation type,
                      AbsExprPointer a,
                      AbsExprPointer b)
    : type_(type),
      a_(a),
      b_(b)
{

}

QString Expression::toQueryString() const
{
    return a_->toQueryString() + operationToString(type_) + b_->toQueryString();
}

bool Expression::isEmpty() const
{
    return a_.isNull() || b_.isNull();
}

QString AbstractExpression::operationToString(const Operation& type) const
{
    switch (type) {
    case Operation::equal: return " = ";
    case Operation::notEqual: return " <> ";
    case Operation::less: return " < ";
    case Operation::lessEqual: return " <= ";
    case Operation::more: return " > ";
    case Operation::moreEqual: return " >= ";
    case Operation::andE: return " and ";
    case Operation::orE: return " or ";

    }

    return QString();
}

CreateTableQuery::CreateTableQuery(AbstractDataBaseInterface *base,
                                   QString name)
    : AbstractExecuteQuery(base),
      tableName_(std::move(name))
{

}

CreateTableQuery &CreateTableQuery::addColumn(QString name,
                                              QString type,
                                              Flags flags)
{
    return addColumn(TableColumn(std::move(name), std::move(type), std::move(flags)));
}

CreateTableQuery &CreateTableQuery::addColumn(TableColumn column)
{
    columns_.append(std::move(column));
    return *this;
}

CreateTableQuery &CreateTableQuery::prepare()
{
    AbstractExecuteQuery::prepare();
    return *this;
}

QString CreateTableQuery::toQueryString() const
{
    QString result("create table if not exists %1 (%2);");

    QStringList lst;
    foreach (const TableColumn &column, columns_)
    {
        lst << column.toQueryString();
    }

    if(!primary_.isEmpty())
    {
        lst << "primary key(" + primary_.join(", ") + ")";
    }

    return result
            .arg(tableName_)
            .arg(lst.join(", "));
}

QString ColumnType::Integer()
{
    return QString("integer");
}

QString ColumnType::String(const int value)
{
    return QString("varchar(%1)").arg(value);
}

QString ColumnType::Date()
{
    return "date";
}

TableColumn::TableColumn(QString name,
                         QString type,
                         Flags flags)
    : name_(std::move(name)),
      type_(std::move(type)),
      flags_(std::move(flags))
{

}

QString TableColumn::toQueryString() const
{
    return name_ + " "
            + type_
            + (flags_.isEmpty()
               ? ""
               : (" " + flags_.join(" ")));
}

InsertQuery::InsertQuery(AbstractDataBaseInterface *base,
                         ColumnsQuery columns)
    : AbstractExecuteQuery(base),
      columns_(std::move(columns))
{

}

InsertQuery &InsertQuery::into(QString tableName)
{
    tableName_ = tableName;
    return *this;
}

InsertQuery &InsertQuery::from(SelectQuery selectQuery)
{
    selectQuery_ = std::move(selectQuery);
    return *this;
}

/// TODO вынести значения из values_ в отдельный класс
QString InsertQuery::toQueryString() const
{
    const QString pattern("insert into %1(%2) %3;");

    QString value;
    if(!values_.isEmpty() && selectQuery_.isEmpty())
    {
        const QString valuesPattern("(%1)");

        QStringList valuesList;
        foreach (const QVariantList& lst, values_)
        {
            QStringList values;
            foreach (const QVariant& var, lst)
            {
                values << "'" + var.toString() + "'";
            }
            valuesList << valuesPattern.arg(values.join(", "));
        }

        value = "values" + valuesList.join(",");
    }
    else if(values_.isEmpty() && !selectQuery_.isEmpty())
    {
        value = selectQuery_.toQueryString();
        value = value.mid(0, value.size() - 1);
    }

    return pattern.arg(tableName_)
            .arg(columns_.toQueryString())
            .arg(value);
}
