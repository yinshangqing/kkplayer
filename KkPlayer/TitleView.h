#pragma once

#include <QWidget>
#include "ui_TitleView.h"

typedef enum{
	TITLEVIEW_ACTION_OPENFILE = 0,
	TITLEVIEW_ACTION_OPENURL,
	TITLEVIEW_ACTION_OPENDIR,
	TITLEVIEW_ACTION_SETTING,
	TITLEVIEW_ACTION_EXIT
}TITLEVIEW_ACTION_TYPE;

class TitleView : public QWidget
{
	Q_OBJECT

public:
	TitleView(QWidget *parent = Q_NULLPTR);
	~TitleView();

private:
	Ui::TitleView ui;
	//È«ÆÁÏÔÊ¾´°¿Ú
	int		mAvailableWidth, mAvailabelHeight;

private:
	void Init();
	void InitUi();
	void InitSignalSlots();

signals:
	void signal_show_maximised();
	void signal_show_minimum();
	void signal_show_normal();
	void signal_close_wnd();
	void signal_start_menu(int type);
	void signal_play_module();
	void signal_netpage_module();

private slots:
	void slot_click_minimum_btn();
	void slot_click_maximum_btn();
	void slot_click_normal_btn();
	void slot_click_close_btn();
	void slot_click_startmenu_btn();
	void slot_click_playmodule_btn();
	void slot_click_netpagemodule_btn();

	void slot_action_open_file();
	void slot_action_open_url();
	void slot_action_open_dir();
	void slot_action_setting();
	void slot_action_exit();

protected:
	void showEvent(QShowEvent *event);
	void resizeEvent(QResizeEvent *event);
	void contextMenuEvent(QContextMenuEvent *e);
};
