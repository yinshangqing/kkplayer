#pragma once

#include <QWidget>
#include "ui_NetActivePageView.h"

class NetActivePageView : public QWidget
{
	Q_OBJECT

public:
	NetActivePageView(QWidget *parent = Q_NULLPTR);
	~NetActivePageView();

private:
	Ui::NetActivePageView ui;

private:
	void Init();
	void InitUi();
	void InitSignalSlots();

public:
	void OpenUrl(QString url);

private slots:
	void slot_click_netback_btn();
	void slot_click_netforward_btn();
	void slot_click_refresh_btn();
	void slot_openNewUrl(QUrl url);

protected:
	void showEvent(QShowEvent *event);
	bool eventFilter(QObject *obj, QEvent *event);
};
