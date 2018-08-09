#include "stdafx.h"
#include "PlayInactivePageView.h"

PlayInactivePageView::PlayInactivePageView(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	Init();
	InitSignalSlots();
}

PlayInactivePageView::~PlayInactivePageView()
{
}

void PlayInactivePageView::Init()
{

}

void PlayInactivePageView::InitUi()
{
	setWindowFlags(Qt::FramelessWindowHint);

	/*ui.menu_btn->setFixedSize(const_menubtn_width, const_menubtn_height);
	ui.play_module_btn->setFixedSize(const_modebtn_width, const_modebtn_height);
	ui.netpage_module_btn->setFixedSize(const_modebtn_width, const_modebtn_height);
	ui.min_btn->setFixedSize(const_operabtn_width, const_operabtn_height);
	ui.max_btn->setFixedSize(const_operabtn_width, const_operabtn_height);
	ui.restore_btn->setFixedSize(const_operabtn_width, const_operabtn_height);
	ui.close_btn->setFixedSize(const_operabtn_width, const_operabtn_height);

	ui.menu_btn->setStyleSheet(
		"QToolButton{border-image: url(:/images/btn_menu_normal.png);}"
		"QToolButton:hover{border-image: url(:/images/btn_menu_hover.png);}"
		"QToolButton:pressed{border-image: url(:/images/btn_menu_press.png);}"
		);*/

	QPalette pal(palette());
	pal.setColor(QPalette::Background, QColor(qRgb(7, 7, 7)));
	setAutoFillBackground(true);
	setPalette(pal);
}

void PlayInactivePageView::InitSignalSlots()
{
	//connect(ui.min_btn, SIGNAL(clicked()), this, SLOT(slot_click_minimum_btn()));
}

void PlayInactivePageView::SwtichToNetPage()
{
	ui.NetPage->SwtichToPage(NETPAGE_ACTIVE_PAGE);
	ui.NetPage->update();
}

void PlayInactivePageView::SwtichToNetPage(QString url)
{
	ui.NetPage->SwtichToPage(NETPAGE_ACTIVE_PAGE, url);
	ui.NetPage->update();
}

void PlayInactivePageView::showEvent(QShowEvent *event)
{
	InitUi();
}