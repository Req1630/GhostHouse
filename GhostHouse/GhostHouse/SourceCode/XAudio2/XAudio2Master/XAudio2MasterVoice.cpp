#include "XAudio2.h"

#include "XAudio2MasterVoice.h"

static bool OneCall = true;

clsXAudio2MasterVoice& clsXAudio2MasterVoice::GetGlobalSystem()
{
	static clsXAudio2MasterVoice xAudioMaster;
	return xAudioMaster;
}

clsXAudio2MasterVoice::clsXAudio2MasterVoice()
	//:m_pXAudioInterface(nullptr),
	//m_pMasterVoice(nullptr)
{
	Initialize();
}

clsXAudio2MasterVoice::~clsXAudio2MasterVoice()
{
	//clsXAudio2MasterVoice::Release();
	//Release();
}

// XAudio取得.
IXAudio2* clsXAudio2MasterVoice::GetInterface()const
{
	return m_pXAudioInterface;
}

// マスターボイス取得.
IXAudio2MasteringVoice* clsXAudio2MasterVoice::GetMasteringVoice()const
{
	return m_pMasterVoice;
}

//初期化 XAudio作成 マスターボイス作成.
bool clsXAudio2MasterVoice::Initialize()
{
	//if (OneCall == true) {
		// COM の初期化
		if (FAILED(CoInitializeEx(NULL, COINIT_MULTITHREADED)) &&
			FAILED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED)))
		{
			//todo debug output string
			return false;
		}


	// XAudio2 インターフェースの作成
	if (FAILED(XAudio2Create(&m_pXAudioInterface, 0)))
		return false;

	// マスターボイスの作成
	if (FAILED(m_pXAudioInterface->CreateMasteringVoice(&m_pMasterVoice)))		
		return FALSE;


		OneCall = false;	
	//}


	return true;
}

void clsXAudio2MasterVoice::Release()
{
	if (GetInstance()->m_pMasterVoice == nullptr) return;
	GetInstance()->m_pMasterVoice->DestroyVoice();
	GetInstance()->m_pMasterVoice = nullptr;
	COM_SAFE_RELEASE(GetInstance()->m_pXAudioInterface);
	GetInstance()->m_pXAudioInterface = nullptr;
	//CoUninitialize();
}

void clsXAudio2MasterVoice::CloseCOM()
{
	//CoUninitialize();
}
