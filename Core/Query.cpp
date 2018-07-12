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

SelectQuery::SelectQuery()
    : BindedExecutableQuery<SelectQuery>(nullptr)
{

}

SelectQuery::SelectQuery(AbstractDataBaseInterface *base,
                         ColumnsQuery columns)
    : BindedExecutableQuery<SelectQuery>(base),
      columns_(std::move(columns))
{

}

SelectQuery &SelectQuery::from(QString table)
{
    setName(std::move(table));
    return *this;
}

SelectQuery &SelectQuery::having(HavingCase havingCond)
{
    havingExpr_ = std::move(havingCond);
    return *this;
}

SelectQuery &SelectQuery::orderBy(QStringList columns, Order::Type type)
{
    order_.set(std::move(columns), std::move(type));
    return *this;
}

SelectQuery &SelectQuery::orderBy(QString column, Order::Type type)
{
    order_.set(QStringList() << std::move(column), std::move(type));
    return *this;
}

SelectQuery &SelectQuery::groupBy(QStringList columns)
{
    group_.set(std::move(columns));
    return *this;
}

SelectQuery &SelectQuery::groupBy(QString column)
{
    group_.set(QStringList() << std::move(column));
    return *this;
}

bool SelectQuery::isEmpty() const
{
    return NamedTableQuery::isEmpty();
}

QString SelectQuery::toQueryString() const
{
    QString result = "select "
            + columns_.toQueryString()
            + " from " + NamedTableQuery::toQueryString();

    result += joinQuery_.toQueryString();
    result += WhereableQuery<SelectQuery>::toQueryString();
    result += group_.toQueryString();
    result += order_.toQueryString();
    result += havingExpr_.toQueryString();

    return result + ";";
}

JoinQuery &SelectQuery::innerJoin(QString tableName)
{
    joinQuery_.set(JoinQuery::Inner, tableName, this);
    return std::ref(joinQuery_);
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
    case Operation::in: return " in ";

    }

    return QString();
}

CreateTableQuery::CreateTableQuery(AbstractDataBaseInterface *base,
                                   QString name)
    : ExecutableQuery<CreateTableQuery>(base),
      NamedTableQuery(std::move(name))
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
            .arg(NamedTableQuery::toQueryString())
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
    : BindedExecutableQuery<InsertQuery>(base),
      columns_(std::move(columns))
{

}

InsertQuery &InsertQuery::into(QString tableName)
{
    setName(std::move(tableName));
    return *this;
}

InsertQuery &InsertQuery::values(Values values)
{
    values_ = std::move(values);
    return *this;
}

InsertQuery &InsertQuery::from(SelectQuery selectQuery)
{
    selectQuery_ = std::move(selectQuery);
    return *this;
}

QString InsertQuery::toQueryString() const
{
    QString value;
    if(!values_.isEmpty() && selectQuery_.isEmpty())
    {
        value = values_.toQueryString();
    }
    else if(values_.isEmpty() && !selectQuery_.isEmpty())
    {
        value = selectQuery_.toQueryString();
        value = value.mid(0, value.size() - 1);
    }

    return QString("insert into %1(%2) %3;").arg(NamedTableQuery::toQueryString())
            .arg(columns_.toQueryString())
            .arg(value);
}

Values &Values::operator <<(QVariantList values)
{
    values_ << std::move(values);
    return *this;
}

QString Values::toQueryString() const
{
    QStringList valuesList;
    foreach (const QVariantList& lst, values_)
    {
        QStringList values;
        foreach (const QVariant& var, lst)
        {
            const QString strVar(var.toString());
            if(var.type() == QVariant::String
                    && strVar.startsWith(":"))
            {
                values << strVar;
            }
            else
            {
                values << "'" + strVar + "'";
            }
        }
        valuesList << "(" + values.join(", ") + ")";
    }

    return "values" + valuesList.join(",");
}

bool Values::isEmpty() const
{
    return values_.isEmpty();
}

CommonCase::CommonCase()
{

}

CommonCase::CommonCase(AbsExprPointer expr)
    : expr_(expr)
{

}

QString CommonCase::toQueryString() const
{
    return expr_.isNull() ? QString() : expr_->toQueryString();
}

bool CommonCase::isEmpty() const
{
    return expr_.isNull() || expr_->isEmpty();
}

WhereCase::WhereCase()
    : CommonCase()
{

}

WhereCase::WhereCase(AbsExprPointer expr)
    : CommonCase(expr)
{

}

QString WhereCase::toQueryString() const
{
    if(isEmpty()) return QString();
    return " where " + CommonCase::toQueryString();
}

JoinQuery::JoinQuery()
{

}

JoinQuery::JoinQuery(JoinQuery::Type type,
                     QString table,
                     SelectQuery *selectQuery)
    : type_(std::move(type)), table_(std::move(table)), selectQuery_(selectQuery)
{

}

SelectQuery &JoinQuery::on(OnCase expr)
{
    case_ = expr;
    return *selectQuery_;
}

void JoinQuery::set(JoinQuery::Type type,
                    QString table,
                    SelectQuery *selectQuery)
{
  type_ = std::move(type);
  table_ = std::move(table);
  selectQuery_ = selectQuery;
}

bool JoinQuery::isEmpty() const
{
    return table_.isEmpty();
}

QString JoinQuery::toQueryString() const
{
    if(isEmpty()) return QString();

    return QString(" %1 join %2%3")
            .arg(typeToStr())
            .arg(table_)
            .arg(case_.toQueryString());
}

QString JoinQuery::typeToStr() const
{
    switch (type_) {
    case Inner: return "inner";
    case Left: return "left";
    case Right: return "right";
    }

    return QString();
}

OnCase::OnCase()
: CommonCase()
{

}

OnCase::OnCase(AbsExprPointer expr)
: CommonCase(expr)
{

}

QString OnCase::toQueryString() const
{
    return " on " + CommonCase::toQueryString();
}

InExpression::InExpression(QString name, QVariantList values, bool isNot)
    : AbstractExpression(),
      name_(name),
      isNot_(isNot)
{
    foreach (const QVariant &value, values)
    {
        values_ << std::move(value.toString());
    }
}

QString InExpression::toQueryString() const
{
    return name_ + (isNot_ ? " not" : "") + " in (" + values_.join(", ") + ")";
}

bool InExpression::isEmpty() const
{
    return values_.isEmpty();
}

HavingCase::HavingCase()
    : CommonCase()
{

}

HavingCase::HavingCase(AbsExprPointer expr)
    : CommonCase(expr)
{

}

QString HavingCase::toQueryString() const
{
    if(isEmpty()) return QString();
    return " having " + CommonCase::toQueryString();
}



UpdateQuery::UpdateQuery(AbstractDataBaseInterface *base, QString tableName)
    : ExecutableQuery<UpdateQuery>(base),
      NamedTableQuery(std::move(tableName))
{

}

UpdateQuery &UpdateQuery::set(QString colName, QVariant value)
{
    columns_ << qMakePair(std::move(colName), std::move(value));
    return *this;
}

QString UpdateQuery::toQueryString() const
{
    QString result("update %1 set %2");

    QStringList sets;
    foreach (auto& column, columns_)
    {
        sets << column.first + " = " + variantToString(column.second);
    }

    result = result.arg(NamedTableQuery::toQueryString()).arg(sets.join(", ")) ;
    result += WhereableQuery<UpdateQuery>::toQueryString();

    return result + ";";
}

DeleteFromTableQuery::DeleteFromTableQuery(AbstractDataBaseInterface *base, QString table)
    : ExecutableQuery<DeleteFromTableQuery>(base),
      NamedTableQuery(std::move(table))
{

}

QString DeleteFromTableQuery::toQueryString() const
{
    QString result("delete from ");

    result += NamedTableQuery::toQueryString();
    result += WhereableQuery<DeleteFromTableQuery>::toQueryString();

    return result + ";";
}



NamedTableQuery::NamedTableQuery(QString name)
    : name_(std::move(name))
{}

void NamedTableQuery::setName(QString name)
{
    name_ = std::move(name);
}

bool NamedTableQuery::isEmpty() const
{
    return name_.isEmpty();
}

QString NamedTableQuery::toQueryString() const
{
    return name_;
}

Order::Order() : AbstractQuery() {}

void Order::set(QStringList columns, Order::Type t)
{
    columns_ = std::move(columns);
    type_ = std::move(t);
}

QString Order::toQueryString() const
{
    if(columns_.isEmpty())
    {
        return QString();
    }

    QString type;
    switch (type_) {
    case None: type = ""; break;
    case Asc: type = " asc"; break;
    case Desc: type = " desc"; break;
    }

    return " order by " + columns_.join(", ") + type;
}

void Group::set(QStringList columns)
{
    columns_ = std::move(columns);
}

QString Group::toQueryString() const
{
    if(columns_.isEmpty())
    {
        return QString();
    }

    return " group by " + columns_.join(", ");
}
