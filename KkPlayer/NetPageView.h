#pragma once

#include <QWidget>
#include "ui_NetPageView.h"

typedef enum{
	NETPAGE_INACTIVE_PAGE = 0,
	NETPAGE_ACTIVE_PAGE
}NETPAGE_TYPE;

class NetPageView : public QWidget
{
	Q_OBJECT

public:
	NetPageView(QWidget *parent = Q_NULLPTR);
	~NetPageView();

private:
	Ui::NetPageView ui;
	NETPAGE_TYPE mPageType;

private:
	void Init();
	void InitUi();
	void InitSignalSlots();
	
public:
	void SwtichToPage(NETPAGE_TYPE pageType);
	void SwtichToPage(NETPAGE_TYPE pageType, QString url);

private slots:

protected:
	void showEvent(QShowEvent *event);
};
