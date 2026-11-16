
#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>

class Database
{
public:
    Database();

    bool open();
    bool initialize();
    QSqlDatabase connection() const;

private:
    QSqlDatabase db;
};
#endif
