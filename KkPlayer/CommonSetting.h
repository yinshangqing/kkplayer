#ifndef COMMON_SETTING_H
#define COMMON_SETTING_H

#include <QObject>
#include <QSettings>
#include "Global.h"

class CommonSetting : public QObject
{
	Q_OBJECT

public:
	CommonSetting(QObject *parent = NULL);
	~CommonSetting();

	COMMON_SETTING GetCommonSettings();

private:
	//ÉèÖÃµÄÊµÀý
	QSettings	*m_pCommonSetting;
	COMMON_SETTING	mSettingStru;
	
private:
	void Init();
	void InitSignalSlots();
	void ReadSettings();
	void ReadCommonSetting();
};

#endif		//COMMON_SETTING_H