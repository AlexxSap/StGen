#ifndef DATABASEINTERFACE_H
#define DATABASEINTERFACE_H

#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>

struct DataBaseSettings
{
    DataBaseSettings(const QString &name) : base(name) {}
    QString base;
    QString user;
    QString password;
    QString host = "localhost";
    QString port;
};

using DataBaseName = QString;

enum class Bases
{
    SQLITE
};

class DataBaseInterface
{
public:
    DataBaseInterface(const DataBaseSettings& settings,
                      const bool autoClose = false);
    ~DataBaseInterface();

    bool open();
    void close();

    virtual bool create() = 0;
    virtual bool remove() = 0;
    virtual bool exists() = 0;

    QSqlQuery query() const;
    QString dataBaseName() const;

    Bases type() const;

private:
    virtual QString driverID() const = 0;

protected:
    DataBaseSettings settings_;
    QSqlDatabase db_;
    Bases type_;

private:
    bool autoClose_ = false;
};

class SqliteInterface : public DataBaseInterface
{
public:
    SqliteInterface(const DataBaseSettings& settings,
                    const bool autoClose = false);

    virtual bool create() override;
    virtual bool remove() override;
    virtual bool exists() override;

private:
    virtual QString driverID() const override;
};


#endif // DATABASEINTERFACE_H
