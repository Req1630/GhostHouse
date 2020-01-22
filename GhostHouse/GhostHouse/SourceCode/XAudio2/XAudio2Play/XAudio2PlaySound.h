#ifndef C_XAUDIO2_PLAYSOUND_H
#define C_XAUDIO2_PLAYSOUND_H


#include <vector>

#include "..\XAudio2.h"
#include "..\XAudio2WaveLoad\XAudio2WaveLoad.h"
#include "..\XAudio2Master\XAudio2MasterVoice.h"

struct IXAudio2SourceVoice;

class clsXAudio2PlaySound
{
public:
	// コンストラクタ.
	clsXAudio2PlaySound();

	// デストラクタ.
	virtual ~clsXAudio2PlaySound();

	// ループさせるかどうか取得関数.
	bool IsLoop()const { return m_isLoop; }

	// 再生してるかどうか取得関数.
	bool IsPlaying()const;

	// サウンド再生、前に一時停止されていた場合は記憶位置から再開.
	virtual bool Play(const char* filename, bool& isEnd, bool& isEmergencyCall);

	// サウンド再生、前に一時停止されていた場合は記憶位置から再開.
	virtual bool SEPlay(
		const char* filename, bool& isEnd,bool& Reset, bool& IsPlay, bool& RePlaySE, bool& isEmergencyCall);

	// サウンド再生、前に一時停止されていた場合は記憶位置から再開.
	virtual bool LoopPlay(const char* filename, bool& isEnd, bool& isEmergencyCall);

	// サウンドを完全停止.(位置を保持しない).
	bool Stop();

	// サウンド一時停止.
	bool Pause();

	// 現在のループでサウンドループを終了.
	bool ExitLoop();

	// ピッチ設定関数.
	bool SetPitch(float value);

	// ピッチを取得関数.
	float GetPitch()const;

	// 現在のピッチから加算または減算.
	bool AdjustPitch(float value);

	// 最大音量を設定します.
	void  SetMaxPitch(float value);

	// wavファイルから音源データを読み込む.
	bool CreateSound(std::shared_ptr< clsXAudio2WaveLoad> pWavData, const char* filename);

	// 音量を取得します.
	float GetVolume()const;

	// 音量を設定します.
	bool  SetVolume(float value);

	// 最大音量を設定します.
	void  SetMaxVolume(float value);

	// 現在の音量から加算または減算.
	bool  AdjustVolume(float value);

	// Reverb(反響エフェクト)の有効フラグを設定します.
	bool UsingReverb(bool flag);

	bool FadeOutBGM( float value, bool& isEmergencyCall );

	bool FadeInBGM( float value, bool& isEmergencyCall );

	void StopSource();

	void DestoroySource();

	void SetFadeOutFlag(bool BGMFadeOutFlag) { m_FadeOutStart = BGMFadeOutFlag; }
	
	bool GetFadeOutFlag() { return m_FadeOutStart; }

	void SetFadeInFlag(bool BGMFadeInFlag) { m_FadeInStart = BGMFadeInFlag; }

	bool GetFadeInFlag() { return m_FadeInStart; }

protected:

	//! 音源データをストリームに流し込みます.
	//  この関数は外部から呼び出せません.
	bool Submit(std::shared_ptr<clsXAudio2WaveLoad> pWaveData, const char* filename);

protected:

	//! 変数
	clsXAudio2WaveLoad*       m_pOriginData;  //  サウンド音源データ
	IXAudio2SourceVoice*   m_pSourceVoice; //  サウンドを再生するストリーム
	//std::shared_ptr<clsXAudio2MasterVoice> m_pMaster;
	size_t nextFirstSample;
	size_t submitCount;
	bool                    m_isLoop;       //  ループフラグ
	float                   m_maxVolume;    //  最大音量
	float                   m_maxPitch;     //  最大ピッチ
	bool					m_bStop;
	std::vector< BYTE >		m_Primary;
	std::vector< BYTE >		m_Secondary;

	bool					m_FadeOutStart;
	bool					m_FadeInStart;

	float					m_fDownVolume;
};


#endif // !C_XAUDIO2_PLAYSOUND_H