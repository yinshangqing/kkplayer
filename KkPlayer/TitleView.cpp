#include "stdafx.h"
#include "TitleView.h"
#include <QPoint>

const int const_mainwnd_height = 40;
const int const_menubtn_width = 39;
const int const_menubtn_height = 26;
const int const_modebtn_width = 81;
const int const_modebtn_height = 30;
const int const_operabtn_width = 20;
const int const_operabtn_height = 20;

TitleView::TitleView(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	Init();
	InitUi();
	InitSignalSlots();
}

TitleView::~TitleView()
{
}

void TitleView::Init()
{
	QDesktopWidget desktop;
	mAvailableWidth = desktop.availableGeometry().width();
	mAvailabelHeight = desktop.availableGeometry().height();
}

void TitleView::InitUi()
{
	setWindowFlags(Qt::FramelessWindowHint);

	setFixedHeight(const_mainwnd_height);
	ui.menu_btn->setFixedSize(const_menubtn_width, const_menubtn_height);
	ui.play_module_btn->setFixedSize(const_modebtn_width, const_modebtn_height);
	ui.netpage_module_btn->setFixedSize(const_modebtn_width, const_modebtn_height);
	ui.min_btn->setFixedSize(const_operabtn_width, const_operabtn_height);
	ui.max_btn->setFixedSize(const_operabtn_width, const_operabtn_height);
	ui.restore_btn->setFixedSize(const_operabtn_width, const_operabtn_height);
	ui.close_btn->setFixedSize(const_operabtn_width, const_operabtn_height);

	ui.min_btn->setToolTip(QStringLiteral("最小化"));
	ui.max_btn->setToolTip(QStringLiteral("最大化"));
	ui.restore_btn->setToolTip(QStringLiteral("还原"));
	ui.close_btn->setToolTip(QStringLiteral("关闭"));

	ui.menu_btn->setStyleSheet(
		"QToolButton{border-image: url(:/images/btn_menu_normal.png);}"
		"QToolButton:hover{border-image: url(:/images/btn_menu_hover.png);}"
		"QToolButton:pressed{border-image: url(:/images/btn_menu_press.png);}"
		);
	ui.play_module_btn->SetUiStyleSheet("QToolButton#play_module_btn{border-image:url(:/images/btn_mode_normal.png);color: rgb(184, 201, 193); font-size: 75 18px;}",
		"QToolButton#play_module_btn{border-image:url(:/images/btn_mode_press.png);color: rgb(184, 201, 193); font-size: 14px;}", ENUM_TOOLBUTTON_SELECT);
	ui.netpage_module_btn->SetUiStyleSheet("QToolButton#netpage_module_btn{border-image:url(:/images/btn_mode_normal.png);color: rgb(184, 201, 193); font-size: 75 18px;}",
		"QToolButton#netpage_module_btn{border-image:url(:/images/btn_mode_press.png);color: rgb(184, 201, 193); font-size: 14px;}", ENUM_TOOLBUTTON_IDLE);
	ui.min_btn->setStyleSheet(
		"QToolButton{border-image: url(:/images/btn_min_normal.png);}"
		"QToolButton:hover{border-image: url(:/images/btn_min_hover.png);}"
		"QToolButton:pressed{border-image: url(:/images/btn_min_press.png);}"
		);
	ui.max_btn->setStyleSheet(
		"QToolButton{border-image: url(:/images/btn_max_normal.png);}"
		"QToolButton:hover{border-image: url(:/images/btn_max_hover.png);}"
		"QToolButton:pressed{border-image: url(:/images/btn_max_press.png);}"
		);
	ui.restore_btn->setStyleSheet(
		"QToolButton{border-image: url(:/images/btn_restore_normal.png);}"
		"QToolButton:hover{border-image: url(:/images/btn_restore_hover.png);}"
		"QToolButton:pressed{border-image: url(:/images/btn_restore_press.png);}"
		);
	ui.close_btn->setStyleSheet(
		"QToolButton{border-image: url(:/images/btn_close_normal.png);}"
		"QToolButton:hover{border-image: url(:/images/btn_close_hover.png);}"
		"QToolButton:pressed{border-image: url(:/images/btn_close_press.png);}"
		);

	ui.play_module_btn->setText(QStringLiteral("播放"));
	ui.netpage_module_btn->setText(QStringLiteral("我的主页"));

	ui.restore_btn->setVisible(false);
}

void TitleView::InitSignalSlots()
{
	connect(ui.min_btn, SIGNAL(clicked()), this, SLOT(slot_click_minimum_btn()));
	connect(ui.max_btn, SIGNAL(clicked()), this, SLOT(slot_click_maximum_btn()));
	connect(ui.restore_btn, SIGNAL(clicked()), this, SLOT(slot_click_normal_btn()));
	connect(ui.close_btn, SIGNAL(clicked()), this, SLOT(slot_click_close_btn()));
	connect(ui.menu_btn, SIGNAL(clicked()), this, SLOT(slot_click_startmenu_btn()));
	connect(ui.play_module_btn, SIGNAL(clicked()), this, SLOT(slot_click_playmodule_btn()));
	connect(ui.netpage_module_btn, SIGNAL(clicked()), this, SLOT(slot_click_netpagemodule_btn()));
}

void TitleView::slot_click_minimum_btn()
{
	emit signal_show_minimum();
}

void TitleView::slot_click_maximum_btn()
{
	ui.max_btn->setVisible(false);
	ui.restore_btn->setVisible(true);
	emit signal_show_maximised();
}

void TitleView::slot_click_normal_btn()
{
	ui.max_btn->setVisible(true);
	ui.restore_btn->setVisible(false);
	emit signal_show_normal();
}

void TitleView::slot_click_close_btn()
{
	emit signal_close_wnd();
}

void TitleView::slot_click_startmenu_btn()
{
	QAction *pActionOpenFile = new QAction(QStringLiteral("打开文件"), this);
	QAction *pActionOpenUrl = new QAction(QStringLiteral("打开URL"), this);
	QAction *pActionOpenDir = new QAction(QStringLiteral("打开目录"), this);
	QAction *pActionSetting = new QAction(QStringLiteral("设置"), this);
	QAction *pActionExit = new QAction(QStringLiteral("退出"), this);
	QMenu *pMenu = new QMenu(this);

	QMenu *pMenu2 = new QMenu(QStringLiteral("打开"), this);
	pMenu->addMenu(pMenu2);
	pMenu->addAction(pActionSetting);
	pMenu->addAction(pActionExit);

	pMenu2->addAction(pActionOpenFile);
	pMenu2->addAction(pActionOpenUrl);
	pMenu2->addAction(pActionOpenDir);

	connect(pActionOpenFile, SIGNAL(triggered()), this, SLOT(slot_action_open_file()));
	connect(pActionOpenUrl, SIGNAL(triggered()), this, SLOT(slot_action_open_url()));
	connect(pActionOpenDir, SIGNAL(triggered()), this, SLOT(slot_action_open_dir()));
	connect(pActionSetting, SIGNAL(triggered()), this, SLOT(slot_action_setting()));
	connect(pActionExit, SIGNAL(triggered()), this, SLOT(slot_action_exit()));

	QPoint point = ui.menu_btn->mapToGlobal(ui.menu_btn->pos());
	point.setY(point.y() + 20);
	pMenu->exec(point);
	delete pActionOpenFile;
	delete pActionOpenUrl;
	delete pActionOpenDir;
	delete pActionSetting;
	delete pActionExit;
	delete pMenu2;
	delete pMenu;
}

void TitleView::slot_click_playmodule_btn()
{
	if (ui.play_module_btn && ui.netpage_module_btn)
	{
		ui.play_module_btn->SetState(ENUM_TOOLBUTTON_SELECT);
		ui.netpage_module_btn->SetState(ENUM_TOOLBUTTON_IDLE);
	}
	emit signal_play_module();
}

void TitleView::slot_click_netpagemodule_btn()
{
	if (ui.play_module_btn && ui.netpage_module_btn)
	{
		ui.play_module_btn->SetState(ENUM_TOOLBUTTON_IDLE);
		ui.netpage_module_btn->SetState(ENUM_TOOLBUTTON_SELECT);
	}
	emit signal_netpage_module();
}

void TitleView::slot_action_open_file()
{
	emit signal_start_menu(TITLEVIEW_ACTION_OPENFILE);
}

void TitleView::slot_action_open_url()
{
	emit signal_start_menu(TITLEVIEW_ACTION_OPENURL);
}

void TitleView::slot_action_open_dir()
{
	emit signal_start_menu(TITLEVIEW_ACTION_OPENDIR);
}

void TitleView::slot_action_setting()
{
	emit signal_start_menu(TITLEVIEW_ACTION_SETTING);
}

void TitleView::slot_action_exit()
{
	emit signal_start_menu(TITLEVIEW_ACTION_EXIT);
}

void TitleView::showEvent(QShowEvent *event)
{
	QPalette t_BgPalette;
	t_BgPalette.setBrush(QPalette::Background, QBrush(QPixmap(":/images/title_bg.png").
		scaled(this->width(), this->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
	this->setAutoFillBackground(TRUE);
	this->setPalette(t_BgPalette);
}

void TitleView::resizeEvent(QResizeEvent *event)
{
	QPalette t_BgPalette;
	t_BgPalette.setBrush(QPalette::Background, QBrush(QPixmap(":/images/title_bg.png").
		scaled(this->width(), this->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
	this->setAutoFillBackground(TRUE);
	this->setPalette(t_BgPalette);
}

void TitleView::contextMenuEvent(QContextMenuEvent *e)
{
	
}