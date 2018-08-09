#include "stdafx.h"
#include "NetActivePageView.h"
#include "MyTipsBox.h"
#include <QtWebKit/QWebSettings>

const int net_back_forward_width_height = 25;

NetActivePageView::NetActivePageView(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	Init();
	InitSignalSlots();
}

NetActivePageView::~NetActivePageView()
{
}

void NetActivePageView::Init()
{
	QWebSettings *settings = QWebSettings::globalSettings();
	settings->setAttribute(QWebSettings::PluginsEnabled, TRUE);
	settings->setAttribute(QWebSettings::JavascriptEnabled, TRUE);
	settings->setAttribute(QWebSettings::DeveloperExtrasEnabled, TRUE);
	settings->setAttribute(QWebSettings::JavascriptCanOpenWindows, TRUE);
	ui.webpage->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
	ui.lineEdit->installEventFilter(this);
}

void NetActivePageView::InitUi()
{
	setWindowFlags(Qt::FramelessWindowHint);

	ui.netback_btn->setFixedSize(net_back_forward_width_height, net_back_forward_width_height);
	ui.netforward_btn->setFixedSize(net_back_forward_width_height, net_back_forward_width_height);
	ui.lineEdit->setFixedHeight(net_back_forward_width_height);
	ui.fresh_btn->setFixedSize(net_back_forward_width_height - 7, net_back_forward_width_height-7);
	ui.widget->setFixedHeight(net_back_forward_width_height);
	
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
	ui.lineEdit->setStyleSheet("QLineEdit{color: rgb(73,85,66); font: 75 14px; border: 0px solid rgb(13, 65, 119);border - radius: 3px;background:rgb(14,14,14);selection-background-color: rgb(14,14,14);}"
		"QLineEdit:hover{border: 1px solid rgb(13, 65, 119);}");

	QPalette pal(palette());
	pal.setColor(QPalette::Background, QColor(qRgb(14, 14, 14)));
	ui.widget->setAutoFillBackground(true);
	ui.widget->setPalette(pal);

	ui.webpage->setStyleSheet("background-color:rgb(34,34,34); padding: 7px;");

	ui.lineEdit->setPlaceholderText("url: ");
}

void NetActivePageView::InitSignalSlots()
{
	connect(ui.netback_btn, SIGNAL(clicked()), this, SLOT(slot_click_netback_btn()));
	connect(ui.netforward_btn, SIGNAL(clicked()), this, SLOT(slot_click_netforward_btn()));
	connect(ui.fresh_btn, SIGNAL(clicked()), this, SLOT(slot_click_refresh_btn()));
	connect(ui.webpage, SIGNAL(linkClicked(QUrl)), this, SLOT(slot_openNewUrl(QUrl)));
}

void NetActivePageView::OpenUrl(QString url)
{
	if (url.isEmpty())
	{
		/*MyTipsBox::showMessage(QStringLiteral("访问网站"),
			QStringLiteral("请输入有效的url"),
			this, QStringLiteral("提示"));*/
		return;
	}
	ui.lineEdit->setText(url);
	ui.webpage->load(QUrl(url));
}

void NetActivePageView::slot_click_netback_btn()
{
	ui.webpage->back();
}

void NetActivePageView::slot_click_netforward_btn()
{
	ui.webpage->forward();
}

void NetActivePageView::slot_click_refresh_btn()
{
	QString url = ui.lineEdit->text();
	if (url.isEmpty())
	{
		/*MyTipsBox::showMessage(QStringLiteral("访问网站"),
			QStringLiteral("请输入有效的url"),
			this, QStringLiteral("提示"));*/
		return;
	}
	QString urlStr = url;
	if (urlStr.startsWith("www."))
	{
		urlStr = "https://" + urlStr;
	}
	ui.webpage->load(QUrl(urlStr));
}

void NetActivePageView::slot_openNewUrl(QUrl url)
{
	ui.webpage->load(url);
	ui.webpage->setStyleSheet("background-color:rgb(221,221,221); padding: 7px;");
}

void NetActivePageView::showEvent(QShowEvent *event)
{
	InitUi();
}

bool NetActivePageView::eventFilter(QObject *obj, QEvent *event)
{
	int nEventType = event->type();
	if (obj == ui.lineEdit)
	{
		switch (nEventType)
		{
		case QEvent::KeyPress:
		{
			QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
			qDebug() << "key type = " << keyEvent->key();
			if (keyEvent->key() == Qt::Key_Return)
			{
				slot_click_refresh_btn();
			}
			break;
		}
		default:
			break;
		}
	}
	return false;
}