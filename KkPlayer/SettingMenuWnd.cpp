#include "stdafx.h"
#include "SettingMenuWnd.h"

const int const_operabtn_width = 20;
const int const_operabtn_height = 20;
const int const_operabtn_width2 = 80;
const int const_operabtn_height2 = 25;

SettingMenuWnd::SettingMenuWnd(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	Init();
	InitUi();
	InitSignalSlots();
}

SettingMenuWnd::~SettingMenuWnd()
{
}

void SettingMenuWnd::Init()
{
	mCurMenuType = SETTING_MENU_CHANGGUI;
}

void SettingMenuWnd::InitUi()
{
	setWindowFlags(Qt::FramelessWindowHint);
	ui.close_btn->setFixedSize(const_operabtn_width, const_operabtn_height);
	ui.toolButton->setFixedSize(const_operabtn_width2, const_operabtn_height2);
	ui.toolButton_2->setFixedSize(const_operabtn_width2, const_operabtn_height2);
	ui.toolButton_3->setFixedSize(const_operabtn_width2, const_operabtn_height2);
	ui.toolButton_4->setFixedSize(const_operabtn_width2, const_operabtn_height2);

	ui.changgui_btn->setStyleSheet(
		"QToolButton{background-color: rgb(40,43,44, 60);color:rgb(255,255,255);}"
		"QToolButton:hover{background-color: rgb(20,52,52, 60);color:rgb(255,255,255);}"
		"QToolButton:pressed{background-color: rgb(48,128,200, 60);color:rgb(255,255,255);}"
		);
	ui.bofang_btn->setStyleSheet(
		"QToolButton{background-color: rgb(40,43,44, 60);color:rgb(255,255,255);}"
		"QToolButton:hover{background-color: rgb(20,52,52, 60);color:rgb(255,255,255);}"
		"QToolButton:pressed{background-color: rgb(48,128,200, 60);color:rgb(255,255,255);}"
		);
	ui.shenyin_btn->setStyleSheet(
		"QToolButton{background-color: rgb(40,43,44, 60);color:rgb(255,255,255);}"
		"QToolButton:hover{background-color: rgb(20,52,52, 60);color:rgb(255,255,255);}"
		"QToolButton:pressed{background-color: rgb(48,128,200, 60);color:rgb(255,255,255);}"
		);
	ui.zimu_btn->setStyleSheet(
		"QToolButton{background-color: rgb(40,43,44, 60);color:rgb(255,255,255);}"
		"QToolButton:hover{background-color: rgb(20,52,52, 60);color:rgb(255,255,255);}"
		"QToolButton:pressed{background-color: rgb(48,128,200, 60);color:rgb(255,255,255);}"
		);
	ui.huamian_btn->setStyleSheet(
		"QToolButton{background-color: rgb(40,43,44, 60);color:rgb(255,255,255);}"
		"QToolButton:hover{background-color: rgb(20,52,52, 60);color:rgb(255,255,255);}"
		"QToolButton:pressed{background-color: rgb(48,128,200, 60);color:rgb(255,255,255);}"
		);
	ui.jietu_btn->setStyleSheet(
		"QToolButton{background-color: rgb(40,43,44, 60);color:rgb(255,255,255);}"
		"QToolButton:hover{background-color: rgb(20,52,52, 60);color:rgb(255,255,255);}"
		"QToolButton:pressed{background-color: rgb(48,128,200, 60);color:rgb(255,255,255);}"
		);
	ui.close_btn->setStyleSheet(
		"QToolButton{border-image: url(:/images/btn_close_normal.png);}"
		"QToolButton:hover{border-image: url(:/images/btn_close_hover.png);}"
		"QToolButton:pressed{border-image: url(:/images/btn_close_press.png);}"
		);
	ui.toolButton_2->setStyleSheet(
		"QToolButton{background-color: rgb(40,43,44, 60);color:rgb(255,255,255);}"
		"QToolButton:hover{background-color: rgb(20,52,52, 60);color:rgb(255,255,255);}"
		"QToolButton:pressed{background-color: rgb(48,128,200, 60);color:rgb(255,255,255);}"
		);
	ui.toolButton_3->setStyleSheet(
		"QToolButton{background-color: rgb(40,43,44, 60);color:rgb(255,255,255);}"
		"QToolButton:hover{background-color: rgb(20,52,52, 60);color:rgb(255,255,255);}"
		"QToolButton:pressed{background-color: rgb(48,128,200, 60);color:rgb(255,255,255);}"
		);
	ui.toolButton->setStyleSheet(
		"QToolButton{background-color: rgb(40,43,44, 60);color:rgb(255,255,255);}"
		"QToolButton:hover{background-color: rgb(20,52,52, 60);color:rgb(255,255,255);}"
		"QToolButton:pressed{background-color: rgb(48,128,200, 60);color:rgb(255,255,255);}"
		);
	ui.toolButton_4->setStyleSheet(
		"QToolButton{background-color: rgb(40,43,44, 60);color:rgb(255,255,255);}"
		"QToolButton:hover{background-color: rgb(20,52,52, 60);color:rgb(255,255,255);}"
		"QToolButton:pressed{background-color: rgb(48,128,200, 60);color:rgb(255,255,255);}"
		);
	ui.imagetype_combo->setStyleSheet(
		"QComboBox#imagetype_combo{border: 0px solid gray; padding: 1px 18px 1px 3px;}"
		"QComboBox:!editable, QComboBox::drop-down:!editable#imagetype_combo{background: rgb(25,25,25);}"
		"QComboBox:!editable:on, QComboBox::drop-down:!editable:on#imagetype_combo{background: rgb(47,47,47);}"
		"QComboBox::down-arrow#imagetype_combo{border-image: url(:/images/combo_right_normal.png);width:20; height:20;}"
		"QComboBox::down-arrow:hover#imagetype_combo{border-image: url(:/images/combo_right_hover.png);width:20; height:20;}"
		"QComboBox::down-arrow:pressed#imagetype_combo{border-image: url(:/images/combo_right_press.png);width:20; height:20;}"
		"QComboBox::drop-down#imagetype_combo{width: 20px; height: 20px;}"
		"QComboBox#imagetype_combo{color: white;}"
		"QComboBox QAbstractItemView#imagetype_combo{color: black; border: 1px solid darkgray; selection-color: red; selection-background-color: rgb(64, 64, 64);}");
	ui.kuaijin_combo->setStyleSheet(
		"QComboBox{border: 0px solid gray; padding: 1px 18px 1px 3px;}"
		"QComboBox:!editable, QComboBox::drop-down:!editable{background: rgb(25,25,25);}"
		"QComboBox:!editable:on, QComboBox::drop-down:!editable:on{background: rgb(47,47,47);}"
		"QComboBox::down-arrow{border-image: url(:/images/combo_right_normal.png);width:20; height:20;}"
		"QComboBox::down-arrow:hover{border-image: url(:/images/combo_right_hover.png);width:20; height:20;}"
		"QComboBox::down-arrow:pressed{border-image: url(:/images/combo_right_press.png);width:20; height:20;}"
		"QComboBox::drop-down{width: 20px; height: 20px;}"
		"QComboBox{color: white;}"
		"QComboBox QAbstractItemView{color: black; border: 1px solid darkgray; selection-color: red; selection-background-color: rgb(64, 64, 64);}");
	ui.kuaitui_combo->setStyleSheet(
		"QComboBox{border: 0px solid gray; padding: 1px 18px 1px 3px;}"
		"QComboBox:!editable, QComboBox::drop-down:!editable{background: rgb(25,25,25);}"
		"QComboBox:!editable:on, QComboBox::drop-down:!editable:on{background: rgb(47,47,47);}"
		"QComboBox::down-arrow{border-image: url(:/images/combo_right_normal.png);width:20; height:20;}"
		"QComboBox::down-arrow:hover{border-image: url(:/images/combo_right_hover.png);width:20; height:20;}"
		"QComboBox::down-arrow:pressed{border-image: url(:/images/combo_right_press.png);width:20; height:20;}"
		"QComboBox::drop-down{width: 20px; height: 20px;}"
		"QComboBox{color: white;}"
		"QComboBox QAbstractItemView{color: black; border: 1px solid darkgray; selection-color: red; selection-background-color: rgb(64, 64, 64);}");

	ui.shenyin_btn->setVisible(false);
}

void SettingMenuWnd::InitSignalSlots()
{
	connect(ui.changgui_btn, SIGNAL(clicked()), this, SLOT(slot_click_changgui_btn()));
	connect(ui.bofang_btn, SIGNAL(clicked()), this, SLOT(slot_click_bofang_btn()));
	connect(ui.shenyin_btn, SIGNAL(clicked()), this, SLOT(slot_click_shenyin_btn()));
	connect(ui.zimu_btn, SIGNAL(clicked()), this, SLOT(slot_click_zimu_btn()));
	connect(ui.huamian_btn, SIGNAL(clicked()), this, SLOT(slot_click_huamian_btn()));
	connect(ui.jietu_btn, SIGNAL(clicked()), this, SLOT(slot_click_jietu_btn()));
	connect(ui.toolButton, SIGNAL(clicked()), this, SLOT(slot_click_queren()));
	connect(ui.toolButton_2, SIGNAL(clicked()), this, SLOT(slot_click_yingyong()));
	connect(ui.toolButton_3, SIGNAL(clicked()), this, SLOT(slot_click_quxiao()));
	connect(ui.close_btn, SIGNAL(clicked()), this, SLOT(slot_click_close()));
	connect(ui.toolButton_4, SIGNAL(clicked()), this, SLOT(slot_click_liulanjietu()));
}

void SettingMenuWnd::slot_click_changgui_btn()
{
	ui.stackedWidget->setCurrentIndex(SETTING_MENU_CHANGGUI);
	mCurMenuType = SETTING_MENU_CHANGGUI;
}

void SettingMenuWnd::slot_click_bofang_btn()
{
	ui.stackedWidget->setCurrentIndex(SETTING_MENU_BOFANG);
	mCurMenuType = SETTING_MENU_BOFANG;
}

void SettingMenuWnd::slot_click_shenyin_btn()
{
	ui.stackedWidget->setCurrentIndex(SETTING_MENU_SHENYIN);
	mCurMenuType = SETTING_MENU_SHENYIN;
}

void SettingMenuWnd::slot_click_zimu_btn()
{
	ui.stackedWidget->setCurrentIndex(SETTING_MENU_ZIMU);
	mCurMenuType = SETTING_MENU_ZIMU;
}

void SettingMenuWnd::slot_click_huamian_btn()
{
	ui.stackedWidget->setCurrentIndex(SETTING_MENU_HUAMIAN);
	mCurMenuType = SETTING_MENU_HUAMIAN;
}

void SettingMenuWnd::slot_click_jietu_btn()
{
	ui.stackedWidget->setCurrentIndex(SETTING_MENU_JIETU);
	mCurMenuType = SETTING_MENU_JIETU;
}

void SettingMenuWnd::showEvent(QShowEvent *event)
{
	QPalette pal(palette());
	pal.setColor(QPalette::Background, QColor(qRgb(40, 43, 44)));
	setAutoFillBackground(true);
	setPalette(pal);
}

void SettingMenuWnd::slot_click_queren()
{
	hide();
}

void SettingMenuWnd::slot_click_yingyong()
{
	hide();
}

void SettingMenuWnd::slot_click_quxiao()
{
	hide();
}

void SettingMenuWnd::slot_click_close()
{
	hide();
}

void SettingMenuWnd::slot_click_liulanjietu()
{
	QString file_path = QFileDialog::getExistingDirectory(this, "ÇëÑ¡ÔñÂ·¾¶...", "./");
	if (file_path.isEmpty())
	{
		return;
	}
	else
	{
		ui.lineEdit->setText(file_path);
		qDebug() << file_path << endl;
	}
}

void SettingMenuWnd::mousePressEvent(QMouseEvent *event)
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

void SettingMenuWnd::mouseMoveEvent(QMouseEvent *event)
{
	if (mIsPressed)
		this->move(event->globalPos() - mPosDelta);
}

void SettingMenuWnd::mouseReleaseEvent(QMouseEvent *event)
{
	mIsPressed = false;
}