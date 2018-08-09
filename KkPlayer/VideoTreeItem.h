#ifndef VIDEOTREEITEM_H
#define VIDEOTREEITEM_H

#include <QTreeWidgetItem>
#include "Common.h"

using namespace Kk;

class VideoTreeItem : public QTreeWidgetItem
{
public:
	VideoTreeItem(QTreeWidgetItem *parent);
	VideoTreeItem(QTreeWidgetItem *parent, const QStringList &strings, int type = Type);
	~VideoTreeItem();

	void SetIpcData(VIDEO_TREE_ITEM itemData);
	void GetIpcData(VIDEO_TREE_ITEM &itemData);

private:
	//IPC����
	VIDEO_TREE_ITEM	mItemData;
};

#endif