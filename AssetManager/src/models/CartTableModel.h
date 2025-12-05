#pragma once

#include <QAbstractTableModel>
#include <QObject>
#include <QVector>
#include <QMap>

struct CartLine {
    int itemId;
    QString name;
    int quantity{1};
};

class CartTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum Column {
        Name = 0,
        Quantity,
        COLUMN_COUNT
    };

    explicit CartTableModel(QObject *parent = nullptr);
    ~CartTableModel() override = default;

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;

    void addItem(int itemId, const QString& name);
    void setQuantity(int itemId, int qty);
    void removeItem(int itemId);
    void clear();
    bool contains(int itemId) const;
    int totalLines() const;
    QVector<CartLine> lines() const;

    void setItems(const QVector<CartLine>& items);

signals:
    void cartChanged();

private:
    int findRowByItemId(int itemId) const;

    QVector<CartLine> m_lines;
    QMap<int, int> m_indexByItemId;  // maps itemId -> row
};
