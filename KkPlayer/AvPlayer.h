#ifndef __AVPlayer_H__
#define __AVPlayer_H__
#include <iostream>
#include <Windows.h>
#include "vlc.h"
#include "Common.h"

typedef void (* pfnCallback) (void *data);  // VLC事件的回调函数指针
struct libvlc_instance_t;
struct libvlc_media_player_t;
struct libvlc_event_t;
struct libvlc_track_description_t;

using namespace Kk;

class CAVPlayer
{
    // VLC的事件管理
    friend void OnVLC_Event(const libvlc_event_t *event, void *data);

public:
    CAVPlayer(void);
    ~CAVPlayer(void);

	static unsigned fVlcDecoderFrameCB(void **opaque, char *chroma, unsigned *width, unsigned *height, unsigned *pitches, unsigned *lines);
	bool Play(const std::string &strPath, int nwidth=0, int nheight=0);  // 播放路径为strPath的文件
    void Play ();                           // 播放
    void Pause();                           // 暂停
    void Stop ();                           // 停止
	void Resume();							// 继续播放

    int Volume(int iVol);                  // 音量设置为iVol
    void VolumeIncrease();                  // 音量增大
    void VolumeReduce();                    // 音量减小 

    void SeekTo(float iPos);                // 跳到指定位置iPos
    void SeekForward();                     // 快进
	void SeekForward(ENUM_FORWARD_TYPE);                     // 快进x1, x2, x4, x8, x16
    void SeekBackward();                    // 快退
	void SeekBackward(ENUM_REVERSE_TYPE);                    // 快退x1, x2, x4, x8, x16

	int  SnapShoot(const char *psz_filepath);//截图

	int	 GetSubtitleCount();				//字幕个数
	void GetSubtitles(libvlc_track_description_t* subtitles);					//获取字幕
	void SetSubtitile(bool open);

	void SetHWND(HWND hwnd);                // 设置视频显示的窗口句柄
    HWND GetHWND();                         // 获取视频显示的窗口句柄

    bool    IsOpen();                       // 文件是否打开
    bool    IsPlaying();                    // 文件是否正在播放
    int     GetPos();                       // 获取文件当前播放的位置
    __int64 GetTotalTime();                 // 获取总时间
    __int64 GetTime();                      // 获取时间
    int     GetVolume();                    // 获取音量

    void SetCallbackPlaying(pfnCallback pfn);         // 设置文件头读取完毕时的回调函数
    void SetCallbackPosChanged(pfnCallback pfn);      // 设置文件位置改变时的回调函数
    void SetCallbackEndReached(pfnCallback pfn);      // 设置文件头读取完毕时的回调函数

private:
    libvlc_instance_t       *m_pVLC_Inst;   // VLC实例
    libvlc_media_player_t   *m_pVLC_Player; // VLC播放器
	libvlc_media_t *m;
    HWND                    m_hWnd;         // 视频显示的窗口句柄
    pfnCallback             m_pfnPlaying;   // 文件读取完毕，准备播放
    pfnCallback             m_pfnPosChanged;// 文件位置改变时的回调函数
    pfnCallback             m_pfnEndReached;// 文件播放完毕的回调函数
	int						iVideoWidth_;
	int						iVideoHeight_;

    void Init();                            // 初始化

public:
    void Release();                         // 清理内存
};

#endif
