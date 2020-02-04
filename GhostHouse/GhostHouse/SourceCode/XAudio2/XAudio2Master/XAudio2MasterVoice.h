#ifndef C_XAUDIO2_MASTERVOICE_H
#define C_XAUDIO2_MASTERVOICE_H

#include "XAudio2.h"
#include "..\..\SingletonBase\SingletonBase.h"
#define COM_SAFE_RELEASE( p ) { if(p) { (p)->Release(); (p) = nullptr; } }

class clsXAudio2MasterVoice : public std::CSingletonBase<clsXAudio2MasterVoice>
{
public:	
	clsXAudio2MasterVoice();
	~clsXAudio2MasterVoice();

	IXAudio2* GetInterface()const;
	// マスターボイス取得.
	IXAudio2MasteringVoice* GetMasteringVoice()const;
	static clsXAudio2MasterVoice& GetGlobalSystem();
	// 開放関数.
	static void Release();
protected:

	bool Initialize();

	bool Crean;

	IXAudio2 *              m_pXAudioInterface;                //XAudio2インターフェース
	IXAudio2MasteringVoice *m_pMasterVoice;                    //マスタボイス
private:

};




#endif //#ifndef C_XAUDIO2_MASTERVOICE_H
