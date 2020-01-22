#include "XAudio2PlaySound.h"
#include "XAudio2.h"
#include "..\XAudio2Master\XAudio2MasterVoice.h"
#include <vector>
#include <process.h>
static bool aaa = false;
clsXAudio2PlaySound::clsXAudio2PlaySound()
	:m_pSourceVoice(nullptr),
	m_pOriginData(nullptr),
	m_isLoop(false),
	m_maxVolume(1.0f),
	m_maxPitch(1.4f),	//5.0fマックスかと思われます.
	nextFirstSample(0),
	submitCount(0),
	m_bStop(false),
	m_fDownVolume( 0.0f ),
	m_FadeInStart( false ),
	m_FadeOutStart( false )
{
	SetVolume(1.0f);
}

clsXAudio2PlaySound::~clsXAudio2PlaySound()
{
}

// 音源データをバッファに入れる.
bool clsXAudio2PlaySound::Submit(std::shared_ptr<clsXAudio2WaveLoad> pWaveData, const char* filename)
{
	nextFirstSample = 0;
	submitCount = 0;
	
	// プライマリバッファ
	m_Primary = std::vector< BYTE >(pWaveData->GetWaveFmtEx().nAvgBytesPerSec * 3);
	if (nextFirstSample < m_pOriginData->GetSamples(filename))
	{
		std::size_t readSamples = m_pOriginData->ReadRaw(nextFirstSample, pWaveData->GetWaveFmtEx().nSamplesPerSec * 3, &(m_Primary[0]),filename);
		if (readSamples > 0)
		{
			XAUDIO2_BUFFER bufferDesc = { 0 };
			bufferDesc.Flags = nextFirstSample + readSamples >= m_pOriginData->GetSamples(filename) ? XAUDIO2_END_OF_STREAM : 0;
			bufferDesc.AudioBytes = readSamples * pWaveData->GetWaveFmtEx().nBlockAlign;
			bufferDesc.pAudioData = &(m_Primary[0]);

			m_pSourceVoice->SubmitSourceBuffer(&bufferDesc);

			nextFirstSample += readSamples;
			++submitCount;
		}
	}

	// セカンダリバッファ
	m_Secondary = std::vector< BYTE >(pWaveData->GetWaveFmtEx().nAvgBytesPerSec * 3);


	//XAUDIO2_BUFFER buffer;
	//memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	//buffer.AudioBytes = m_pOriginData->GetWaveSize();
	//buffer.pAudioData = m_pOriginData->GetWaveData();
	//buffer.LoopCount = IsLoop() ? XAUDIO2_LOOP_INFINITE : 0;

	//if (m_pSourceVoice)
	//	m_pSourceVoice->SubmitSourceBuffer(&buffer);

	return true;
}

bool clsXAudio2PlaySound::IsPlaying()const
{
	if (m_pSourceVoice == nullptr) return 0;
	XAUDIO2_VOICE_STATE xState;
	m_pSourceVoice->GetState(&xState);

	return xState.BuffersQueued != 0;
}


// 再生.
bool clsXAudio2PlaySound::Play(const char* filename, bool& isEnd, bool& isEmergencyCall)
{
	static float TST = 1.0f;
	m_pSourceVoice->Start(0);

	while (1)
	{
		if (isEnd == true || isEmergencyCall == true) {
			break;
		}

		XAUDIO2_VOICE_STATE state;
		if (m_pSourceVoice == nullptr) break;
		m_pSourceVoice->GetState(&state);

		if (state.BuffersQueued == 0 && nextFirstSample >= m_pOriginData->GetSamples(filename))
		{
			// すべて再生し終わっている
			break;
		}
		else if (state.BuffersQueued < 2 && nextFirstSample < m_pOriginData->GetSamples(filename))
		{
			if (isEnd == true || isEmergencyCall == true) {
				break;
			}
			// キューにバッファを追加
			std::vector< BYTE > & buffer = submitCount & 1 ? m_Secondary : m_Primary;

			std::size_t readSamples = m_pOriginData->ReadRaw(nextFirstSample, m_pOriginData->GetWaveFmtEx().nSamplesPerSec * 3, &(buffer[0]),filename);
			if (readSamples > 0)
			{
				if (isEnd == true || isEmergencyCall == true) {
					break;
				}
				XAUDIO2_BUFFER bufferDesc = { 0 };
				bufferDesc.Flags = nextFirstSample + readSamples >= m_pOriginData->GetSamples(filename) ? XAUDIO2_END_OF_STREAM : 0;
				bufferDesc.AudioBytes = readSamples * m_pOriginData->GetWaveFmtEx().nBlockAlign;
				bufferDesc.pAudioData = &(buffer[0]);

				m_pSourceVoice->SubmitSourceBuffer(&bufferDesc);

				nextFirstSample += readSamples;
				++submitCount;
			}
		}
		
		// 過負荷にならないよう調整
		Sleep(1);
	}
	return true;
}

bool clsXAudio2PlaySound::SEPlay(
	const char * filename, bool & isEnd, bool& Reset, bool& IsPlay,bool& RePlaySE, bool& isEmergencyCall)
{
	m_bStop = false;
	IsPlay = true;

	if (Reset == true) {
		XAUDIO2_VOICE_STATE state;
		m_pSourceVoice->GetState(&state);
		m_pSourceVoice->FlushSourceBuffers();
		nextFirstSample = 0;
		Reset = false;
		RePlaySE = false;
	}
	//SetPitch(2.0f);
	static float TST = 1.0f;
	SetVolume(1.0f);
	m_pSourceVoice->Start(0);

	while (1)
	{
		if (isEmergencyCall == true) break;
		if (RePlaySE == true) {
			Stop();
			break;
		}
		XAUDIO2_VOICE_STATE state;
			if (m_pSourceVoice == nullptr) break;
			m_pSourceVoice->GetState(&state);


		if (state.BuffersQueued == 0 && nextFirstSample >= m_pOriginData->GetSamples(filename))
		{
			// すべて再生し終わっている
			break;
		}
		else if (state.BuffersQueued < 2 && nextFirstSample < m_pOriginData->GetSamples(filename))
		{
			if (RePlaySE == true) {
				Stop();
				break;
			}
			// キューにバッファを追加
			std::vector< BYTE > & buffer = submitCount & 1 ? m_Secondary : m_Primary;

			std::size_t readSamples = m_pOriginData->ReadRaw(nextFirstSample, m_pOriginData->GetWaveFmtEx().nSamplesPerSec * 3, &(buffer[0]), filename);
			if (readSamples > 0)
			{
				if (isEnd == true || isEmergencyCall == true) {
					break;
				}
				XAUDIO2_BUFFER bufferDesc = { 0 };
				bufferDesc.Flags = nextFirstSample + readSamples >= m_pOriginData->GetSamples(filename) ? XAUDIO2_END_OF_STREAM : 0;
				bufferDesc.AudioBytes = readSamples * m_pOriginData->GetWaveFmtEx().nBlockAlign;
				bufferDesc.pAudioData = &(buffer[0]);
				if (m_pSourceVoice == nullptr) break;
				m_pSourceVoice->SubmitSourceBuffer(&bufferDesc);

				nextFirstSample += readSamples;
				++submitCount;
			}
		}

		//if (GetAsyncKeyState('P') & 0x8000) {
		//	TST += 0.01f;
		//	AdjustPitch(TST);

		//}


		// 過負荷にならないよう調整
		Sleep(1);
	}

	IsPlay = false;

	return true;
}

bool clsXAudio2PlaySound::LoopPlay(const char * filename, bool & isEnd, bool& isEmergencyCall)
{
	static float TST = 0.0f;
	isEnd = false;
	Stop();
	if (m_pSourceVoice == nullptr) return false;
	m_pSourceVoice->Start(0);
	while (1)
	{
		if (isEnd == true || isEmergencyCall == true) {
			break;
		}

		XAUDIO2_VOICE_STATE state;
		if (isEnd != true || isEmergencyCall != true) {
			if (m_pSourceVoice != nullptr) {
				if (isEnd != true || isEmergencyCall != true) {
					m_pSourceVoice->GetState(&state);
				}
			}
			else {
				break;
			}
		}

		if (state.BuffersQueued == 0 && nextFirstSample >= m_pOriginData->GetSamples(filename))
		{
			// すべて再生し終わっている
			break;
		}
		else if (state.BuffersQueued < 2 && nextFirstSample < m_pOriginData->GetSamples(filename))
		{
			if (isEnd == true || isEmergencyCall == true) {
				break;
			}
			// キューにバッファを追加
			std::vector< BYTE > & buffer = submitCount & 1 ? m_Secondary : m_Primary;

			std::size_t readSamples = m_pOriginData->ReadRaw(nextFirstSample, m_pOriginData->GetWaveFmtEx().nSamplesPerSec * 3, &(buffer[0]), filename);
			if (readSamples > 0)
			{
				if (isEnd == true || isEmergencyCall == true) {
					break;
				}
				XAUDIO2_BUFFER bufferDesc = { 0 };
				bufferDesc.Flags = nextFirstSample + readSamples >= m_pOriginData->GetSamples(filename) ? XAUDIO2_END_OF_STREAM : 0;
				bufferDesc.AudioBytes = readSamples * m_pOriginData->GetWaveFmtEx().nBlockAlign;
				bufferDesc.pAudioData = &(buffer[0]);
				if (m_pSourceVoice != nullptr) {
					m_pSourceVoice->SubmitSourceBuffer(&bufferDesc);
				}
				nextFirstSample += readSamples;
				++submitCount;
			}
			// 終わりまで読み切ったら頭から読み直し (ループ再生)
			// 元はここにループのためのソースを入れる.
			if (nextFirstSample >= m_pOriginData->GetSamples(filename)) nextFirstSample = 0;
		}

		if (m_FadeInStart == true) {
			FadeInBGM(0.001f, isEmergencyCall );
			m_FadeInStart = false;
		}

		if(m_FadeOutStart == true){
			FadeOutBGM(-0.001f, isEmergencyCall );
			m_FadeOutStart = false;
		}


		// 過負荷にならないよう調整
		Sleep(1);
		//// 終わりまで読み切ったら頭から読み直し (ループ再生)
		//クッソ短い奴はここじゃないとしない.
		//if (nextFirstSample >= reader.GetSamples()) nextFirstSample = 0;
	}

	return true;
}


//現ループでループ終了.
bool clsXAudio2PlaySound::ExitLoop()
{
	if (m_pSourceVoice)
	{
		m_pSourceVoice->ExitLoop();
	}
	else
	{
		return false;
	}
	return true;
}

// サウンド完全停止.
bool clsXAudio2PlaySound::Stop()
{
	if (m_pSourceVoice)
	{
		m_pSourceVoice->Stop(0);
		m_pSourceVoice->FlushSourceBuffers();
		nextFirstSample = 0;
	}
	return true;
}

//// サウンド最初からながしなおし.
//bool clsXAudio2PlaySound::RePlay()
//{
//	if (m_pSourceVoice)
//	{
//		Stop();
//		Play();
//	}
//	return true;
//}

// サウンド一時停止.
bool clsXAudio2PlaySound::Pause()
{
	if (m_pSourceVoice)
	{
		m_pSourceVoice->Stop(0);
	}
	return true;
}

// Wavファイルからデータを読み込む.
bool clsXAudio2PlaySound::CreateSound(std::shared_ptr<clsXAudio2WaveLoad> pWaveData,const char* filename)
{
	//一回データが作られていたらリターン.
	if (m_pOriginData != nullptr) return true;
	m_pOriginData = pWaveData.get();

	clsXAudio2MasterVoice& xAudioMaster = clsXAudio2MasterVoice::GetGlobalSystem();
	
	IXAudio2*   pHandle = xAudioMaster.GetInterface();

	HRESULT hr = pHandle->CreateSourceVoice(&m_pSourceVoice,
		&m_pOriginData->GetWaveFmtEx(),0,m_maxPitch,nullptr);
	
	if (FAILED(hr))
		return false;

	//! Create Reverb
	//IUnknown * pReverbAPO;
	//XAudio2CreateReverb(&pReverbAPO);


	//XAUDIO2_EFFECT_DESCRIPTOR desc;
	//desc.InitialState = false;
	//desc.pEffect = pReverbAPO;
	//desc.OutputChannels = 1;

	//XAUDIO2_EFFECT_CHAIN chain;
	//chain.EffectCount = 1;
	//chain.pEffectDescriptors = &desc;
	//m_pSourceVoice->SetEffectChain(&chain);
	Submit(pWaveData,filename);

	return true;
}

// 音量取得.
float clsXAudio2PlaySound::GetVolume()const
{
	if (m_pSourceVoice == nullptr) return false;
	float voluem = 0;
	if (m_pSourceVoice)
	{
		m_pSourceVoice->GetVolume(&voluem);
	}

	return voluem;
}

// 音量設定.
bool  clsXAudio2PlaySound::SetVolume(float value)
{
	if (m_pSourceVoice)
	{
		m_pSourceVoice->SetVolume(max(min(value, m_maxVolume), 0));
		return true;
	}

	return false;
}

// 最大音量設定.
void  clsXAudio2PlaySound::SetMaxVolume(float value)
{
	m_maxVolume = value;
}

// 現在の音量から加算または減算.
bool clsXAudio2PlaySound::AdjustVolume(float value)
{
	return SetVolume(GetVolume() + value);
}

// ピッチを設定.
bool clsXAudio2PlaySound::SetPitch(float value)
{
	if (m_pSourceVoice)
	{
		m_pSourceVoice->SetFrequencyRatio(max(min(value, m_maxPitch), XAUDIO2_MIN_FREQ_RATIO));
	}
	else
	{
		return false;
	}
	return true;
}

//ピッチを取得.
float clsXAudio2PlaySound::GetPitch()const
{
	float pitch = XAUDIO2_MIN_FREQ_RATIO;
	if (m_pSourceVoice)
	{
		m_pSourceVoice->GetFrequencyRatio(&pitch);
	}
	return pitch;
}

// 現在のピッチから加算or減算.
bool clsXAudio2PlaySound::AdjustPitch(float value)
{
	return SetPitch(GetPitch() + value);
}

// 最大ピッチ設定.
void  clsXAudio2PlaySound::SetMaxPitch(float value)
{
	m_maxPitch = max(min(value, 2.0f), XAUDIO2_MIN_FREQ_RATIO);
}

// リバーブ(残響)を使うかどうかフラグ設定.
bool clsXAudio2PlaySound::UsingReverb(bool flag)
{
	if (m_pSourceVoice)
	{
		if (flag)
			m_pSourceVoice->EnableEffect(0);
		else
			m_pSourceVoice->DisableEffect(0);
	}
	else
	{
		return false;
	}

	return true;
}

bool clsXAudio2PlaySound::FadeOutBGM(float value, bool& isEmergencyCall )
{
	while (GetVolume() > 0.0f)
	{
		if( isEmergencyCall == true ) return true;

		AdjustVolume(value);
		Sleep(1);
	}

	return true;
}

bool clsXAudio2PlaySound::FadeInBGM(float value, bool& isEmergencyCall )
{
	while (GetVolume() < 1.0f)
	{
		if( isEmergencyCall == true ) return true;

		AdjustVolume(value);
		Sleep(1);
	}

	return true;
}

void clsXAudio2PlaySound::StopSource()
{
	if (m_pSourceVoice != nullptr)
	{
		m_pSourceVoice->FlushSourceBuffers();

		XAUDIO2_VOICE_STATE xState;
		m_pSourceVoice->GetState(&xState);
		if (IsPlaying() == true) {
			Stop();
			xState.BuffersQueued = 0;
			//isEnd = true;
		}
		while (xState.BuffersQueued != 0)
		{
			m_pSourceVoice->GetState(&xState);
			Sleep(5);
		}
	m_pSourceVoice->DestroyVoice();
	m_pSourceVoice = nullptr;
	}
}

void clsXAudio2PlaySound::DestoroySource()
{
	if (m_pSourceVoice != nullptr) {
		m_pSourceVoice->DestroyVoice();
		m_pSourceVoice = nullptr;
	}
}
