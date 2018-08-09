#pragma once

#include <QWidget>
#include "ui_UrlView.h"

class UrlView : public QWidget
{
	Q_OBJECT

public:
	UrlView(QWidget *parent = Q_NULLPTR);
	~UrlView();

private:
	Ui::UrlView ui;

private:
	void Init();
	void InitUi();
	void InitSignalSlots();

signals:
	void signal_openUrl();

private slots:
	void slot_click_refresh_btn();

protected:
	bool eventFilter(QObject *obj, QEvent *event);
	void showEvent(QShowEvent *event);
};
