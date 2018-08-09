#include "stdafx.h"
#include "TreeView.h"
#include "Common.h"
#include "MyDatabase.h"
#include "MyTipsBox.h"
#include <QFileDialog>
#include "CommonSetting.h"

extern QString	gLastDir;
QString gCurrentTreeItemPath = "";

const int const_treeview_width = 290;
const int const_fontbtn_width_height = 27;
const int const_searchbtn_width_height = 18;
const int const_toolbtn_width = 286;
const int const_toolbtn_height = 32;
const int const_toolpanel_height = 41;
const int const_combo_width = 65;
const int const_combo_height = 28;
const int const_smallbtn_width = 18;
const int const_smallbtn_height = 18;
const int const_videotree_item_height = 20;

extern MyDatabase mDb;

TreeView::TreeView(QWidget *parent)
	: QWidget(parent),
	mSelTreeItem(NULL),
	mPlayingItem(NULL)
{
	ui.setupUi(this);
	Init();
	InitSignalSlots();
}

TreeView::~TreeView()
{
}

void TreeView::Init()
{
	m_pVideoTree = new ListTreeWidget;
	QHBoxLayout *hLayout = new QHBoxLayout;
	hLayout->addWidget(m_pVideoTree);
	hLayout->setContentsMargins(0, 0, 0, 0);
	ui.playlist_tree->setLayout(hLayout);
	m_pVideoTree->setFixedWidth(const_treeview_width);
	m_pVideoTree->setHeaderHidden(true);
	m_pVideoTree->setIndentation(0);
	m_pVideoTree->AddGroup(QStringLiteral("本地媒体文件"));
}

void TreeView::InitUi()
{
	setWindowFlags(Qt::FramelessWindowHint);

	setFixedWidth(const_treeview_width);
	ui.widget->setFixedHeight(const_fontbtn_width_height + 3);
	ui.set_font_btn->setFixedSize(const_fontbtn_width_height, const_fontbtn_width_height);
	ui.search_btn->setFixedSize(const_searchbtn_width_height, const_searchbtn_width_height);
	ui.search_edit->setFixedHeight(const_fontbtn_width_height-5);
	ui.playlist_btn->setFixedSize(const_toolbtn_width, const_toolbtn_height);
	ui.playlist_tree_2->setFixedSize(const_toolbtn_width, const_toolpanel_height);
	ui.listtype_combo->setFixedSize(const_combo_width, const_combo_height);
	ui.playmode_btn->setFixedSize(const_smallbtn_width, const_smallbtn_height);
	ui.addfile_btn->setFixedSize(const_smallbtn_width, const_smallbtn_height);
	ui.delfile_btn->setFixedSize(const_smallbtn_width, const_smallbtn_height);
	ui.playlist_tree->setFixedWidth(const_toolbtn_width);

	ui.addfile_btn->setToolTip(QStringLiteral("添加文件"));
	ui.delfile_btn->setToolTip(QStringLiteral("删除文件"));

	ui.set_font_btn->setStyleSheet(
		"QToolButton{border-image: url(:/images/btn_font_normal.png);}"
		"QToolButton:hover{border-image: url(:/images/btn_font_hover.png);}"
		"QToolButton:pressed{border-image: url(:/images/btn_font_press.png);}"
		);
	ui.search_btn->setStyleSheet(
		"QToolButton{border-image: url(:/images/btn_search_normal.png);}"
		"QToolButton:hover{border-image: url(:/images/btn_search_hover.png);}"
		"QToolButton:pressed{border-image: url(:/images/btn_search_press.png);}"
		);
	ui.playlist_btn->SetUiStyleSheet("QToolButton#playlist_btn{border-image:url(:/images/treeview_header.png);color: rgb(233, 233, 233); font-size: 75 16px;}",
		"QToolButton#playlist_btn{border-image:url(:/images/treeview_header_expand.png);color: rgb(233, 233, 233); font-size: 75 16px;}", ENUM_TOOLBUTTON_SELECT);
	ui.playmode_btn->setStyleSheet(
		"QToolButton{border-image: url(:/images/btn_play_mode_normal.png);}"
		"QToolButton:hover{border-image: url(:/images/btn_play_mode_hover.png);}"
		"QToolButton:pressed{border-image: url(:/images/btn_play_mode_press.png);}"
		);
	ui.addfile_btn->setStyleSheet(
		"QToolButton{border-image: url(:/images/btn_add_normal.png);}"
		"QToolButton:hover{border-image: url(:/images/btn_add_hover.png);}"
		"QToolButton:pressed{border-image: url(:/images/btn_add_press.png);}"
		);
	ui.delfile_btn->setStyleSheet(
		"QToolButton{border-image: url(:/images/btn_delete_normal.png);}"
		"QToolButton:hover{border-image: url(:/images/btn_delete_hover.png);}"
		"QToolButton:pressed{border-image: url(:/images/btn_delete_press.png);}"
		);
	ui.listtype_combo->setStyleSheet(
		"QComboBox{border: 0px solid gray; padding: 1px 18px 1px 3px;}"
		"QComboBox:!editable, QComboBox::drop-down:!editable{background: rgba(25,25,25);}"
		"QComboBox:!editable:on, QComboBox::drop-down:!editable:on{background: rgba(47,47,47);}"
		"QComboBox::down-arrow{border-image: url(:/images/combo_right_normal.png);width:20; height:20;}"
		"QComboBox::down-arrow:hover{border-image: url(:/images/combo_right_hover.png);width:20; height:20;}"
		"QComboBox::down-arrow:pressed{border-image: url(:/images/combo_right_press.png);width:20; height:20;}"
		"QComboBox::drop-down{width: 20px; height: 20px;}"
		"QComboBox{color: white;}"
		"QComboBox QAbstractItemView{selection-color: white; background: rgba(32,32,32,60);}"
		"QComboBox QAbstractItemView{color: white; border: 1px solid darkgray; selection-background-color: rgb(64, 64, 64);}");
	ui.search_edit->setStyleSheet("QLineEdit{color: rgb(75,79,81); font: 75 14px; border: 0px solid rgb(13, 65, 119);border - radius: 3px;background:rgb(36,37,39);selection-background-color: rgb(36,37,39);}"
		"QLineEdit:hover{border: 1px solid rgb(13, 65, 119);}");

	QPalette pal(palette());
	pal.setColor(QPalette::Background, QColor(qRgb(32, 32, 32)));
	setAutoFillBackground(true);
	setPalette(pal);

	ui.playlist_btn->setText(QStringLiteral("播放列表"));
	ui.search_edit->setPlaceholderText(QStringLiteral("请输入搜索关键词"));
	QStringList typecomboList;
	ui.listtype_combo->clear();
	typecomboList << QStringLiteral("全部") /*<< QStringLiteral("在线") */<< QStringLiteral("本地");
	ui.listtype_combo->addItems(typecomboList);

	ui.playmode_btn->setVisible(false);
}

void TreeView::InitSignalSlots()
{
	connect(ui.set_font_btn, SIGNAL(clicked()), this, SLOT(slot_click_setfont_btn()));
	connect(ui.search_btn, SIGNAL(clicked()), this, SLOT(slot_click_search_btn()));
	connect(ui.listtype_combo, SIGNAL(clicked()), this, SLOT(slot_click_filetype_btn()));
	connect(ui.playmode_btn, SIGNAL(clicked()), this, SLOT(slot_click_playmode_btn()));
	connect(ui.addfile_btn, SIGNAL(clicked()), this, SLOT(slot_click_addfile_btn()));
	connect(ui.delfile_btn, SIGNAL(clicked()), this, SLOT(slot_click_delfile_btn()));
	connect(ui.playlist_btn, SIGNAL(clicked()), this, SLOT(slot_click_playlist_btn()));

	connect(m_pVideoTree, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(slot_treeItemClicked(QTreeWidgetItem *, int)));
	connect(m_pVideoTree, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(slot_treeItemDbClicked(QTreeWidgetItem *, int)));
}

void TreeView::LoadVideoList()
{
	QString sqlCmd = QString("select * from Video");
	if (mDb.ExecuteSql(sqlCmd))
	{
		mVecVideoList.clear();

		QSqlQuery sq;
		mDb.GetSq(sq);
		int datePlanId = -1;
		while (sq.next())
		{
			VIDEO_TREE_ITEM videoTree;
			videoTree.videoId = std::string(sq.value(1).toString().toLocal8Bit().data());
			videoTree.videoName = std::string(sq.value(2).toString().toLocal8Bit().data());
			videoTree.videoFilePath = std::string(sq.value(3).toString().toLocal8Bit().data());
			videoTree.videoPlayTimeStamp = sq.value(4).toInt();
			videoTree.setId = std::string(sq.value(5).toString().toLocal8Bit().data());
			mVecVideoList.push_back(videoTree);
		}
	}
	RefreshUi();
}

void TreeView::RefreshUi()
{
	if (mVecVideoList.size() > 0)
	{
		vector<VIDEO_TREE_ITEM>::iterator it;
		for (it = mVecVideoList.begin(); it != mVecVideoList.end(); it++)
		{
			m_pVideoTree->AddMember(0, QString::fromLocal8Bit(it->videoName.c_str()), *it);
		}
	}
	if (m_pVideoTree)
		m_pVideoTree->expandAll();
	VideoTreeItem *itemSel = (VideoTreeItem *)m_pVideoTree->topLevelItem(0)->child(0);
	if (itemSel)
	{
		itemSel->GetIpcData(mSelVideoItem);
		mCurIndexOfTree = 0;
		mSelTreeItem = m_pVideoTree->topLevelItem(0)->child(0);
	}
	else
	{
		memset(&mSelVideoItem, 0, sizeof(VIDEO_TREE_ITEM));
		mCurIndexOfTree = -1;
		mSelTreeItem = NULL;
	}
}

bool TreeView::FindItem(QString filePath)
{
	vector<VIDEO_TREE_ITEM>::iterator it;
	for (it = mVecVideoList.begin(); it != mVecVideoList.end(); it++)
	{
		QString filePath2 = QString::fromLocal8Bit(it->videoFilePath.c_str());
		if (filePath2.compare(filePath) == 0)
			return true;
	}
	return false;
}

void TreeView::AddVideoItem(VIDEO_TREE_ITEM videoInfo)
{
	if (!FindItem(QString::fromLocal8Bit(videoInfo.videoFilePath.c_str())))
	{
		m_pVideoTree->AddMember(0, QString::fromLocal8Bit(videoInfo.videoName.c_str()), videoInfo);
		SetCurrentItem(m_pVideoTree->topLevelItem(0)->childCount()-1);
		QString sqlDb = QString("insert into Video(id, videoId,videoName, videoFilePath, videoPlayTimeStamp, setId)");
		sqlDb += QString("values(%1, '%2', '%3', '%4', %5,'%6')").arg(mVecVideoList.size()).arg(QString::fromLocal8Bit(videoInfo.videoId.c_str())).\
			arg(QString::fromLocal8Bit(videoInfo.videoName.c_str())).arg(QString::fromLocal8Bit(videoInfo.videoFilePath.c_str())).\
			arg(videoInfo.videoPlayTimeStamp).arg(QString::fromLocal8Bit(videoInfo.setId.c_str()));
		mDb.ExecuteSql(sqlDb);
	}
	else
	{
		if (m_pVideoTree)
		{
			for (int i = 0; i < m_pVideoTree->topLevelItem(0)->childCount(); i++)
			{
				QTreeWidgetItem* item = m_pVideoTree->topLevelItem(0)->child(i);
				if (item)
				{
					VideoTreeItem* truelyItem = (VideoTreeItem*)item;
					if (truelyItem)
					{
						VIDEO_TREE_ITEM itemParam;
						truelyItem->GetIpcData(itemParam);
						if (videoInfo.videoFilePath.compare(itemParam.videoFilePath) == 0)
						{
							SetCurrentItem(i);
							break;
						}
					}
				}
			}
		}
	}
}

int TreeView::GetCurrentItemIndex()
{
	int curIdx = m_pVideoTree->currentIndex().row();
	if (curIdx != mCurIndexOfTree)
		return mCurIndexOfTree;
	return curIdx;
}

VIDEO_TREE_ITEM TreeView::GetNextItem()
{
	VIDEO_TREE_ITEM videoItem;
	if (m_pVideoTree && m_pVideoTree->topLevelItem(0)->childCount() > mCurIndexOfTree+1)
	{
		QTreeWidgetItem* item = m_pVideoTree->topLevelItem(0)->child(mCurIndexOfTree+1);
		VideoTreeItem* item2 = (VideoTreeItem*)item;
		if (item2)
		{
			item2->GetIpcData(videoItem);
		}
	}
	return videoItem;
}

VIDEO_TREE_ITEM TreeView::GetPreviousItem()
{
	VIDEO_TREE_ITEM videoItem;
	if (m_pVideoTree && mCurIndexOfTree>0)
	{
		QTreeWidgetItem* item = m_pVideoTree->topLevelItem(0)->child(mCurIndexOfTree - 1);
		VideoTreeItem* item2 = (VideoTreeItem*)item;
		if (item2)
		{
			item2->GetIpcData(videoItem);
		}
	}
	return videoItem;
}

void TreeView::SetCurrentItem(int itemIndex)
{
	if (m_pVideoTree && itemIndex < m_pVideoTree->topLevelItem(0)->childCount())
	{
		QTreeWidgetItem* item = m_pVideoTree->topLevelItem(0)->child(itemIndex);
		VideoTreeItem* videoItem = (VideoTreeItem*)item;
		if (videoItem && mSelTreeItem && item && m_pVideoTree)
		{
			mSelTreeItem->setSelected(false);
			videoItem->GetIpcData(mSelVideoItem);
			mCurIndexOfTree = itemIndex;
			item->setSelected(true);
			mSelTreeItem = item;
			m_pVideoTree->scrollToItem(item);
		}
	}
}

void TreeView::SetPlayingColor(QTreeWidgetItem* item)
{
	return;
	if (item && mPlayingItem)
	{
		if (mPlayingItem == item)
			return;

		QColor PlayingColor(qRgb(255,255,255));
		QColor normalColor(qRgb(25,25,230));
		mPlayingItem->setTextColor(0, PlayingColor);
		item->setTextColor(0, normalColor);
	}
}

void TreeView::slot_click_setfont_btn()
{
	
}

void TreeView::slot_click_search_btn()
{
	QString keywords = ui.search_edit->text();
	keywords = keywords.trimmed();
	if (keywords.isEmpty())
	{
		for (int i = 0; i < m_pVideoTree->topLevelItem(0)->childCount(); i++)
		{
			QTreeWidgetItem* item = m_pVideoTree->topLevelItem(0)->child(i);
			if (item)
			{
				item->setHidden(false);
			}
		}
	}
	else
	{
		for (int i = 0; i < m_pVideoTree->topLevelItem(0)->childCount(); i++)
		{
			QTreeWidgetItem* item = m_pVideoTree->topLevelItem(0)->child(i);
			if (item)
			{
				QString text = item->text(0);
				if (!text.contains(keywords))
				{
					item->setHidden(true);
				}
				else
				{
					item->setHidden(false);
				}
			}
		}
	}
}

void TreeView::slot_click_filetype_btn()
{

}

void TreeView::slot_click_playmode_btn()
{

}

void TreeView::slot_click_addfile_btn()
{
	QStringList fileList = QFileDialog::getOpenFileNames(this, QStringLiteral("打开"), gLastDir, QStringLiteral("媒体文件（所有文件）") + ";*.*");
	if (fileList.empty())
	{
		return;
	}

	QString dirPath = fileList.at(0);
	QFileInfo fileDev(dirPath);
	dirPath = fileDev.dir().path();
	gLastDir = dirPath;

	int videoIdStart = mVecVideoList.size();
	for (int i = 0; i < fileList.size(); i++)
	{
		QString filePath = fileList.at(i);

		VIDEO_TREE_ITEM videoInfo;
		QFileInfo fileInfo(filePath);
		QString fileName = fileInfo.fileName();

		QString videoIdQ = "video_" + QString::number(videoIdStart);
		QString videoSet = "videoset_0";
		videoInfo.videoId = std::string(videoIdQ.toLocal8Bit().data());
		videoInfo.videoName = std::string(fileName.toLocal8Bit().data());
		videoInfo.videoFilePath = std::string(filePath.toLocal8Bit().data());
		videoInfo.videoPlayTimeStamp = 0;
		videoInfo.setId = std::string(videoSet.toLocal8Bit().data());

		AddVideoItem(videoInfo);
		mVecVideoList.push_back(videoInfo);
	}
	if (m_pVideoTree)
		m_pVideoTree->expandAll();
}

void TreeView::slot_click_delfile_btn()
{
	QList<QTreeWidgetItem*> listSelectedItems = m_pVideoTree->selectedItems();
	if (listSelectedItems.size() == 0)
	{
		/*MyTipsBox::showMessage(QStringLiteral("操作错误"),
			QStringLiteral("请选择一组要删除的方案"),
			this, QStringLiteral("提示"));*/
		return;
	}

	for (int i = 0; i < listSelectedItems.size(); i++)
	{
		QTreeWidgetItem* item = listSelectedItems.at(i);
		if (item->parent() == NULL)
			continue;

		if (item)
		{
			VideoTreeItem *arItem = (VideoTreeItem*)item;
			//先删除数据库
			VIDEO_TREE_ITEM itemParams;
			if (arItem)
				arItem->GetIpcData(itemParams);
			QString sqlCmd = QString("delete from Video where videoFilePath='%1'").arg(QString::fromLocal8Bit(itemParams.videoFilePath.c_str()));
			mDb.ExecuteSql(sqlCmd);

			//再删除列表
			m_pVideoTree->topLevelItem(0)->takeChild(m_pVideoTree->currentIndex().row());


			//删除缓存
			/*vector<VIDEO_TREE_ITEM>::iterator it;
			for (it = mVecVideoList.begin(); it != mVecVideoList.end(); it++)
			{
				if (it->videoFilePath.compare(itemParams.videoFilePath) == 0)
				{
					mVecVideoList.erase(it+i);
					break;
				}
			}*/
			for (int i = 0; i < mVecVideoList.size(); i++)
			{
				if (mVecVideoList[i].videoFilePath == itemParams.videoFilePath)
				{
					mVecVideoList.erase(mVecVideoList.begin() + i);
					i--;
				}
			}
		}
	}
}

void TreeView::slot_click_playlist_btn()
{
	ui.playlist_btn->SetState(ENUM_TOOLBUTTON_SELECT);
	static int clickIdx = 0;
	if (clickIdx % 2 != 0)
	{
		ui.playlist_tree->setVisible(true);
	}
	else
	{
		ui.playlist_tree->setVisible(false);
	}
	clickIdx++;
}

void TreeView::slot_treeItemClicked(QTreeWidgetItem* selItem, int column)
{
	QTreeWidgetItem* t_parentItem = selItem->parent();
	if (!t_parentItem)	return;

	VideoTreeItem *videoItem = (VideoTreeItem*)selItem;
	if (!videoItem)	return;

	mSelTreeItem = selItem;
	mCurIndexOfTree = m_pVideoTree->topLevelItem(0)->indexOfChild(selItem);
	videoItem->GetIpcData(mSelVideoItem);
	gCurrentTreeItemPath = QString::fromLocal8Bit(mSelVideoItem.videoFilePath.c_str());
}

void TreeView::slot_treeItemDbClicked(QTreeWidgetItem* selItem, int column)
{
	QTreeWidgetItem* t_parentItem = selItem->parent();
	if (!t_parentItem)	return;

	VideoTreeItem *videoItem = (VideoTreeItem*)selItem;
	if (!videoItem)	return;

	SetPlayingColor(selItem);

	mSelTreeItem = selItem;
	videoItem->GetIpcData(mSelVideoItem);

	emit signal_playvideo();
	mPlayingItem = selItem;
}

bool TreeView::eventFilter(QObject *obj, QEvent *event)
{
	int keyType = event->type();
	if (obj == ui.search_edit)
	{
		if (keyType == QEvent::KeyPress)
		{
			QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
			if (keyEvent->key() == Qt::Key_Enter)
			{
				slot_click_search_btn();
			}
		}
	}
	return false;
}

void TreeView::showEvent(QShowEvent *event)
{
	InitUi();
}