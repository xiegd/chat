#ifndef LISTITEMBASE_H
#define LISTITEMBASE_H
#include <QWidget>
#include "global.h"

class ListItemBase : public QWidget
{
    Q_OBJECT
public:
    explicit ListItemBase(QWidget *parent = nullptr);
    QSize sizeHint() const override {
        return QSize(0, 0); // 返回自定义的尺寸
    }

    void SetItemType(ListItemType itemType);

    ListItemType GetItemType();

private:
    ListItemType _itemType;

public slots:

signals:


};

#endif // LISTITEMBASE_H
