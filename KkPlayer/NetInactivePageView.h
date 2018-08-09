#pragma once

#include <QWidget>
#include "ui_NetInactivePageView.h"

class NetInactivePageView : public QWidget
{
	Q_OBJECT

public:
	NetInactivePageView(QWidget *parent = Q_NULLPTR);
	~NetInactivePageView();

private:
	Ui::NetInactivePageView ui;

private:
	void Init();
	void InitUi();
	void InitSignalSlots();

protected:
	void showEvent(QShowEvent *event);
};
