#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include "InstitutionsDAO.h"
#include "src/core/DatabaseManager.h"

QVector<Institution> InstitutionsDAO::getAll() const
{
    QVector<Institution> result = {};

    QSqlQuery query(DatabaseManager::instance().db());
    query.prepare(R"(
        SELECT id, name
        FROM institutions
    )");

    if (!query.exec())
    {
        qDebug() << "[INSTITUTIONSDAO] Error in getAll:" << query.lastError().text();
        return result;
    }
    while(query.next())
    {
        QSqlRecord record = query.record();
        result.append({record.value("id").toInt(), record.value("name").toString()});
    }
    return result;
}

Institution InstitutionsDAO::getById(int id) const
{
    Institution result = {};

    QSqlQuery query(DatabaseManager::instance().db());
    query.prepare(R"(
        SELECT id, name
        FROM institutions
        WHERE id = :id
    )");
    query.bindValue(":id", id);

    if (!query.exec())
    {
        qDebug() << "[INSTITUTIONSDAO] Error in getById:" << query.lastError().text();
        return result;
    }
    if (query.next())
    {
        QSqlRecord record = query.record();
        result = {record.value("id").toInt(), record.value("name").toString()};
    }
    return result;
}

QString InstitutionsDAO::getNameById(int id) const
{
    QString result{};

    QSqlQuery query(DatabaseManager::instance().db());
    query.prepare(R"(
        SELECT name
        FROM institutions
        WHERE id = :id
    )");
    query.bindValue(":id", id);

    if (!query.exec())
    {
        qDebug() << "[INSTITUTIONSDAO] Error in getNameById:" << query.lastError().text();
        return result;
    }
    if (query.next())
    {
        QSqlRecord record = query.record();
        result = record.value("name").toString();
    }
    return result;
}

bool InstitutionsDAO::insert(const QString& name)
{
    QSqlQuery query(DatabaseManager::instance().db());
    query.prepare(R"(
        INSERT INTO institutions (name) VALUES (:name)
    )");
    query.bindValue(":name", name);

    if (!query.exec())
    {
        qDebug() << "[INSTITUTIONSDAO] Error in insert:" << query.lastError().text();
        return false;
    }
    return true;
}

bool InstitutionsDAO::update(int id, const QString& name)
{
    if (id <= 0)
    {
        qDebug() << "[INSTITUTIONSDAO] update called with invalid id:" << id;
        return false;
    }

    QSqlQuery query(DatabaseManager::instance().db());
    query.prepare(R"(
        UPDATE institutions
        SET name = :name
        WHERE id = :id
    )");
    query.bindValue(":name", name);
    query.bindValue(":id", id);

    if (!query.exec())
    {
        qDebug() << "[INSTITUTIONSDAO] Error in update:" << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}

bool InstitutionsDAO::remove(int id)
{
    if (id <= 0)
    {
        qDebug() << "[INSTITUTIONSDAO] remove called with invalid id:" << id;
        return false;
    }

    QSqlQuery query(DatabaseManager::instance().db());
    query.prepare(R"(
        DELETE FROM institutions
        WHERE id = :id
    )");
    query.bindValue(":id", id);

    if (!query.exec())
    {
        qDebug() << "[INSTITUTIONSDAO] Error in remove:" << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}
