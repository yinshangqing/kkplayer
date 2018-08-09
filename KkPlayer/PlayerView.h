#pragma once

#include <QWidget>
#include <QMenu>
#include <QAction>
#include <QTimer>
#include "ui_PlayerView.h"
#include "Common.h"

typedef enum{
	MYWEB_PAGE = 0,
	REAL_PLAYER_PAGE	
}PAGETYPE_ENUM;

typedef enum{
	PLAYER_MENU_OPEN_URL,
	PLAYER_MENU_OPEN_DIR,
	PLAYER_MENU_OPEN_BT,
	PLAYER_MENU_OPEN_FILE,
	PLAYER_MENU_OPEN_FULLSCREEN,
	PLAYER_MENU_OPEN_EXITFULLSCREEN,
	PLAYER_MENU_OPEN_FRONT,
	PLAYER_MENU_OPEN_NEVERFRONT,
	PLAYER_MENU_OPEN_PLAYSTART,
	PLAYER_MENU_OPEN_PLAYSTOP,
	PLAYER_MENU_OPEN_AUDIO,
	PLAYER_MENU_OPEN_VIDEO,
	PLAYER_MENU_OPEN_SNAP,
	PLAYER_MENU_OPEN_SETTING
}PLAYER_MENU_TYPE;

using namespace Kk;

class PlayerView : public QWidget
{
	Q_OBJECT

public:
	PlayerView(QWidget *parent = Q_NULLPTR);
	~PlayerView();

private:
	Ui::PlayerView ui;
	int	mAvailableWidth, mAvailabelHeight;
	PAGETYPE_ENUM mPageType;
	QTimer	*m_pVideoTimer;
	QTimer	*m_pEnumTimer;
	ENUM_PLAYSTATUS_TYPE mVideoStatus;
	int mVideoVolume;
	bool m_enterFullscreen;
	std::string mMediaFile;
	long mCurVideoSliderRange;
	QRect	mToolbarOldSize;
	QRect	mActivePageOldSize;
	
	QAction *m_pSubActionOpenFile;
	QAction *m_pSubActionOpenUrl;
	QAction *m_pSubActionOpenDir;
	QAction *m_pSubActionFront1;
	QAction *m_pSubActionFront2;
	QAction *m_pSubActionFront3;

	int m_startDragValue, m_stopDragValue;
	bool m_isVideoSliderPause;

private:
	void Init();
	void InitUi();
	void InitSignalSlots();
	void InitPlayer();
	bool PlayStart(std::string mediaFile, int nWidth, int nHeight);
	void DockToolbar(bool);

public:
	void SwtichToMyPage();
	void SwtichToMyPage(QString url);
	void BackFromMyPage();
	bool PlayVideoFile(VIDEO_TREE_ITEM videoItem);
	void OpenVideo();
	void SetFullScreen(bool fullscreen);
	void Snap();
	void ShowRightMenu(QPoint point);
	void ToolOperation(int type);

signals:
	void signal_showhide_treeview(bool show);
	void signal_openmenubtn_from_playactive(int type);
	void signal_switchOpenUrl();
	void signal_playermenu(int type);

private slots:
	void slot_click_show_hide_treeview(bool showOrhide);
	void slot_video_timer();
	void slot_enum_timer();
	void slot_volume_changed(int);
	void slot_video_changed(int);
	void slot_openvideobtn_clicked();
	void slot_openmenubtn_clicked(int type);
	void slot_toolbar_operation(int);
	void slot_video_slider_behavior(int behave);

	//menu slot
	void slot_playermenu_openfile();
	void slot_playermenu_openurl();
	void slot_playermenu_opendir();
	void slot_playermenu_openbt();
	void slot_playermenu_fullscreen();
	void slot_playermenu_exitfullscreen();
	void slot_playermenu_playstart();
	void slot_playermenu_playstop();
	void slot_playermenu_audio();
	void slot_playermenu_video();
	void slot_playermenu_front();
	void slot_playermenu_neverfront();
	void slot_playermenu_snap();
	void slot_playermenu_setting();

public:
	void slot_toolbar_video_operation(int type);

protected:
	void showEvent(QShowEvent *event);
	void resizeEvent(QResizeEvent *event);
	bool eventFilter(QObject *pObject, QEvent *pEvent);
};
