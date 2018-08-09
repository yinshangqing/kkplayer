#pragma once

#include <QWidget>
#include "ui_TreeView.h"
#include "Common.h"
#include "ListTreeWidget.h"

using namespace Kk;
using namespace std;

class TreeView : public QWidget
{
	Q_OBJECT

public:
	TreeView(QWidget *parent = Q_NULLPTR);
	~TreeView();

private:
	Ui::TreeView ui;

	ListTreeWidget *m_pVideoTree;
	vector<VIDEO_TREE_ITEM> mVecVideoList;
	VIDEO_TREE_ITEM mSelVideoItem;
	QTreeWidgetItem*	mSelTreeItem;
	QTreeWidgetItem*	mPlayingItem;
	int mCurIndexOfTree;

private:
	void Init();
	void InitUi();
	void InitSignalSlots();
	void RefreshUi();
	bool FindItem(QString filePath);
	void SetPlayingColor(QTreeWidgetItem* item);

public:
	void LoadVideoList();
	VIDEO_TREE_ITEM GetSelTreeItem(){ return mSelVideoItem; };
	VIDEO_TREE_ITEM GetNextItem();
	VIDEO_TREE_ITEM GetPreviousItem();
	int GetCurrentItemIndex();
	void SetCurrentItem(int itemIndex);
	void AddVideoItem(VIDEO_TREE_ITEM tvshowItemInfo);
	void GetVecVideoList(vector<VIDEO_TREE_ITEM> &vecVideoList){ vecVideoList = mVecVideoList; };
	void PushVideoItem(VIDEO_TREE_ITEM item){ mVecVideoList.push_back(item); };

signals:
	void signal_setfont();
	void signal_playvideo();

private slots:
	void slot_click_setfont_btn();
	void slot_click_search_btn();
	void slot_click_filetype_btn();
	void slot_click_playmode_btn();
	void slot_click_addfile_btn();
	void slot_click_delfile_btn();
	void slot_click_playlist_btn();

	void slot_treeItemClicked(QTreeWidgetItem* selItem, int column);
	void slot_treeItemDbClicked(QTreeWidgetItem* selItem, int column);

protected:
	bool eventFilter(QObject *obj, QEvent *event);
	void showEvent(QShowEvent *event);
};
