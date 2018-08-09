#include "stdafx.h"
#include "AvPlayer.h"
#include <cmath>
#include <vector>
#include <QDebug>
#pragma comment(lib, "vlc/lib/libvlc.lib")
#pragma comment(lib, "vlc/lib/libvlccore.lib")

BOOL lock = FALSE;

CAVPlayer::CAVPlayer(void) :
m_pVLC_Inst(NULL),
m_pVLC_Player(NULL),
m_hWnd(NULL),
m_pfnPlaying(NULL),
m_pfnPosChanged(NULL),
m_pfnEndReached(NULL)
{
}

CAVPlayer::~CAVPlayer(void)
{
    Release();
}

void CAVPlayer::Init()
{
    if (! m_pVLC_Inst)
    {
        m_pVLC_Inst = libvlc_new(0, NULL);
		if (m_pVLC_Inst != NULL)
		{
			//libvlc_set_app_id(m_pVLC_Inst, "org.VideoLAN.VLC", "2.2.1","vlc");
			//libvlc_set_user_agent(m_pVLC_Inst, "VLC media player", "VLC/""2.2.1");
			//libvlc_add_intf(m_pVLC_Inst, "hotkeys,none");
			//libvlc_add_intf(m_pVLC_Inst, "globalhotkeys,none");
			//libvlc_add_intf(m_pVLC_Inst, NULL);
			//libvlc_playlist_play(m_pVLC_Inst, -1, 0, NULL);
			//libvlc_wait(m_pVLC_Inst);
			//libvlc_release(m_pVLC_Inst);
		}
		else
			MessageBox(NULL, TEXT("VLC media player could not start.\nEither the command line options were invalid or no plugins were found.\n"),
			TEXT("VLC media player"),
			MB_OK | MB_ICONERROR);
    }
}

void CAVPlayer::Release()
{
    Stop();

    if (m_pVLC_Inst)
    {
        libvlc_release (m_pVLC_Inst);
        m_pVLC_Inst   = NULL;
    }
}

bool CAVPlayer::Play(const std::string &strPath, int nwidth,int nheight)
{
    if (! m_pVLC_Inst)
    {
        Init();
    }

    if(strPath.empty() || ! m_pVLC_Inst)
    {
        return false;
    }

    // 验证地址是网络地址，还是本地地址
    bool bURL = false;
    std::vector<std::string> vctURL;

    vctURL.push_back("http"); 
    vctURL.push_back("https"); 
    vctURL.push_back("ftp");
    vctURL.push_back("rstp");

    for (unsigned i = 0; i < vctURL.size(); i++)
    {
        // 实际使用请判断大小写
        if (! strPath.compare(0, vctURL[i].size(), vctURL[i]))
        {
            bURL = true;
            break;
        }
    }

    Stop();

    bool bRet = false;

    if (bURL)
    {
        // 示例网络地址：http://zhangmenshiting.baidu.com/data2/music/1203175/51597064800128.mp3?xcode=cb97ecc9d44ba29122881c69e505a43849f04e8309ecc859
        m = libvlc_media_new_location(m_pVLC_Inst, strPath.c_str());
    } 
    else
    {
        m = libvlc_media_new_path(m_pVLC_Inst, strPath.c_str());
    }

    if (m)
    {
		if (m_pVLC_Player = libvlc_media_player_new_from_media(m))
        {
            libvlc_media_player_set_hwnd(m_pVLC_Player, m_hWnd);
            libvlc_media_player_play(m_pVLC_Player);
			//libvlc_video_set_key_input(m_pVLC_Player, true);
            // 事件管理
            libvlc_event_manager_t *vlc_evt_man = libvlc_media_player_event_manager(m_pVLC_Player);
            libvlc_event_attach(vlc_evt_man, libvlc_MediaPlayerPlaying, ::OnVLC_Event, this);
            libvlc_event_attach(vlc_evt_man, libvlc_MediaPlayerPositionChanged, ::OnVLC_Event, this);
            libvlc_event_attach(vlc_evt_man, libvlc_MediaPlayerEndReached, ::OnVLC_Event, this);
            bRet = true;
        }

        libvlc_media_release(m);
    }

    return bRet;
}

void CAVPlayer::Stop()
{
    if (m_pVLC_Player)
    {
        libvlc_media_player_stop (m_pVLC_Player);      /* Stop playing */    
        libvlc_media_player_release (m_pVLC_Player);   /* Free the media_player */
        m_pVLC_Player = NULL;
    }
}

void CAVPlayer::Play()
{
    if (m_pVLC_Player)
    {
        libvlc_media_player_play(m_pVLC_Player);
    }
}

void CAVPlayer::Pause()
{
    if (m_pVLC_Player)
    {
		libvlc_media_player_set_pause(m_pVLC_Player, 1);
    }
}

void CAVPlayer::Resume()
{
	if (m_pVLC_Player)
	{
		libvlc_media_player_set_pause(m_pVLC_Player, 0);
	}
}

int CAVPlayer::Volume(int iVol)
{
    if (iVol < 0)
    {
        return -1;
    }

    if (m_pVLC_Player)
    {
        return libvlc_audio_set_volume(m_pVLC_Player,int(iVol * 2)); // 如果放到100，感觉比迅雷的100少了30，所以这里用1.3倍音量
    }
	return -1;
}

void CAVPlayer::VolumeIncrease()
{
    if (m_pVLC_Player)
    {
        int iVol = libvlc_audio_get_volume(m_pVLC_Player);
        Volume((int)ceil(iVol * 1.1));
    }
}

void CAVPlayer::VolumeReduce()
{
    if (m_pVLC_Player)
    {
        int iVol = libvlc_audio_get_volume(m_pVLC_Player);
        Volume((int)floor(iVol * 0.9));
    }
}

int CAVPlayer::GetPos()
{   
    if (m_pVLC_Player)
    {
        return (int)(1000 * libvlc_media_player_get_position(m_pVLC_Player));
    }

    return 0;
}

void CAVPlayer::SeekTo(float iPos)
{
    if (iPos < 0 || iPos > 1)
    {
        return;
    }

    if (m_pVLC_Player)
    {
        libvlc_media_player_set_position(m_pVLC_Player, iPos);
    }
}

void CAVPlayer::SeekForward()
{
    // 一次快退5秒
    if (m_pVLC_Player)
    {
        libvlc_time_t i_time = libvlc_media_player_get_time(m_pVLC_Player) + 5000;

        if (i_time > GetTotalTime())
        {
            i_time = GetTotalTime();
        }

        libvlc_media_player_set_time(m_pVLC_Player, i_time);
    }
}

void CAVPlayer::SeekBackward()
{
    if (m_pVLC_Player)
    {
        libvlc_time_t i_time = libvlc_media_player_get_time(m_pVLC_Player) - 5000;

        if (i_time < 0)
        {
            i_time = 0;
        }

        libvlc_media_player_set_time(m_pVLC_Player, i_time);
    }
}

void CAVPlayer::SeekForward(ENUM_FORWARD_TYPE nX)
{
	if (nullptr == m_pVLC_Player)
		return;

	long nXX = 1000;
	switch(nX)
	{
	case ENUM_FORWARD_X1:
		return;
		nXX = 1000 * 0;
		break;
	case ENUM_FORWARD_X2:
		nXX = 1000 * 1;
		break;
	case ENUM_FORWARD_X4:
		nXX = 1000 * 3;
		break;
	case ENUM_FORWARD_X8:
		nXX = 1000 * 7;
		break;
	case ENUM_FORWARD_X16:
		nXX = 1000 * 15;
		break;
	}

	libvlc_time_t i_time = libvlc_media_player_get_time(m_pVLC_Player) + nXX;

	if (i_time > GetTotalTime())
	{
		i_time = GetTotalTime();
	}

	libvlc_media_player_set_time(m_pVLC_Player, i_time);
}

void CAVPlayer::SeekBackward(ENUM_REVERSE_TYPE nX)
{
	if (nullptr == m_pVLC_Player)
		return;

	long nXX = 1000;
	switch(nX)
	{
	case ENUM_REVERSE_X1:
		nXX = 1000 * 2;
		break;
	case ENUM_REVERSE_X2:
		nXX = 1000 * 3;
		break;
	case ENUM_REVERSE_X4:
		nXX = 1000 * 5;
		break;
	case ENUM_REVERSE_X8:
		nXX = 1000 * 9;
		break;
	case ENUM_REVERSE_X16:
		nXX = 1000 * 16;
		break;
	}
	if (m_pVLC_Player)
	{
		libvlc_time_t i_time = libvlc_media_player_get_time(m_pVLC_Player) - nXX;

		if (i_time < 0)
		{
			i_time = 0;
		}

		libvlc_media_player_set_time(m_pVLC_Player, i_time);
	}
}

int CAVPlayer::SnapShoot(const char *psz_filepath)
{
	if (NULL != m_pVLC_Player && psz_filepath != "")
		return libvlc_video_take_snapshot(m_pVLC_Player, 0, psz_filepath, 0, 0);
	else
		return -1;
}

int CAVPlayer::GetSubtitleCount()
{
	int nCnt;
	nCnt = libvlc_video_get_spu(m_pVLC_Player);
	return nCnt;
}

void CAVPlayer::GetSubtitles(libvlc_track_description_t* subtitles)
{
	// 解析媒体实例
	libvlc_media_parse(m);
	// 获取媒体文件的播放长度,  返回 ms
	libvlc_time_t duration = libvlc_media_get_duration(m);
	char *str = libvlc_media_get_meta(m, libvlc_meta_Description);
	qDebug() << str;
	// 此处是获取媒体包含多个的视频和音频轨以及其他类型的轨道信息
	libvlc_media_track_info_t *media_tracks_info = NULL;
	libvlc_media_track_t **media_tracks = NULL;
	int trackCount = libvlc_media_get_tracks_info(m, &media_tracks_info);
	unsigned tracksNt = libvlc_media_tracks_get(m, &media_tracks);
	// 释放内存
	libvlc_media_tracks_release(media_tracks, tracksNt);
	SetSubtitile(true);
	// 把打开的媒体文件设置给播放器
	//libvlc_media_player_set_media(m_pVLC_Player, m);
}

void CAVPlayer::SetSubtitile(bool open)
{
	if (open)
	{
		
		int nTrack = libvlc_video_get_spu(m_pVLC_Player);
		qDebug() << "current video subtitle track = "<<nTrack << "\n";
		libvlc_video_set_spu(m_pVLC_Player, 1);
		//libvlc_media_player_set_video_title_display(m_pVLC_Player, libvlc_position_t::libvlc_position_center, 300);
	}
	else
	{
		libvlc_video_set_spu(m_pVLC_Player, 0);
		//libvlc_media_player_set_video_title_display(m_pVLC_Player, libvlc_position_t::libvlc_position_disable, 300);
	}
}

void CAVPlayer::SetHWND(HWND hwnd)
{
    if (::IsWindow(hwnd))
    {
        m_hWnd = hwnd;
    }
}

HWND CAVPlayer::GetHWND()
{
    return m_hWnd;
}

bool CAVPlayer::IsOpen()
{
    return NULL != m_pVLC_Player;
}

bool CAVPlayer::IsPlaying()
{
    if (m_pVLC_Player)
    {
        return (1 == libvlc_media_player_is_playing(m_pVLC_Player));
    }

    return false;
}

__int64 CAVPlayer::GetTotalTime()
{
    if (m_pVLC_Player)
    {
        return libvlc_media_player_get_length(m_pVLC_Player);
    }

    return 0;
}

__int64 CAVPlayer::GetTime()
{
    if (m_pVLC_Player)
    {
        return libvlc_media_player_get_time(m_pVLC_Player);
    }

    return 0;
}

int CAVPlayer::GetVolume()
{
    if (m_pVLC_Player)
    {
        return libvlc_audio_get_volume(m_pVLC_Player);
    }

    return 0;
}

void CAVPlayer::SetCallbackPlaying( pfnCallback pfn )
{
    m_pfnPlaying = pfn;
}

void CAVPlayer::SetCallbackPosChanged( pfnCallback pfn )
{
    m_pfnPosChanged = pfn;
}

void CAVPlayer::SetCallbackEndReached( pfnCallback pfn )
{
    m_pfnEndReached = pfn;
}

void OnVLC_Event( const libvlc_event_t *event, void *data )
{
    CAVPlayer *pAVPlayer = (CAVPlayer *) data;
    pfnCallback pfn = NULL;

    if (! pAVPlayer)
    {
        return;
    }

    switch(event->type)
    {
    case libvlc_MediaPlayerPlaying:
        pfn = pAVPlayer->m_pfnPlaying;
        break;
    case libvlc_MediaPlayerPositionChanged:
        pfn = pAVPlayer->m_pfnPosChanged;
        break;
    case libvlc_MediaPlayerEndReached:
        pfn = pAVPlayer->m_pfnEndReached;
        break;
    default:
        break;
    }

    if (pfn)
    {
        pfn(data);  // 此回调函数还可以传入其他参数，除了data外，还有event的各种信息（如event->u.media_player_position_changed.new_position）等等，请自行扩展。
    }   
}
