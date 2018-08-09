#pragma once

#include <QWidget>
#include "ui_PlayInactivePageView.h"

class PlayInactivePageView : public QWidget
{
	Q_OBJECT

public:
	PlayInactivePageView(QWidget *parent = Q_NULLPTR);
	~PlayInactivePageView();

private:
	Ui::PlayInactivePageView ui;

private:
	void Init();
	void InitUi();
	void InitSignalSlots();

public:
	void SwtichToNetPage();
	void SwtichToNetPage(QString url);

protected:
	void showEvent(QShowEvent *event);
};
