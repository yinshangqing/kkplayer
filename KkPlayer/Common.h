#ifndef COMMON_H 
#define COMMON_H

#include <QString>

namespace Kk
{
	typedef enum{
		ENUM_PLAYSTATUS_PLAYING = 0,	//正在播放
		ENUM_PLAYSTATUS_PAUSE,			//暂停播放
		ENUM_PLAYSTATUS_STOP			//停止播放
	}ENUM_PLAYSTATUS_TYPE;

	typedef enum{
		ENUM_FORWARD_X1 = 0,
		ENUM_FORWARD_X2,
		ENUM_FORWARD_X4,
		ENUM_FORWARD_X8,
		ENUM_FORWARD_X16
	} ENUM_FORWARD_TYPE;

	typedef enum{
		ENUM_REVERSE_X1 = 0,
		ENUM_REVERSE_X2,
		ENUM_REVERSE_X4,
		ENUM_REVERSE_X8,
		ENUM_REVERSE_X16
	} ENUM_REVERSE_TYPE;

	typedef enum{
		MODULE_PAGE_PLAY = 0,	//播放界面
		MODULE_PAGE_NETPAGE		//我的网页
	}MODULE_PAGE_ENUM;

	typedef enum{
		ENUM_TOOLBUTTON_IDLE = 0,
		ENUM_TOOLBUTTON_SELECT
	}ENUM_TOOLBUTTON_STYLE_STATE;

	typedef struct{
		std::string videoId;
		std::string videoName;
		std::string videoFilePath;
		int videoPlayTimeStamp;
		std::string setId;
	}VIDEO_TREE_ITEM;
}

#define	DELAY_10_DAY									(1*60*60*24*10)
#define	DELAY_1_DAY									(1*60*60*24)
#define HORIZONTAL_SCROLLBAR_TYPE		0		//横向滚动条
#define VERTICAL_SCROLLBAR_TYPE			1		//纵向滚动条

#endif // COMMON_H