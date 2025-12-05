#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include "PersonsDAO.h"
#include "src/core/DatabaseManager.h"

QVector<Person> PersonsDAO::getAll() const
{
    QVector<Person> result = {};

    QSqlQuery query(DatabaseManager::instance().db());
    query.prepare(R"(
        SELECT id, first_name, last_name
        FROM persons
        ORDER BY id
    )");

    if (!query.exec())
    {
        qDebug() << "[PERSONSDAO] Error getAll:" << query.lastError().text();
        return result;
    }
    while(query.next())
    {
        QSqlRecord record = query.record();
        result.append({record.value("id").toInt(),
                       record.value("first_name").toString(),
                       record.value("last_name").toString()});
    }
    return result;
}

Person PersonsDAO::getById(int id) const
{
    Person result = {};

    QSqlQuery query(DatabaseManager::instance().db());
    query.prepare(R"(
        SELECT id, first_name, last_name
        FROM persons
        WHERE id = :id
    )");
    query.bindValue(":id", id);

    if (!query.exec())
    {
        qDebug() << "[PERSONSDAO] Error in getById:" << query.lastError().text();
        return result;
    }
    if (query.next())
    {
        QSqlRecord record = query.record();
        result = {record.value("id").toInt(),
                  record.value("first_name").toString(),
                  record.value("last_name").toString()};
    }
    return result;
}

QString PersonsDAO::getFullNameById(int id) const
{
    QString result{};

    QSqlQuery query(DatabaseManager::instance().db());
    query.prepare(R"(
        SELECT first_name, last_name
        FROM persons
        WHERE id = :id
    )");
    query.bindValue(":id", id);

    if (!query.exec())
    {
        qDebug() << "[PERSONSDAO] Error in getFullNameById:" << query.lastError().text();
        return result;
    }
    if (query.next())
    {
        QSqlRecord record = query.record();
        QString firstName = record.value("first_name").toString();
        QString lastName = record.value("last_name").toString();
        result = QString("%1 %2").arg(firstName, lastName);
    }
    return result.trimmed();
}

bool PersonsDAO::insert(const QString& firstName, const QString& lastName)
{
    QSqlQuery query(DatabaseManager::instance().db());
    query.prepare(R"(
        INSERT INTO persons (first_name, last_name) VALUES (:first_name, :last_name)
    )");
    query.bindValue(":first_name", firstName);
    query.bindValue(":last_name", lastName);

    if (!query.exec())
    {
        qDebug() << "[PERSONSDAO] Error in insert:" << query.lastError().text();
        return false;
    }
    return true;
}

bool PersonsDAO::update(int id, const QString& firstName, const QString& lastName)
{
    if (id <= 0)
    {
        qDebug() << "[PERSONSDAO] update called with invalid id:" << id;
        return false;
    }

    QSqlQuery query(DatabaseManager::instance().db());
    query.prepare(R"(
        UPDATE persons
        SET first_name = :first_name, last_name = :last_name
        WHERE id = :id
    )");
    query.bindValue(":first_name", firstName);
    query.bindValue(":last_name", lastName);
    query.bindValue(":id", id);

    if (!query.exec())
    {
        qDebug() << "[PERSONSDAO] Error in update:" << query.lastError().text();
        return false;
    }
    return query.numRowsAffected() > 0;
}

bool PersonsDAO::remove(int id)
{
    if (id <= 0)
    {
        qDebug() << "[PERSONSDAO] remove called with invalid id:" << id;
        return false;
    }

    QSqlQuery query(DatabaseManager::instance().db());
    query.prepare(R"(
        DELETE FROM persons
        WHERE id = :id
    )");
    query.bindValue(":id", id);

    if (!query.exec())
    {
        qDebug() << "[PERSONSDAO] Error in remove:" << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}
