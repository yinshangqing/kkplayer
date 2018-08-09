#include "stdafx.h"
#include "CommonSetting.h"
#include "Common.h"

const QString SettingFile = "/setting.ini";
QString	gLastDir = "";

CommonSetting::CommonSetting(QObject *parent)
	: QObject(parent),
	m_pCommonSetting(NULL)
{
	Init();
	InitSignalSlots();
	ReadSettings();
}

CommonSetting::~CommonSetting()
{
	if (m_pCommonSetting)
	{
		if (!gLastDir.isEmpty())
			m_pCommonSetting->setValue("LastDir", QVariant(gLastDir));
		delete m_pCommonSetting;
		m_pCommonSetting = 0;
	}
}

void CommonSetting::Init()
{
	QString appPath = QApplication::applicationDirPath();
	QString tmpAppPath = appPath;
	tmpAppPath.append(SettingFile);
	//LOG(INFO) << "cmd config file path = " << tmpAppPath.toLocal8Bit().data();
	if (NULL == m_pCommonSetting)
		m_pCommonSetting = new QSettings(tmpAppPath, QSettings::IniFormat, this);

	m_pCommonSetting->setIniCodec("UTF8");
}

void CommonSetting::InitSignalSlots()
{

}

void CommonSetting::ReadSettings()
{
	ReadCommonSetting();
}

void CommonSetting::ReadCommonSetting()
{
	if (m_pCommonSetting)
	{
		if (m_pCommonSetting->contains("LastDir"))
		{
			mSettingStru.lastDir = m_pCommonSetting->value("LastDir").toString();
			gLastDir = mSettingStru.lastDir;
		}
	}
}

COMMON_SETTING CommonSetting::GetCommonSettings()
{
	return mSettingStru;
}