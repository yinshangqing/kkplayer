#include "stdafx.h"
#include "MyTipsBox.h"
#include "KkPlayer.h"
#include <QBitmap>

const int confirm_cancel_button_width = 86;
const int confirm_cancel_button_height = 26;
const int label_wrap_left_right_offset = 30;

extern KkPlayer* g_pMainWnd;

MyTipsBox::MyTipsBox(QString title, QString tipsInfo1, QString tipsInfo2, QWidget* parent, int timeCount)
	: QDialog(parent)
	, mStatus(0)		//0: ok , 1: cancel
	, mTimeCounts(timeCount)
	, m_pTimer(NULL)
{
	ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

	QPalette t_BgPalette;
	t_BgPalette.setBrush(QPalette::Background, QBrush(QPixmap(":/images/tips_bg.png").
		scaled(this->width(), this->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
	this->setAutoFillBackground(TRUE);
	this->setPalette(t_BgPalette);

	connect(ui.okBtn, SIGNAL(clicked()), this, SLOT(slot_clickOkButton()));
	connect(ui.cancelBtn, SIGNAL(clicked()), this, SLOT(slot_clickCancelButton()));
	ui.titleLabel->setText(title);
	ui.tipsLabel_1->setText(tipsInfo1);
	ui.tipsLabel_2->setText(tipsInfo2);
	ui.tipsLabel_1->setWordWrap(true);
	ui.tipsLabel_1->setWordWrap(true);
	setOkButtonStyle("", timeCount);
	setCancelButtonStyle("", timeCount);
	if (timeCount > 0 && !m_pTimer)
	{
		m_pTimer = new QTimer(this);
	}
}

MyTipsBox::~MyTipsBox()
{

}

int MyTipsBox::showMessage(
	const QString & tips1, 
	const QString & tips2,
	QWidget *parent, 
	const QString& title,
	int timeCount)
{
	MyTipsBox m(title, tips1, tips2, parent, timeCount);
	if (timeCount>0 && m.m_pTimer)
	{
		connect(m.m_pTimer, SIGNAL(timeout()), &m, SLOT(slot_timer()));
		m.m_pTimer->start(timeCount);
		if (g_pMainWnd)
		{
			QRect rect = g_pMainWnd->geometry();
			int x = (rect.width() - m.width()) / 2;
			int y = (rect.height() - m.height()) / 2;
			m.setGeometry(x, y, m.width(), m.height());
		}
	}
	return m.exec();
}

int MyTipsBox::showTipsMsg(
	const int & errorCode,
	const QString & errorDesc,
	QWidget *parent, 
	const QString& title,
	int timeCount)
{
	MyTipsBox m(title, "", errorDesc, parent, timeCount);
	if (timeCount>0 && m.m_pTimer)
	{
		connect(m.m_pTimer, SIGNAL(timeout()), &m, SLOT(slot_timer()));
		m.m_pTimer->start(timeCount);
		if (g_pMainWnd)
		{
			QRect rect = g_pMainWnd->geometry();
			int x = (rect.width() - m.width()) / 2;
			int y = (rect.height() - m.height()) / 2;
			m.setGeometry(x, y, m.width(), m.height());
		}
	}
	return m.exec();
}

bool MyTipsBox::setOkButtonStyle(QString imgPath, int timeCount)
{
	if (timeCount > 0)
	{
		ui.okBtn->setVisible(false);
		return true;
	}
	ui.okBtn->setStyleSheet("QPushButton#okBtn{border-image: url(:/images/ok_normal.png);}"
		"QPushButton#okBtn:hover{border-image: url(:/images/ok_hover.png);}"
		"QPushButton#okBtn:pressed{border-image: url(:/images/ok_press.png);}");
	ui.okBtn->setFixedSize(confirm_cancel_button_width, confirm_cancel_button_height);
	ui.okBtn->setVisible(true);
	return true;
}

bool MyTipsBox::setCancelButtonStyle(QString imgPath, int timeCount)
{
	if (timeCount > 0)
	{
		ui.cancelBtn->setVisible(false);
		return true;
	}
	ui.cancelBtn->setStyleSheet("QPushButton#cancelBtn{border-image: url(:/images/cancel_normal.png);}"
		"QPushButton#cancelBtn:hover{border-image: url(:/images/cancel_hover.png);}"
		"QPushButton#cancelBtn:pressed{border-image: url(:/images/cancel_press.png);}");
	ui.cancelBtn->setFixedSize(confirm_cancel_button_width, confirm_cancel_button_height);
	ui.cancelBtn->setVisible(true);
	return true;
}

int MyTipsBox::status()
{
	return mStatus;
}

void MyTipsBox::slot_clickOkButton()
{
	mStatus = 0;
	done(0);
}

void MyTipsBox::slot_clickCancelButton()
{
	mStatus = 1;
	done(3);
}

void MyTipsBox::slot_clickCloseButton()
{
	mStatus = 2;
	done(2);
}

void MyTipsBox::slot_timer()
{
	m_pTimer->stop();
	delete m_pTimer;
	close();
}

void MyTipsBox::mousePressEvent(QMouseEvent *event)
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

void MyTipsBox::mouseMoveEvent(QMouseEvent *event)
{
	if (mIsPressed)
		this->move(event->globalPos() - mPosDelta);
}

void MyTipsBox::mouseReleaseEvent(QMouseEvent *event)
{
	mIsPressed = false;
}