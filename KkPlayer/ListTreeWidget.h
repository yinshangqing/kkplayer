/*
创建人：袁晓东;
日期：2017/06/29;
功能：列表;
*/

#ifndef JM_LISTTREEWIDGET_H
#define JM_LISTTREEWIDGET_H

#include <QFont>
#include <QMenu>
#include <QAction>
#include <QDebug>
#include <QTextCodec>
#include <QScrollBar>
#include <QToolButton>
#include <QTreeWidget>
#include <QHeaderView>
#include <QTreeWidgetItem>
#include <QWidget>
#include "SelfdefinedScrollbar.h"
#include "Common.h"
#include "VideoTreeItem.h"

class ListTreeWidget: public QTreeWidget
{
	Q_OBJECT

public:
	ListTreeWidget();
	//添加组;
	void AddGroup(QString GroupName);
	//添加成员;
	void AddMember(int GroupOrder, QString MemberName, VIDEO_TREE_ITEM itemData);
};

#endif