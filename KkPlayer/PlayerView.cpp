#include "stdafx.h"
#include "PlayerView.h"
#include "AvPlayer.h"
#include "KkPlayer.h"
#include "MyTipsBox.h"

const int const_mainwnd_height = 40;
const int const_menubtn_width = 39;
const int const_menubtn_height = 26;
const int const_modebtn_width = 81;
const int const_modebtn_height = 30;
const int const_operabtn_width = 20;
const int const_operabtn_height = 20;
const int toolbar_height = 90;

CAVPlayer	g_AVPlayer;
extern KkPlayer* g_pMainWnd;
extern QString gCurrentTreeItemPath;

BOOL CALLBACK EnumerateVLC(HWND hWndvlc, LPARAM lParam)
{
	TCHAR szWndTitle[1024];
	int nLen = GetWindowText(hWndvlc, szWndTitle, 1024);
	if (0 != nLen)
	{
		//禁用鼠标消息
		EnableWindow(hWndvlc, FALSE);
	}
	return TRUE;
}


PlayerView::PlayerView(QWidget *parent)
	: QWidget(parent),
	mPageType(REAL_PLAYER_PAGE),
	mVideoStatus(ENUM_PLAYSTATUS_STOP),
	mVideoVolume(25),
	m_enterFullscreen(false),
	mCurVideoSliderRange(0)
{
	ui.setupUi(this);
	Init();
	InitSignalSlots();
}

PlayerView::~PlayerView()
{
}

void PlayerView::Init()
{
	QDesktopWidget desktop;
	mAvailableWidth = desktop.width();
	mAvailabelHeight = desktop.height();
	mMediaFile.empty();
	InitPlayer();
	m_pVideoTimer = new QTimer(this);
	m_pVideoTimer->setInterval(1000);
	m_pEnumTimer = new QTimer(this);
	m_pEnumTimer->setInterval(1000);
	m_pEnumTimer->start();
	ui.ActivePage->installEventFilter(this);
	ui.InactivePage->SwtichToNetPage("http://www.baidu.com");
}

void PlayerView::InitUi()
{
	//setWindowFlags(Qt::FramelessWindowHint);
}

void PlayerView::InitSignalSlots()
{
	connect(ui.ToolbarWidget, SIGNAL(signal_click_show_hide_treeview(bool)), this, SLOT(slot_click_show_hide_treeview(bool)));
	connect(ui.ToolbarWidget, SIGNAL(signal_toolbar_video_operation(int)), this, SLOT(slot_toolbar_operation(int)));
	connect(ui.ToolbarWidget, SIGNAL(signal_voice_value_changed(int)), this, SLOT(slot_volume_changed(int)));
	connect(ui.ToolbarWidget, SIGNAL(signal_video_value_changed(int)), this, SLOT(slot_video_changed(int)));
	connect(ui.ToolbarWidget, SIGNAL(signal_video_slider_behavior(int)), this, SLOT(slot_video_slider_behavior(int)));
	connect(m_pVideoTimer, SIGNAL(timeout()), this, SLOT(slot_video_timer()));
	connect(m_pEnumTimer, SIGNAL(timeout()), this, SLOT(slot_enum_timer()));
	connect(ui.ActivePage, SIGNAL(signal_openvideobtn_clicked()), this, SLOT(slot_openvideobtn_clicked()));
	connect(ui.ActivePage, SIGNAL(signal_openmenubtn_clicked(int)), this, SLOT(slot_openmenubtn_clicked(int)));
	connect(ui.UrlWidget, SIGNAL(signal_openUrl()), this, SIGNAL(signal_switchOpenUrl()));
}

void PlayerView::InitPlayer()
{
	g_AVPlayer.SetHWND((HWND)ui.ActivePage->winId());
	g_AVPlayer.SetCallbackPlaying(NULL);
	g_AVPlayer.SetCallbackPosChanged(NULL);
	g_AVPlayer.SetCallbackEndReached(NULL);
}

void PlayerView::showEvent(QShowEvent *event)
{
	QPalette pal(palette());
	pal.setColor(QPalette::Background, QColor(qRgb(7, 7, 7)));
	setAutoFillBackground(true);
	setPalette(pal);
}

void PlayerView::SwtichToMyPage()
{
	if (mPageType == REAL_PLAYER_PAGE)
	{
		ui.ToolbarWidget->setVisible(false);
		ui.UrlWidget->setVisible(false);
		ui.stackedWidget->setCurrentIndex(MYWEB_PAGE);
		mPageType = MYWEB_PAGE;
		ui.InactivePage->SwtichToNetPage("http://www.baidu.com");
		ui.InactivePage->update();
	}
}

void PlayerView::SwtichToMyPage(QString	url)
{
	if (mPageType == REAL_PLAYER_PAGE)
	{
		ui.ToolbarWidget->setVisible(false);
		ui.UrlWidget->setVisible(false);
		ui.stackedWidget->setCurrentIndex(MYWEB_PAGE);
		mPageType = MYWEB_PAGE;
		ui.InactivePage->SwtichToNetPage(url);
	}
}

void PlayerView::BackFromMyPage()
{
	if (mPageType == MYWEB_PAGE)
	{
		ui.ToolbarWidget->setVisible(true);
		ui.UrlWidget->setVisible(true);
		ui.stackedWidget->setCurrentIndex(REAL_PLAYER_PAGE);
		mPageType = REAL_PLAYER_PAGE;
	}
}

bool PlayerView::PlayStart(std::string mediaFile, int nWidth, int nHeight)
{
	if (mediaFile.empty())
		return false;

	bool bRet = false;
	if (g_AVPlayer.IsPlaying())
	{
		g_AVPlayer.Stop();
		mVideoStatus = ENUM_PLAYSTATUS_STOP;
	}
	if (g_AVPlayer.Play(mediaFile, nWidth, nHeight))
	{
		g_AVPlayer.Volume(25);
		bRet = true;
		mVideoStatus = ENUM_PLAYSTATUS_PLAYING;
		ui.ActivePage->SetActiveMode(true);
		ui.ToolbarWidget->SetPlayUi(true);
		mMediaFile = mediaFile;
		m_pVideoTimer->start();
	}
	else
	{
		ui.ActivePage->SetActiveMode(false);
	}

	return bRet;
}

bool PlayerView::PlayVideoFile(VIDEO_TREE_ITEM videoItem)
{
	QString fileName = QString::fromLocal8Bit(videoItem.videoFilePath.c_str());
	fileName.replace("/", "\\");
	QByteArray cdata = fileName.toUtf8();
	std::string mediafile(cdata);
	return PlayStart(mediafile, ui.ActivePage->width(), ui.ActivePage->height());
}

void PlayerView::DockToolbar(bool IsDock)
{
	if (IsDock)
	{
		ui.ToolbarWidget->setParent(NULL);
		ui.ToolbarWidget->setWindowFlags(Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);
		ui.ToolbarWidget->setGeometry(0, mAvailabelHeight - toolbar_height, mAvailableWidth, toolbar_height);
		ui.ToolbarWidget->show();
	}
	else
	{
		ui.verticalLayout_2->addWidget(ui.ToolbarWidget);
		ui.ToolbarWidget->setParent(this);
		//ui.ToolbarWidget->showNormal();
		ui.ToolbarWidget->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
		ui.ToolbarWidget->setGeometry(mToolbarOldSize);
		ui.ToolbarWidget->show();
	}
}

void PlayerView::OpenVideo()
{
	QString filePath = QFileDialog::getOpenFileName(this, QStringLiteral("打开"), "", QStringLiteral("媒体文件（所有文件）") + ";*.*");
	if (filePath.isEmpty() || !g_pMainWnd)
	{
		return;
	}

	vector<VIDEO_TREE_ITEM> vecVideoList;
	g_pMainWnd->GetTreeViewInst()->GetVecVideoList(vecVideoList);
	int videoIdStart = vecVideoList.size();
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

	g_pMainWnd->GetTreeViewInst()->AddVideoItem(videoInfo);
	g_pMainWnd->GetTreeViewInst()->PushVideoItem(videoInfo);

	PlayVideoFile(videoInfo);
}

void PlayerView::SetFullScreen(bool fullscreen)
{
	DockToolbar(fullscreen);
	m_enterFullscreen = fullscreen;
	ui.ToolbarWidget->SetFullScreenUi(fullscreen);

	if (fullscreen)
	{
		mToolbarOldSize = ui.ToolbarWidget->geometry();
		mActivePageOldSize = ui.ActivePage->geometry();
		ui.ActivePage->setParent(NULL);
		g_pMainWnd->hide();
		ui.ActivePage->showFullScreen();
	}
	else
	{
		ui.ActivePage->setParent(ui.stackedWidget);
		ui.ActivePage->setGeometry(mActivePageOldSize);
		g_pMainWnd->show();
		slot_click_show_hide_treeview(false);
		slot_click_show_hide_treeview(true);
		ui.ToolbarWidget->SetHideTreeBtn(false);
	}
}

void PlayerView::Snap()
{
	if (g_AVPlayer.IsPlaying())
	{
		// 获取当前时间
		QDateTime dTime = QDateTime::currentDateTime();
		// 格式化
		QString fullPath = "picture_" + dTime.toString("yyMMddhhmmss.jpg");
		// 保存文件全路径
		QString strDir;
		QString appPath = QApplication::applicationDirPath();
		appPath.append("/picture/");
		appPath.append(fullPath);
		if (0 != g_AVPlayer.SnapShoot(fullPath.toLocal8Bit().data()))
		{
			/*MyTipsBox::showMessage(QStringLiteral("截图"),
				QStringLiteral("截图失败，请重试"),
				this, QStringLiteral("提示"));*/
		}
	}
	else
	{
		/*MyTipsBox::showMessage(QStringLiteral("截图"),
			QStringLiteral("请先开启视频再进行截图"),
			this, QStringLiteral("提示"));*/
	}
}

void PlayerView::ShowRightMenu(QPoint point)
{
	QAction *pActionOpenFile = new QAction(QStringLiteral("打开文件"), this);
	QAction *pActionOpenUrl = new QAction(QStringLiteral("打开URL"), this);
	QAction *pActionOpenBt = new QAction(QStringLiteral("打开BT"), this);
	QAction *pActionOpenDir = new QAction(QStringLiteral("打开缓存目录"), this);

	QAction *pActionFullscreen = new QAction(QStringLiteral("全屏"), this);
	QAction *pActionExitFullscreen = new QAction(QStringLiteral("退出全屏"), this);
	QAction *pActionFront = new QAction(QStringLiteral("总是最前"), this);
	QAction *pActionNeverFront = new QAction(QStringLiteral("取消最前"), this);
	QAction *pActionPlay = new QAction(QStringLiteral("播放"), this);
	QAction *pActionStop = new QAction(QStringLiteral("停止播放"), this);
	QAction *pActionAudio = new QAction(QStringLiteral("声音"), this);
	QAction *pActionPicture = new QAction(QStringLiteral("画面"), this);
	QAction *pActionCapture = new QAction(QStringLiteral("截图"), this);
	QAction *pActionSetting = new QAction(QStringLiteral("设置"), this);
	QMenu *pMenu = new QMenu(this);
	QMenu *pMenu2 = new QMenu(QStringLiteral("打开"), this);

	pMenu->addMenu(pMenu2);
	pMenu2->addAction(pActionOpenFile);
	pMenu2->addAction(pActionOpenUrl);
	pMenu2->addAction(pActionOpenBt);
	pMenu2->addAction(pActionOpenDir);
	connect(pActionOpenFile, SIGNAL(triggered()), this, SLOT(slot_playermenu_openfile()));
	connect(pActionOpenUrl, SIGNAL(triggered()), this, SLOT(slot_playermenu_openurl()));
	connect(pActionOpenDir, SIGNAL(triggered()), this, SLOT(slot_playermenu_opendir()));
	connect(pActionOpenBt, SIGNAL(triggered()), this, SLOT(slot_playermenu_openbt()));

	if (m_enterFullscreen)
		pMenu->addAction(pActionExitFullscreen);
	else
		pMenu->addAction(pActionFullscreen);

	if (g_AVPlayer.IsPlaying())
		pMenu->addAction(pActionStop);
	else
		pMenu->addAction(pActionPlay);
	//pMenu->addAction(pActionAudio);
	//pMenu->addAction(pActionPicture);
	pMenu->addAction(pActionFront);
	pMenu->addAction(pActionNeverFront);
	pMenu->addAction(pActionCapture);
	pMenu->addAction(pActionSetting);
	connect(pActionFullscreen, SIGNAL(triggered()), this, SLOT(slot_playermenu_fullscreen()));
	connect(pActionExitFullscreen, SIGNAL(triggered()), this, SLOT(slot_playermenu_exitfullscreen()));
	connect(pActionPlay, SIGNAL(triggered()), this, SLOT(slot_playermenu_playstart()));
	connect(pActionStop, SIGNAL(triggered()), this, SLOT(slot_playermenu_playstop()));
	connect(pActionAudio, SIGNAL(triggered()), this, SLOT(slot_playermenu_audio()));
	connect(pActionPicture, SIGNAL(triggered()), this, SLOT(slot_playermenu_video()));
	connect(pActionFront, SIGNAL(triggered()), this, SLOT(slot_playermenu_front()));
	connect(pActionNeverFront, SIGNAL(triggered()), this, SLOT(slot_playermenu_neverfront()));
	connect(pActionCapture, SIGNAL(triggered()), this, SLOT(slot_playermenu_snap()));
	connect(pActionSetting, SIGNAL(triggered()), this, SLOT(slot_playermenu_setting()));

	pMenu->exec(point);

	delete pActionOpenFile;
	delete pActionOpenUrl;
	delete pActionOpenBt;
	delete pActionOpenDir;
	delete pActionFullscreen;
	delete pActionFront;
	delete pActionNeverFront;
	delete pActionPlay;
	delete pActionStop;
	delete pActionAudio;
	delete pActionPicture;
	delete pActionCapture;
	delete pActionSetting;
	delete pMenu2;
	delete pMenu;
}

void PlayerView::ToolOperation(int type)
{
	int curIdx;
	VIDEO_TREE_ITEM treeItem;
	if (type == TOOLBAR_VIDEO_AUDIO_CLOSE)
	{
		if (g_AVPlayer.IsPlaying())
		{
			mVideoVolume = g_AVPlayer.GetVolume();
			g_AVPlayer.Volume(0);
			ui.ToolbarWidget->SetVolumeUi(false);
		}
	}
	else if (type == TOOLBAR_VIDEO_AUDIO_OPEN)
	{
		if (g_AVPlayer.IsPlaying())
		{
			g_AVPlayer.Volume(mVideoVolume);
			ui.ToolbarWidget->SetVolumeUi(true);
		}
	}
	else if (type == TOOLBAR_VIDEO_FAST)
	{
		if (g_AVPlayer.IsPlaying())
		{
			g_AVPlayer.SeekForward();
		}
	}
	else if (type == TOOLBAR_VIDEO_FULLSCREEN)
	{
		if (g_AVPlayer.IsPlaying())
		{
			SetFullScreen(true);
		}
	}
	else if (type == TOOLBAR_VIDEO_NORMALSCREEN)
	{
		if (g_AVPlayer.IsPlaying())
		{
			SetFullScreen(false);
		}
	}
	else if (type == TOOLBAR_VIDEO_STOP)
	{
		if (g_AVPlayer.IsPlaying() || mVideoStatus != ENUM_PLAYSTATUS_STOP)
		{
			g_AVPlayer.Stop();
			mVideoStatus = ENUM_PLAYSTATUS_STOP;
			ui.ToolbarWidget->SetPlayUi(false);
		}
	}
	else if (type == TOOLBAR_VIDEO_PLAY)
	{
		if (g_AVPlayer.IsPlaying())
		{
			if (mVideoStatus == ENUM_PLAYSTATUS_STOP)
			{
				g_AVPlayer.Play();
				mVideoStatus = ENUM_PLAYSTATUS_PLAYING;
				ui.ActivePage->SetActiveMode(true);
				ui.ToolbarWidget->SetPlayUi(true);
			}
		}
		else
		{
			if (mVideoStatus == ENUM_PLAYSTATUS_PAUSE)
			{
				g_AVPlayer.Resume();
				mVideoStatus = ENUM_PLAYSTATUS_PLAYING;
				ui.ToolbarWidget->SetPlayUi(true);
			}
			else if (mVideoStatus == ENUM_PLAYSTATUS_STOP)
			{
				if (!gCurrentTreeItemPath.isEmpty())
				{
					QString fileName = gCurrentTreeItemPath;
					fileName.replace("/", "\\");
					QByteArray cdata = fileName.toUtf8();
					std::string mediafile(cdata);
					mMediaFile = mediafile;
				}
				PlayStart(mMediaFile, ui.ActivePage->width(), ui.ActivePage->height());
			}
		}
	}
	else if (type == TOOLBAR_VIDEO_PAUSE)
	{
		if (g_AVPlayer.IsPlaying() && mVideoStatus != ENUM_PLAYSTATUS_PAUSE)
		{
			g_AVPlayer.Pause();
			mVideoStatus = ENUM_PLAYSTATUS_PAUSE;
			ui.ToolbarWidget->SetPlayUi(false, true);
		}
	}
	else if (type == TOOLBAR_VIDEO_SLOW)
	{
		if (g_AVPlayer.IsPlaying())
		{
			g_AVPlayer.SeekBackward();
		}
	}
	else if (type == TOOLBAR_VIDEO_PREVIOUS)
	{
		if (g_AVPlayer.IsPlaying())
		{
			if (g_pMainWnd)
			{
				curIdx = g_pMainWnd->GetTreeViewInst()->GetCurrentItemIndex();
				treeItem = g_pMainWnd->GetTreeViewInst()->GetPreviousItem();
				if (PlayVideoFile(treeItem))
					g_pMainWnd->GetTreeViewInst()->SetCurrentItem(curIdx - 1);
			}
		}
	}
	else if (type == TOOLBAR_VIDEO_NEXT)
	{
		if (g_AVPlayer.IsPlaying())
		{
			if (g_pMainWnd)
			{
				curIdx = g_pMainWnd->GetTreeViewInst()->GetCurrentItemIndex();
				treeItem = g_pMainWnd->GetTreeViewInst()->GetNextItem();
				if (PlayVideoFile(treeItem))
					g_pMainWnd->GetTreeViewInst()->SetCurrentItem(curIdx + 1);
			}
		}
	}
	else if (type == TOOLBAR_VIDEO_OPEN)
	{
		OpenVideo();
	}
}

void PlayerView::slot_click_show_hide_treeview(bool showOrhide)
{
	emit signal_showhide_treeview(showOrhide);
}

void PlayerView::slot_toolbar_video_operation(int type)
{
	int curIdx;
	VIDEO_TREE_ITEM treeItem;
	if (type == TOOLBAR_VIDEO_AUDIO_CLOSE)
	{
		if (g_AVPlayer.IsPlaying())
		{
			mVideoVolume = g_AVPlayer.GetVolume();
			g_AVPlayer.Volume(0);
			ui.ToolbarWidget->SetVolumeUi(false);
		}
	}
	else if (type == TOOLBAR_VIDEO_AUDIO_OPEN)
	{
		if (g_AVPlayer.IsPlaying())
		{
			g_AVPlayer.Volume(mVideoVolume);
			ui.ToolbarWidget->SetVolumeUi(true);
		}
	}
	else if (type == TOOLBAR_VIDEO_FAST)
	{
		if (g_AVPlayer.IsPlaying())
		{
			g_AVPlayer.SeekForward();
		}
	}
	else if (type == TOOLBAR_VIDEO_FULLSCREEN)
	{
		if (g_AVPlayer.IsPlaying())
		{
			SetFullScreen(true);
		}
	}
	else if (type == TOOLBAR_VIDEO_NORMALSCREEN)
	{
		if (g_AVPlayer.IsPlaying())
		{
			SetFullScreen(false);
		}
	}
	else if (type == TOOLBAR_VIDEO_STOP)
	{
		if (g_AVPlayer.IsPlaying() || mVideoStatus != ENUM_PLAYSTATUS_STOP)
		{
			g_AVPlayer.Stop();
			mVideoStatus = ENUM_PLAYSTATUS_STOP;
			ui.ToolbarWidget->SetPlayUi(false);
		}
	}
	else if (type == TOOLBAR_VIDEO_PLAY)
	{
		if (g_AVPlayer.IsPlaying())
		{
			if (mVideoStatus == ENUM_PLAYSTATUS_STOP)
			{
				g_AVPlayer.Play();
				mVideoStatus = ENUM_PLAYSTATUS_PLAYING;
				ui.ActivePage->SetActiveMode(true);
				ui.ToolbarWidget->SetPlayUi(true);
			}
		}
		else
		{
			if (mVideoStatus == ENUM_PLAYSTATUS_PAUSE)
			{
				g_AVPlayer.Resume();
				mVideoStatus = ENUM_PLAYSTATUS_PLAYING;
				ui.ToolbarWidget->SetPlayUi(true);
			}
			else if (mVideoStatus == ENUM_PLAYSTATUS_STOP)
			{
				if (!mMediaFile.empty())
				{
					PlayStart(mMediaFile, ui.ActivePage->width(), ui.ActivePage->height());
				}
			}
		}
	}
	else if (type == TOOLBAR_VIDEO_PAUSE)
	{
		if (g_AVPlayer.IsPlaying() && mVideoStatus != ENUM_PLAYSTATUS_PAUSE)
		{
			g_AVPlayer.Pause();
			mVideoStatus = ENUM_PLAYSTATUS_PAUSE;
			ui.ToolbarWidget->SetPlayUi(false);
		}
	}
	else if (type == TOOLBAR_VIDEO_SLOW)
	{
		if (g_AVPlayer.IsPlaying())
		{
			g_AVPlayer.SeekBackward();
		}
	}
	else if (type == TOOLBAR_VIDEO_PREVIOUS)
	{
		if (g_AVPlayer.IsPlaying())
		{
			if (g_pMainWnd)
			{
				curIdx = g_pMainWnd->GetTreeViewInst()->GetCurrentItemIndex();
				treeItem = g_pMainWnd->GetTreeViewInst()->GetPreviousItem();
				if (PlayVideoFile(treeItem))
					g_pMainWnd->GetTreeViewInst()->SetCurrentItem(curIdx - 1);
			}
		}
	}
	else if (type == TOOLBAR_VIDEO_NEXT)
	{
		if (g_AVPlayer.IsPlaying())
		{
			if (g_pMainWnd)
			{
				curIdx = g_pMainWnd->GetTreeViewInst()->GetCurrentItemIndex();
				treeItem = g_pMainWnd->GetTreeViewInst()->GetNextItem();
				if (PlayVideoFile(treeItem))
					g_pMainWnd->GetTreeViewInst()->SetCurrentItem(curIdx + 1);
			}
		}
	}
	else if (type == TOOLBAR_VIDEO_OPEN)
	{
		OpenVideo();
		/*int curItemIdx = g_pMainWnd->GetTreeViewInst()->GetCurrentItemIndex();
		g_pMainWnd->GetTreeViewInst()->SetCurrentItem(curItemIdx);*/
	}
}

void PlayerView::slot_toolbar_operation(int type)
{
	ToolOperation(type);
}

void PlayerView::slot_volume_changed(int volume)
{
	if (g_AVPlayer.IsPlaying())
	{
		g_AVPlayer.Volume(volume);
	}
}

void PlayerView::slot_video_changed(int volume)
{
	if (g_AVPlayer.IsPlaying())
	{
		g_AVPlayer.SeekTo((float)volume / (ui.ToolbarWidget->GetVideoRange()));
	}
}

void PlayerView::slot_video_slider_behavior(int behave)
{
	if (behave == 0)
	{
		g_AVPlayer.Pause();
		m_startDragValue = ui.ToolbarWidget->GetVideoSliderValue();
		m_isVideoSliderPause = true;
	}
	else if (behave == 1)
	{
		int value = ui.ToolbarWidget->GetVideoSliderValue();
		m_stopDragValue = value;
		if (m_stopDragValue == m_startDragValue)
		{
			g_AVPlayer.Play();
			m_startDragValue = -1;
			m_stopDragValue = -1;
			return;
		}

		float percent = value / (float)(ui.ToolbarWidget->GetVideoRange());
		g_AVPlayer.SeekTo(percent);
		m_isVideoSliderPause = false;
		g_AVPlayer.Play();
		m_startDragValue = -1;
		m_stopDragValue = -1;
	}
}

void PlayerView::slot_video_timer()
{
	if (!g_AVPlayer.IsPlaying())
	{
		ui.ActivePage->SetActiveMode(false);
		m_pVideoTimer->stop();
	}
}

void PlayerView::slot_enum_timer()
{
	if (g_AVPlayer.IsPlaying())
		EnumChildWindows((HWND)ui.ActivePage->winId(), EnumerateVLC, NULL);
}

void PlayerView::slot_openvideobtn_clicked()
{
	OpenVideo();
}

void PlayerView::slot_openmenubtn_clicked(int type)
{
	emit signal_openmenubtn_from_playactive(type);
}

void PlayerView::slot_playermenu_openfile()
{
	emit signal_playermenu(PLAYER_MENU_OPEN_FILE);
}

void PlayerView::slot_playermenu_openurl()
{
	emit signal_playermenu(PLAYER_MENU_OPEN_URL);
}

void PlayerView::slot_playermenu_opendir()
{
	emit signal_playermenu(PLAYER_MENU_OPEN_DIR);
}

void PlayerView::slot_playermenu_openbt()
{
	emit signal_playermenu(PLAYER_MENU_OPEN_BT);
}

void PlayerView::slot_playermenu_fullscreen()
{
	emit signal_playermenu(PLAYER_MENU_OPEN_FULLSCREEN);
}

void PlayerView::slot_playermenu_exitfullscreen()
{
	emit signal_playermenu(PLAYER_MENU_OPEN_EXITFULLSCREEN);
}

void PlayerView::slot_playermenu_playstart()
{
	emit signal_playermenu(PLAYER_MENU_OPEN_PLAYSTART);
}

void PlayerView::slot_playermenu_playstop()
{
	emit signal_playermenu(PLAYER_MENU_OPEN_PLAYSTOP);
}

void PlayerView::slot_playermenu_audio()
{
	emit signal_playermenu(PLAYER_MENU_OPEN_AUDIO);
}

void PlayerView::slot_playermenu_video()
{
	emit signal_playermenu(PLAYER_MENU_OPEN_VIDEO);
}

void PlayerView::slot_playermenu_front()
{
	emit signal_playermenu(PLAYER_MENU_OPEN_FRONT);
}

void PlayerView::slot_playermenu_neverfront()
{
	emit signal_playermenu(PLAYER_MENU_OPEN_NEVERFRONT);
}

void PlayerView::slot_playermenu_snap()
{
	emit signal_playermenu(PLAYER_MENU_OPEN_SNAP);
}

void PlayerView::slot_playermenu_setting()
{
	emit signal_playermenu(PLAYER_MENU_OPEN_SETTING);
}

void PlayerView::resizeEvent(QResizeEvent *event)
{
	ui.ActivePage->setFixedSize(ui.stackedWidget->width(), ui.stackedWidget->height());
}

bool PlayerView::eventFilter(QObject *pObject, QEvent *pEvent)
{
	int nEventType = pEvent->type();
	if (pObject == ui.ActivePage)
	{
		switch (nEventType)
		{
		case QEvent::KeyPress:
		{
			QKeyEvent *keyEvent = static_cast<QKeyEvent*>(pEvent);
			if (keyEvent->key() == Qt::Key_Escape)
			{
				if (g_AVPlayer.IsPlaying() && m_enterFullscreen)
				{
					SetFullScreen(false);
				}
			}
			break;
		}
		case QEvent::MouseButtonDblClick:
		{
			if (g_AVPlayer.IsPlaying())
			{
				if (m_enterFullscreen)
				{
					SetFullScreen(false);
				}
				else
				{
					SetFullScreen(true);
				}
			}
			break;
		}
		case QEvent::MouseButtonPress:
		{
			QString name = pObject->objectName();
			QMouseEvent *miceEvent = static_cast<QMouseEvent*>(pEvent);
			if (miceEvent && miceEvent->button() == Qt::RightButton)
			{
				ShowRightMenu(miceEvent->globalPos());
				return true;
			}
			break;
		}
		default:
			break;
		}
	}
	return FALSE;
}