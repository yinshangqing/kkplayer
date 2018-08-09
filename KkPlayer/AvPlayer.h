#ifndef __AVPlayer_H__
#define __AVPlayer_H__
#include <iostream>
#include <Windows.h>
#include "vlc.h"
#include "Common.h"

typedef void (* pfnCallback) (void *data);  // VLC�¼��Ļص�����ָ��
struct libvlc_instance_t;
struct libvlc_media_player_t;
struct libvlc_event_t;
struct libvlc_track_description_t;

using namespace Kk;

class CAVPlayer
{
    // VLC���¼�����
    friend void OnVLC_Event(const libvlc_event_t *event, void *data);

public:
    CAVPlayer(void);
    ~CAVPlayer(void);

	static unsigned fVlcDecoderFrameCB(void **opaque, char *chroma, unsigned *width, unsigned *height, unsigned *pitches, unsigned *lines);
	bool Play(const std::string &strPath, int nwidth=0, int nheight=0);  // ����·��ΪstrPath���ļ�
    void Play ();                           // ����
    void Pause();                           // ��ͣ
    void Stop ();                           // ֹͣ
	void Resume();							// ��������

    int Volume(int iVol);                  // ��������ΪiVol
    void VolumeIncrease();                  // ��������
    void VolumeReduce();                    // ������С 

    void SeekTo(float iPos);                // ����ָ��λ��iPos
    void SeekForward();                     // ���
	void SeekForward(ENUM_FORWARD_TYPE);                     // ���x1, x2, x4, x8, x16
    void SeekBackward();                    // ����
	void SeekBackward(ENUM_REVERSE_TYPE);                    // ����x1, x2, x4, x8, x16

	int  SnapShoot(const char *psz_filepath);//��ͼ

	int	 GetSubtitleCount();				//��Ļ����
	void GetSubtitles(libvlc_track_description_t* subtitles);					//��ȡ��Ļ
	void SetSubtitile(bool open);

	void SetHWND(HWND hwnd);                // ������Ƶ��ʾ�Ĵ��ھ��
    HWND GetHWND();                         // ��ȡ��Ƶ��ʾ�Ĵ��ھ��

    bool    IsOpen();                       // �ļ��Ƿ��
    bool    IsPlaying();                    // �ļ��Ƿ����ڲ���
    int     GetPos();                       // ��ȡ�ļ���ǰ���ŵ�λ��
    __int64 GetTotalTime();                 // ��ȡ��ʱ��
    __int64 GetTime();                      // ��ȡʱ��
    int     GetVolume();                    // ��ȡ����

    void SetCallbackPlaying(pfnCallback pfn);         // �����ļ�ͷ��ȡ���ʱ�Ļص�����
    void SetCallbackPosChanged(pfnCallback pfn);      // �����ļ�λ�øı�ʱ�Ļص�����
    void SetCallbackEndReached(pfnCallback pfn);      // �����ļ�ͷ��ȡ���ʱ�Ļص�����

private:
    libvlc_instance_t       *m_pVLC_Inst;   // VLCʵ��
    libvlc_media_player_t   *m_pVLC_Player; // VLC������
	libvlc_media_t *m;
    HWND                    m_hWnd;         // ��Ƶ��ʾ�Ĵ��ھ��
    pfnCallback             m_pfnPlaying;   // �ļ���ȡ��ϣ�׼������
    pfnCallback             m_pfnPosChanged;// �ļ�λ�øı�ʱ�Ļص�����
    pfnCallback             m_pfnEndReached;// �ļ�������ϵĻص�����
	int						iVideoWidth_;
	int						iVideoHeight_;

    void Init();                            // ��ʼ��

public:
    void Release();                         // �����ڴ�
};

#endif
