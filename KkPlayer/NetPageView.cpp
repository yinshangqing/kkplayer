#include "stdafx.h"
#include "NetPageView.h"

NetPageView::NetPageView(QWidget *parent)
	: QWidget(parent),
	mPageType(NETPAGE_ACTIVE_PAGE)
{
	ui.setupUi(this);
	Init();
	InitUi();
	InitSignalSlots();
}

NetPageView::~NetPageView()
{

}

void NetPageView::Init()
{
	
}

void NetPageView::InitUi()
{
	setWindowFlags(Qt::FramelessWindowHint);

	QPalette pal(palette());
	pal.setColor(QPalette::Background, QColor(qRgb(7, 7, 7)));
	setAutoFillBackground(true);
	setPalette(pal);
}

void NetPageView::InitSignalSlots()
{
	//connect(ui.min_btn, SIGNAL(clicked()), this, SLOT(slot_click_minimum_btn()));
}

void NetPageView::SwtichToPage(NETPAGE_TYPE pageType)
{
	ui.stackedWidget->setCurrentIndex(pageType);
}

void NetPageView::SwtichToPage(NETPAGE_TYPE pageType, QString url)
{
	ui.stackedWidget->setCurrentIndex(pageType);
	ui.ActivePage->OpenUrl(url);
}

void NetPageView::showEvent(QShowEvent *event)
{
	InitUi();
}