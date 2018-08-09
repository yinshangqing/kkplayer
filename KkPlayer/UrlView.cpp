#include "stdafx.h"
#include "UrlView.h"

const int wnd_height = 30;
const int ctrl_height = 18;
const int refreshbtn_width = 18;

QString g_urlStr = "";

UrlView::UrlView(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	Init();
	InitSignalSlots();
}

UrlView::~UrlView()
{
}

void UrlView::Init()
{
	ui.url_edit->installEventFilter(this);
}

void UrlView::InitUi()
{
	setWindowFlags(Qt::FramelessWindowHint);

	setFixedHeight(wnd_height);
	ui.url_edit->setFixedHeight(ctrl_height);
	ui.refresh_btn->setFixedSize(refreshbtn_width, ctrl_height);

	ui.refresh_btn->setStyleSheet(
		"QToolButton{border-image: url(:/images/btn_refresh_normal.png);}"
		"QToolButton:hover{border-image: url(:/images/btn_refresh_hover.png);}"
		"QToolButton:pressed{border-image: url(:/images/btn_refresh_press.png);}"
		);
	ui.url_edit->setStyleSheet("QLineEdit{color: rgb(82,86,88); font: 75 14px; border: 0px solid rgb(13, 65, 119);border - radius: 3px;background:rgb(14,14,14);selection-background-color: rgb(14,14,14);}"
		"QLineEdit:hover{border: 1px solid rgb(13, 65, 119);}");

	QPalette pal(palette());
	pal.setColor(QPalette::Background, QColor(qRgb(20, 20, 20)));
	setAutoFillBackground(true);
	setPalette(pal);

	ui.url_edit->setPlaceholderText(QStringLiteral("ÇëÊäÈëËÑË÷¹Ø¼ü´Ê"));
}

void UrlView::InitSignalSlots()
{
	connect(ui.refresh_btn, SIGNAL(clicked()), this, SLOT(slot_click_refresh_btn()));
}

void UrlView::slot_click_refresh_btn()
{
	g_urlStr = ui.url_edit->text();
	if (!g_urlStr.isEmpty())
		emit signal_openUrl();
}

bool UrlView::eventFilter(QObject *obj, QEvent *event)
{
	int nEventType = event->type();
	if (obj == ui.url_edit)
	{
		switch (nEventType)
		{
		case QEvent::KeyPress:
		{
			QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
			if (keyEvent->key() == Qt::Key_Enter)
			{
				slot_click_refresh_btn();
			}
			break;
		}
		}
	}
	return false;
}

void UrlView::showEvent(QShowEvent *event)
{
	InitUi();
}