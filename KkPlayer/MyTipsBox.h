/*
创建：袁晓东
日期：2017-06-15
功能：自定义模态提示框，包括手动操作和定时隐藏功能
*/
#ifndef CUIMYTIPSBOX_H
#define CUIMYTIPSBOX_H

#include <QWidget>
#include <QtWidgets/QDialog>
#include <QMouseEvent>
#include <QTimer>
#include "ui_myTipsBox.h"

class MyTipsBox : public QDialog
{
	Q_OBJECT

public:
	~MyTipsBox();

	static int MyTipsBox::showMessage(
		const QString & tips1,
		const QString& tips2,
		QWidget *parent = NULL, 
		const QString & title = "",
		int timeCount = 0);

	static int MyTipsBox::showTipsMsg(
		const int & errorCode,
		const QString& errorDesc,
		QWidget *parent = NULL, 
		const QString & title = "",
		int	timeCount = 0);
	
private:
	Ui::myTipsBox ui;
	int	mStatus;
	int mTimeCounts;
	QTimer	*m_pTimer;

	QPoint	mWindowPos, mMousePos, mPosDelta;
	bool	mIsPressed;

	bool setOkButtonStyle(QString imgPath, int timeCount = 0);
	bool setCancelButtonStyle(QString imgPath, int timeCount = 0);
	int status();

private slots:
	void slot_clickOkButton();
	void slot_clickCancelButton();
	void slot_clickCloseButton();
	void slot_timer();

protected:
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);

	MyTipsBox(QString title, QString tipsInfo1, QString tipsInfo2, QWidget* parent = NULL, int timeCount = 0);
};

#endif // CUIMYTIPSBOX_H
