#include "stdafx.h"
#include "KkPlayer.h"
#include "UrlView.h"

KkPlayer* g_pMainWnd = NULL;
extern QString g_urlStr;
MyDatabase	mDb;
bool		g_DbIsOnline = false;
extern QString	gLastDir;

const int const_mainwnd_width = 1224;
const int const_mainwnd_height = 760;

KkPlayer::KkPlayer(QWidget *parent)
	: QWidget(parent),
	mCurModulePage(MODULE_PAGE_PLAY),
	m_pSetMenuWnd(NULL),
	m_pCommonSetting(NULL)
{
	g_pMainWnd = this;
	mRuntimePath = QApplication::applicationDirPath();
	m_pCommonSetting = new CommonSetting;
	gLastDir = m_pCommonSetting->GetCommonSettings().lastDir;
	ui.setupUi(this);
	Init();
	InitUi();
	InitSignalSlots();
}

KkPlayer::~KkPlayer()
{
	if (m_pCommonSetting)
	{
		delete m_pCommonSetting;
		m_pCommonSetting = NULL;
	}
}

void KkPlayer::Init()
{
	QDesktopWidget desktop;
	mAvailableWidth = desktop.availableGeometry().width();
	mAvailabelHeight = desktop.availableGeometry().height();

	g_DbIsOnline = mDb.Connect("KkPlayer", "admin", "123456");
	if (g_DbIsOnline)
	{
		mDb.CreateVideoTable();
	}
	ui.TreeWidget->LoadVideoList();
	ui.PlayerWidget->installEventFilter(this);
}

void KkPlayer::InitUi()
{
	setWindowFlags(Qt::FramelessWindowHint);
	setFixedSize(const_mainwnd_width, const_mainwnd_height);

	QPalette pal(palette());
	pal.setColor(QPalette::Background, QColor(qRgb(43, 43, 43)));
	setAutoFillBackground(true);
	setPalette(pal);
}

void KkPlayer::InitSignalSlots()
{
	//标题栏的信号与槽对应关系
	connect(ui.TitleWidget, SIGNAL(signal_show_minimum()), this, SLOT(OnMinimumWnd()));
	connect(ui.TitleWidget, SIGNAL(signal_show_maximised()), this, SLOT(OnMaximumWnd()));
	connect(ui.TitleWidget, SIGNAL(signal_show_normal()), this, SLOT(OnNormalWnd()));
	connect(ui.TitleWidget, SIGNAL(signal_close_wnd()), this, SLOT(OnCloseWnd()));
	connect(ui.TitleWidget, SIGNAL(signal_start_menu(int)), this, SLOT(OnStartMenuAction(int)));
	connect(ui.TitleWidget, SIGNAL(signal_play_module()), this, SLOT(OnPlayModule()));
	connect(ui.TitleWidget, SIGNAL(signal_netpage_module()), this, SLOT(OnNetpageModule()));
	connect(ui.TitleWidget, SIGNAL(signal_start_menu(int)), this, SLOT(OnTitleStartMenu(int)));

	connect(ui.PlayerWidget, SIGNAL(signal_showhide_treeview(bool)), this, SLOT(OnShowHideTreeview(bool)));
	connect(ui.PlayerWidget, SIGNAL(signal_openmenubtn_from_playactive(int)), this, SLOT(OnOpenmenubtnFromPlayactive(int)));
	connect(ui.PlayerWidget, SIGNAL(signal_switchOpenUrl()), this, SLOT(OnSwitchOpenUrl()));
	connect(ui.PlayerWidget, SIGNAL(signal_playermenu(int)), this, SLOT(OnPlayerMenu(int)));

	connect(ui.TreeWidget, SIGNAL(signal_playvideo()), this, SLOT(OnDbClickVideoFile()));
}

void KkPlayer::ExitApplication()
{
	close();
}

void KkPlayer::OnMinimumWnd()
{
	showMinimized();
}

void KkPlayer::OnMaximumWnd()
{
	showMaximized();
}

void KkPlayer::OnNormalWnd()
{
	showNormal();
}

void KkPlayer::OnCloseWnd()
{
	ExitApplication();
}

void KkPlayer::OnPlayModule()
{
	ui.TreeWidget->setVisible(true);
	ui.PlayerWidget->BackFromMyPage();
	ui.PlayerWidget->update();
}

void KkPlayer::OnNetpageModule()
{
	ui.TreeWidget->setVisible(false);
	ui.PlayerWidget->SwtichToMyPage();
	ui.PlayerWidget->update();
}

void KkPlayer::OnTitleStartMenu(int type)
{
	if( type == TITLEVIEW_ACTION_OPENFILE)
		ui.PlayerWidget->OpenVideo();
	else if (type == TITLEVIEW_ACTION_OPENDIR)
	{
		QString appPath = QApplication::applicationDirPath();
		appPath.append("/download/");
		QDesktopServices::openUrl(appPath);
	}
	else if (type == TITLEVIEW_ACTION_OPENURL)
	{
		OnNetpageModule();
	}
	else if (type == TITLEVIEW_ACTION_EXIT)
	{
		OnCloseWnd();
	}
	else if (type == TITLEVIEW_ACTION_SETTING)
	{
		if (!m_pSetMenuWnd)
			m_pSetMenuWnd = new SettingMenuWnd;
		if (m_pSetMenuWnd && !m_pSetMenuWnd->isVisible())
			m_pSetMenuWnd->show();
	}
}

void KkPlayer::OnOpenmenubtnFromPlayactive(int type)
{
	if (type == PLAYACTIVE_OPEN_URL)
	{
		OnNetpageModule();
	}
	else if (type == PLAYACTIVE_OPEN_DIR)
	{
		QString appPath = QApplication::applicationDirPath();
		appPath.append("/download/");
		QDesktopServices::openUrl(appPath); 
	}
	else if (type == PLAYACTIVE_OPEN_BT)
	{
		OnNetpageModule();
	}
}

void KkPlayer::OnSwitchOpenUrl()
{
	ui.TreeWidget->setVisible(false);
	QString urlStr = g_urlStr;
	if (urlStr.startsWith("www."))
	{
		urlStr = "https://" + urlStr;
	}
	ui.PlayerWidget->SwtichToMyPage(urlStr);
}

void KkPlayer::OnPlayerMenu(int type)
{
	if (type == PLAYER_MENU_OPEN_FILE)
	{
		ui.PlayerWidget->OpenVideo();
	}
	else if (type == PLAYACTIVE_OPEN_URL)
	{
		OnNetpageModule();
	}
	else if (type == PLAYER_MENU_OPEN_DIR)
	{
		QString appPath = QApplication::applicationDirPath();
		appPath.append("/download/");
		QDesktopServices::openUrl(appPath);
	}
	else if (type == PLAYER_MENU_OPEN_BT)
	{
		OnNetpageModule();
	}
	else if (type == PLAYER_MENU_OPEN_FULLSCREEN)
	{
		ui.PlayerWidget->SetFullScreen(true);
	}
	else if (type == PLAYER_MENU_OPEN_EXITFULLSCREEN)
	{
		ui.PlayerWidget->SetFullScreen(false);
	}
	else if (type == PLAYER_MENU_OPEN_FRONT)
	{
		this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
		this->show();
	}
	else if (type == PLAYER_MENU_OPEN_NEVERFRONT)
	{
		this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnBottomHint);
		this->show();
	}
	else if (type == PLAYER_MENU_OPEN_PLAYSTART)
	{
		ui.PlayerWidget->ToolOperation(TOOLBAR_VIDEO_PLAY);
	}
	else if (type == PLAYER_MENU_OPEN_PLAYSTOP)
	{
		ui.PlayerWidget->ToolOperation(TOOLBAR_VIDEO_STOP);
	}
	else if (type == PLAYER_MENU_OPEN_AUDIO)
	{

	}
	else if (type == PLAYER_MENU_OPEN_VIDEO)
	{

	}
	else if (type == PLAYER_MENU_OPEN_SNAP)
	{
		ui.PlayerWidget->Snap();
	}
	else if (type == PLAYER_MENU_OPEN_SETTING)
	{
		if (!m_pSetMenuWnd)
			m_pSetMenuWnd = new SettingMenuWnd;
		if (m_pSetMenuWnd && !m_pSetMenuWnd->isVisible())
			m_pSetMenuWnd->show();
	}
}

void KkPlayer::OnShowHideTreeview(bool show)
{
	ui.TreeWidget->setVisible(show);
}

void KkPlayer::OnStartMenuAction(int action)
{
	switch (action)
	{
	case TITLEVIEW_ACTION_EXIT:
		close();
		exit(1);
		break;
	default:
		break;
	}
}

void KkPlayer::OnDbClickVideoFile()
{
	ui.PlayerWidget->PlayVideoFile(ui.TreeWidget->GetSelTreeItem());
}

void KkPlayer::mousePressEvent(QMouseEvent *event)
{
	if (!event)	return;
	if (event->button() == Qt::LeftButton)
	{
		mIsPressed = true;
		mWindowPos = this->pos();
		mMousePos = event->globalPos();
		mPosDelta = mMousePos - mWindowPos;
	}
}

void KkPlayer::mouseMoveEvent(QMouseEvent *event)
{
	if (mIsPressed)
		this->move(event->globalPos() - mPosDelta);
}

void KkPlayer::mouseReleaseEvent(QMouseEvent *event)
{
	mIsPressed = false;
}

void KkPlayer::showEvent(QShowEvent *event)
{
	/*QPalette palete(palette());
	palete.setBrush(QPalette::Background, QBrush(QPixmap(":/image/bottom_bg.png").\
		scaled(this->width(), this->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
	setAutoFillBackground(TRUE);
	setPalette(palete);*/
}

void KkPlayer::resizeEvent(QResizeEvent *event)
{
	showEvent(NULL);
}

void KkPlayer::customEvent(QEvent *event)
{

}

bool KkPlayer::eventFilter(QObject *pObject, QEvent *pEvent)
{
	int nEventType = pEvent->type();
	if (pObject == ui.PlayerWidget)
	{
		switch (nEventType)
		{
		case QEvent::MouseButtonPress:
		{
			QMouseEvent *miceEvent = static_cast<QMouseEvent*>(pEvent);
			if (miceEvent && ui.PlayerWidget && miceEvent->button() == Qt::RightButton)
			{
				ui.PlayerWidget->ShowRightMenu(miceEvent->globalPos());
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