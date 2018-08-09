#pragma once

#include <QWidget>
#include "ui_PlayActivePageView.h"

typedef enum{
	PLAYACTIVE_OPEN_URL,
	PLAYACTIVE_OPEN_DIR,
	PLAYACTIVE_OPEN_BT
}PLAYACTIVE_OPENMENU_TYPE;

class PlayActivePageView : public QWidget
{
	Q_OBJECT

public:
	PlayActivePageView(QWidget *parent = Q_NULLPTR);
	~PlayActivePageView();

private:
	Ui::PlayActivePageView ui;

private:
	void Init();
	void InitUi();
	void InitSignalSlots();

public:
	void SetActiveMode(bool active);

signals:
	void signal_openvideobtn_clicked();
	void signal_openmenubtn_clicked(int menuType);

private slots:
	void slot_click_openfile_btn();
	void slot_click_openmenu_btn();
	void slot_click_openurl_btn();
	void slot_click_opendir_btn();
	void slot_click_openbt_btn();

protected:
	void showEvent(QShowEvent *event);
};
