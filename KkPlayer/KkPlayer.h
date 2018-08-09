#pragma once

#include <QtWidgets/QWidget>
#include "ui_KkPlayer.h"
#include "Common.h"
#include "MyDatabase.h"
#include "SettingMenuWnd.h"
#include "CommonSetting.h"

using namespace Kk;

class KkPlayer : public QWidget
{
	Q_OBJECT

public:
	KkPlayer(QWidget *parent = Q_NULLPTR);
	~KkPlayer();

private:
	void Init();
	void InitUi();
	void InitSignalSlots();
	void ExitApplication();
	void LoadVideoList();

public:
	TreeView* GetTreeViewInst(){ return ui.TreeWidget; };

private:
	Ui::KkPlayerClass ui;

	//移动窗体
	QPoint				mWindowPos, mMousePos, mPosDelta;
	bool				mIsPressed;
	//获取程序运行目录
	QString				mRuntimePath;
	//全屏显示窗口
	int					mAvailableWidth, mAvailabelHeight;
	MODULE_PAGE_ENUM	mCurModulePage;
	SettingMenuWnd		*m_pSetMenuWnd;
	CommonSetting		*m_pCommonSetting;

public slots:
	void OnMinimumWnd();
	void OnMaximumWnd();
	void OnNormalWnd();
	void OnCloseWnd();
	void OnStartMenuAction(int);
	void OnPlayModule();
	void OnNetpageModule();
	void OnTitleStartMenu(int);
	void OnOpenmenubtnFromPlayactive(int);
	void OnShowHideTreeview(bool show);
	void OnDbClickVideoFile();
	void OnSwitchOpenUrl();
	void OnPlayerMenu(int type);

protected:
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void showEvent(QShowEvent *event);
	void resizeEvent(QResizeEvent *event);
	void customEvent(QEvent *event);
	bool eventFilter(QObject *pObject, QEvent *pEvent);
};
