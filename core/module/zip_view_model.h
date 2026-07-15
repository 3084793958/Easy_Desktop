#ifndef ZIP_VIEW_MODEL_H
#define ZIP_VIEW_MODEL_H

#include <QStandardItemModel>

class Zip_View_Model : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit Zip_View_Model(QObject *parent = nullptr);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
};

#endif // ZIP_VIEW_MODEL_H
