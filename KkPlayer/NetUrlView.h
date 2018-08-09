#pragma once

#include <QWidget>
#include "ui_NetUrlView.h"

class NetUrlView : public QWidget
{
	Q_OBJECT

public:
	NetUrlView(QWidget *parent = Q_NULLPTR);
	~NetUrlView();

private:
	Ui::NetUrlView ui;
};
