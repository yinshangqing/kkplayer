#include "stdafx.h"
#include "NetInactivePageView.h"

const int net_back_forward_width_height = 25;
const int tips_widget_width = 452;
const int tips_widget_height = 132;
const int tips_image_width = 122;
const int tips_msg_width = 300;
const int tips_msg_height = 64;
const int tips_refresh_width = 86;
const int tips_refresh_height = 27;

NetInactivePageView::NetInactivePageView(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	Init();
	InitSignalSlots();
}

NetInactivePageView::~NetInactivePageView()
{
}

void NetInactivePageView::Init()
{

}

void NetInactivePageView::InitUi()
{
	setWindowFlags(Qt::FramelessWindowHint);

	ui.netback_btn->setFixedSize(net_back_forward_width_height, net_back_forward_width_height);
	ui.netforward_btn->setFixedSize(net_back_forward_width_height, net_back_forward_width_height);
	ui.lineEdit->setFixedHeight(net_back_forward_width_height);
	ui.fresh_btn->setFixedSize(net_back_forward_width_height - 7, net_back_forward_width_height - 7);
	ui.widget->setFixedHeight(net_back_forward_width_height);
	ui.widget_4->setFixedSize(tips_widget_width, tips_widget_height);
	ui.widget_3->setFixedSize(tips_msg_width, tips_widget_height);
	ui.manual_refresh_btn->setFixedSize(tips_refresh_width, tips_refresh_height);
	ui.tips_image->setFixedSize(tips_image_width, tips_widget_height);
	ui.tips_msg->setFixedSize(tips_msg_width, tips_msg_height);

	ui.netback_btn->setStyleSheet(
		"QToolButton{border-image: url(:/images/btn_net_back.png);}"
		"QToolButton:hover{border-image: url(:/images/btn_net_back.png);}"
		"QToolButton:pressed{border-image: url(:/images/btn_net_back.png);}"
		);
	ui.netforward_btn->setStyleSheet(
		"QToolButton{border-image: url(:/images/btn_net_forward.png);}"
		"QToolButton:hover{border-image: url(:/images/btn_net_forward.png);}"
		"QToolButton:pressed{border-image: url(:/images/btn_net_forward.png);}"
		);
	ui.fresh_btn->setStyleSheet(
		"QToolButton{border-image: url(:/images/btn_refresh_normal.png);}"
		"QToolButton:hover{border-image: url(:/images/btn_refresh_hover.png);}"
		"QToolButton:pressed{border-image: url(:/images/btn_refresh_press.png);}"
		);
	ui.manual_refresh_btn->setStyleSheet(
		"QToolButton{border-image: url(:/images/btn_net_refresh.png);}"
		"QToolButton:hover{border-image: url(:/images/btn_net_refresh_hover.png);}"
		"QToolButton:pressed{border-image: url(:/images/btn_net_refresh_hover.png);}"
		);
	ui.lineEdit->setStyleSheet("QLineEdit{color: rgb(73,85,66); font: 75 14px; border: 0px solid rgb(13, 65, 119);border - radius: 3px;background:rgb(14,14,14);selection-background-color: rgb(14,14,14);}"
		"QLineEdit:hover{border: 1px solid rgb(13, 65, 119);}");
	ui.tips_image->setStyleSheet( "QLabel{border-image:url(:/images/tips_image.png)};" );
	ui.tips_msg->setStyleSheet("QLabel{border-image:url(:/images/tips_msg.png)};");

	QPalette pal(palette());
	pal.setColor(QPalette::Background, QColor(qRgb(14, 14, 14)));
	ui.widget->setAutoFillBackground(true);
	ui.widget->setPalette(pal);

	QPalette pal1(palette());
	pal1.setColor(QPalette::Background, QColor(qRgb(34, 34, 34)));
	ui.widget_2->setAutoFillBackground(true);
	ui.widget_2->setPalette(pal1);

	ui.lineEdit->setPlaceholderText("url: ");
}

void NetInactivePageView::InitSignalSlots()
{
	//connect(ui.min_btn, SIGNAL(clicked()), this, SLOT(slot_click_minimum_btn()));
}

void NetInactivePageView::showEvent(QShowEvent *event)
{
	InitUi();
}