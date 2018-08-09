#pragma once

#include <QWidget>
#include "ui_SettingMenuWnd.h"

typedef enum{
	SETTING_MENU_CHANGGUI = 0,
	SETTING_MENU_BOFANG,
	SETTING_MENU_SHENYIN,
	SETTING_MENU_ZIMU,
	SETTING_MENU_HUAMIAN,
	SETTING_MENU_JIETU
}SETTING_MENU_TYPE;

class SettingMenuWnd : public QWidget
{
	Q_OBJECT

public:
	SettingMenuWnd(QWidget *parent = Q_NULLPTR);
	~SettingMenuWnd();

private:
	Ui::SettingMenuWnd ui;
	SETTING_MENU_TYPE	mCurMenuType;
	//ÒÆ¶¯´°Ìå
	QPoint				mWindowPos, mMousePos, mPosDelta;
	bool				mIsPressed;

private:
	void Init();
	void InitUi();
	void InitSignalSlots();

private slots:
	void slot_click_changgui_btn();
	void slot_click_bofang_btn();
	void slot_click_shenyin_btn();
	void slot_click_zimu_btn();
	void slot_click_huamian_btn();
	void slot_click_jietu_btn();
	void slot_click_queren();
	void slot_click_yingyong();
	void slot_click_quxiao();
	void slot_click_close();
	void slot_click_liulanjietu();

protected:
	void showEvent(QShowEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
};
