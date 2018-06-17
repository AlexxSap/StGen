#ifndef DATABASEINTERFACE_H
#define DATABASEINTERFACE_H

#include <QString>
#include <QSqlDatabase>

struct DataBaseSettings
{
    QString user;
    QString password;
    QString host;
    QString port;
};

using DataBaseName = QString;

class DataBaseInterface
{
public:
    DataBaseInterface(const DataBaseSettings& settings,
                      const bool autoClose = false);
    ~DataBaseInterface();

    bool open(const DataBaseName &dataBaseName);
    void close();

    virtual bool create(const DataBaseName &name) = 0;
    virtual bool remove(const DataBaseName &name) = 0;

private:
    virtual QString driverID() const = 0;

private:
    DataBaseSettings settings_;
    bool autoClose_ = false;
    QSqlDatabase db_;
};

class SqliteInterface : public DataBaseInterface
{
public:
    SqliteInterface(const DataBaseSettings& settings);

    virtual bool create(const DataBaseName &name) override;
    virtual bool remove(const DataBaseName &name) override;

private:
    virtual QString driverID() const override;
};

#endif // DATABASEINTERFACE_H
