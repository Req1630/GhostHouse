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
	m_maxPitch(1.4f),	//5.0f�}�b�N�X���Ǝv���܂�.
	nextFirstSample(0),
	submitCount(0),
	m_FadeInStart(false),
	m_FadeOutStart(false)
{
	SetVolume(1.0f);
}

clsXAudio2PlaySound::~clsXAudio2PlaySound()
{
}

// �����f�[�^���o�b�t�@�ɓ����.
bool clsXAudio2PlaySound::Submit(std::shared_ptr<clsXAudio2WaveLoad> pWaveData, const char* filename)
{
	nextFirstSample = 0;
	submitCount = 0;

	// �v���C�}���o�b�t�@
	m_Primary = std::vector< BYTE >(pWaveData->GetWaveFmtEx().nAvgBytesPerSec * 3);
	if (nextFirstSample < m_pOriginData->GetSamples(filename))
	{
		std::size_t readSamples = m_pOriginData->ReadRaw(nextFirstSample, pWaveData->GetWaveFmtEx().nSamplesPerSec * 3, &(m_Primary[0]), filename);
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

	// �Z�J���_���o�b�t�@
	m_Secondary = std::vector< BYTE >(pWaveData->GetWaveFmtEx().nAvgBytesPerSec * 3);

	return true;
}
// �Đ������ǂ������擾����֐�.
bool clsXAudio2PlaySound::IsPlaying()const
{
	// SoundSource���Ȃ��ꍇ���^�[�� 0(�Đ����Ă��Ȃ��Ƃ��Ɠ���).
	if (m_pSourceVoice == nullptr) return 0;
	XAUDIO2_VOICE_STATE xState;
	// SoundSource�̃X�e�[�g�擾.
	m_pSourceVoice->GetState(&xState);

	return xState.BuffersQueued != 0;
}


// �Đ�.
bool clsXAudio2PlaySound::Play(const char* filename, bool& isEnd, bool& isEmergencyCall)
{
	// SoundSource���Đ�.
	m_pSourceVoice->Start(0);
	while (1)
	{
		// �I���t���O�A�ً}��~�t���O�������Ă����while���[�v�𔲂���.
		if (isEnd == true || isEmergencyCall == true) break;

		XAUDIO2_VOICE_STATE state;
		if (m_pSourceVoice == nullptr) break;
		m_pSourceVoice->GetState(&state);
		// ���ׂčĐ����I����Ă����璆�ɓ���.
		if (state.BuffersQueued == 0 && nextFirstSample >= m_pOriginData->GetSamples(filename))
		{
			break;
		}
		else if (state.BuffersQueued < 2 && nextFirstSample < m_pOriginData->GetSamples(filename))
		{
			// �I���t���O�A�ً}��~�t���O�������Ă����while���[�v�𔲂���.
			if (isEnd == true || isEmergencyCall == true) break;
			// �L���[�Ƀo�b�t�@��ǉ�
			std::vector< BYTE > & buffer = submitCount & 1 ? m_Secondary : m_Primary;
			// �T���v���[��ǂݍ���.
			std::size_t readSamples = m_pOriginData->ReadRaw(nextFirstSample, m_pOriginData->GetWaveFmtEx().nSamplesPerSec * 3, &(buffer[0]), filename);
			if (readSamples > 0)
			{
				if (isEnd == true || isEmergencyCall == true) break;
				XAUDIO2_BUFFER bufferDesc = { 0 };
				bufferDesc.Flags = nextFirstSample + readSamples >= m_pOriginData->GetSamples(filename) ? XAUDIO2_END_OF_STREAM : 0;
				bufferDesc.AudioBytes = readSamples * m_pOriginData->GetWaveFmtEx().nBlockAlign;
				bufferDesc.pAudioData = &(buffer[0]);

				m_pSourceVoice->SubmitSourceBuffer(&bufferDesc);

				nextFirstSample += readSamples;
				++submitCount;
			}
		}

		// �ߕ��ׂɂȂ�Ȃ��悤����
		Sleep(1);
	}
	return true;
}

bool clsXAudio2PlaySound::SEPlay(
	const char * filename, bool & isEnd, bool& Reset, bool& IsPlay, bool& RePlaySE, bool& isEmergencyCall)
{
	IsPlay = true;

	if (Reset == true) {
		XAUDIO2_VOICE_STATE state;
		m_pSourceVoice->GetState(&state);
		m_pSourceVoice->FlushSourceBuffers();
		nextFirstSample = 0;
		Reset = false;
		RePlaySE = false;
	}
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
			// ���ׂčĐ����I����Ă���
			break;
		}
		else if (state.BuffersQueued < 2 && nextFirstSample < m_pOriginData->GetSamples(filename))
		{
			if (RePlaySE == true) {
				Stop();
				break;
			}
			// �L���[�Ƀo�b�t�@��ǉ�
			std::vector< BYTE > & buffer = submitCount & 1 ? m_Secondary : m_Primary;

			std::size_t readSamples = m_pOriginData->ReadRaw(nextFirstSample, m_pOriginData->GetWaveFmtEx().nSamplesPerSec * 3, &(buffer[0]), filename);
			if (readSamples > 0)
			{
				if (isEnd == true || isEmergencyCall == true) break;
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

		// �ߕ��ׂɂȂ�Ȃ��悤����
		Sleep(1);
	}

	IsPlay = false;

	return true;
}

bool clsXAudio2PlaySound::LoopPlay(const char * filename, bool & isEnd, bool& isEmergencyCall)
{
	isEnd = false;
	Stop();
	if (m_pSourceVoice == nullptr) return false;
	m_pSourceVoice->Start(0);
	while (1)
	{
		if (isEnd == true || isEmergencyCall == true) break;

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
			// ���ׂčĐ����I����Ă���
			break;
		}
		else if (state.BuffersQueued < 2 && nextFirstSample < m_pOriginData->GetSamples(filename))
		{
			if (isEnd == true || isEmergencyCall == true) break;
			// �L���[�Ƀo�b�t�@��ǉ�
			std::vector< BYTE > & buffer = submitCount & 1 ? m_Secondary : m_Primary;

			std::size_t readSamples = m_pOriginData->ReadRaw(nextFirstSample, m_pOriginData->GetWaveFmtEx().nSamplesPerSec * 3, &(buffer[0]), filename);
			if (readSamples > 0)
			{
				if (isEnd == true || isEmergencyCall == true) break;

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
			// �I���܂œǂݐ؂����瓪����ǂݒ��� (���[�v�Đ�)
			if (nextFirstSample >= m_pOriginData->GetSamples(filename)) nextFirstSample = 0;
		}
		// �t�F�[�h�C���t���O�������Ă���΁A�t�F�[�h�C��.
		if (m_FadeInStart == true) {
			FadeInBGM(FADE_VOLUME, isEmergencyCall);
			m_FadeInStart = false;
		}
		// �t�F�[�h�A�E�g�t���O�������Ă���΁A�t�F�[�h�C��.
		if (m_FadeOutStart == true) {
			FadeOutBGM(-FADE_VOLUME, isEmergencyCall);
			m_FadeOutStart = false;
		}


		// �ߕ��ׂɂȂ�Ȃ��悤����
		Sleep(1);
	}

	return true;
}


//�����[�v�Ń��[�v�I��.
bool clsXAudio2PlaySound::ExitLoop()
{
	if (m_pSourceVoice == nullptr) return false;

	m_pSourceVoice->ExitLoop();

	return true;
}

// �T�E���h���S��~.
bool clsXAudio2PlaySound::Stop()
{
	if (m_pSourceVoice == nullptr) return true;

	m_pSourceVoice->Stop(0);
	m_pSourceVoice->FlushSourceBuffers();
	nextFirstSample = 0;
	
	return true;
}

// �T�E���h�ꎞ��~.
bool clsXAudio2PlaySound::Pause()
{
	if (m_pSourceVoice == nullptr) return true;

	m_pSourceVoice->Stop(0);

	return true;
}

// Wav�t�@�C������f�[�^��ǂݍ���.
bool clsXAudio2PlaySound::CreateSound(std::shared_ptr<clsXAudio2WaveLoad> pWaveData, const char* filename)
{
	//���f�[�^������Ă����烊�^�[��.
	if (m_pOriginData != nullptr) return true;
	m_pOriginData = pWaveData.get();

	clsXAudio2MasterVoice& xAudioMaster = clsXAudio2MasterVoice::GetGlobalSystem();

	IXAudio2*   pHandle = xAudioMaster.GetInterface();

	HRESULT hr = pHandle->CreateSourceVoice(&m_pSourceVoice,
		&m_pOriginData->GetWaveFmtEx(), 0, m_maxPitch, nullptr);

	if (FAILED(hr))
		return false;

	Submit(pWaveData, filename);

	return true;
}

// ���ʎ擾.
float clsXAudio2PlaySound::GetVolume()const
{
	if (m_pSourceVoice == nullptr) return false;
	float voluem = 0;

	m_pSourceVoice->GetVolume(&voluem);

	return voluem;
}

// ���ʐݒ�.
bool  clsXAudio2PlaySound::SetVolume(float value)
{
	if (m_pSourceVoice == nullptr) return false;

	m_pSourceVoice->SetVolume(max(min(value, m_maxVolume), 0));
	return true;
}

// �ő剹�ʐݒ�.
void  clsXAudio2PlaySound::SetMaxVolume(float value)
{
	m_maxVolume = value;
}

// ���݂̉��ʂ�����Z�܂��͌��Z.
bool clsXAudio2PlaySound::AdjustVolume(float value)
{
	if (m_pSourceVoice == nullptr) return false;

	return SetVolume(GetVolume() + value);
}

// �s�b�`��ݒ�.
bool clsXAudio2PlaySound::SetPitch(float value)
{
	if (m_pSourceVoice == nullptr) return false;

	m_pSourceVoice->SetFrequencyRatio(max(min(value, m_maxPitch), XAUDIO2_MIN_FREQ_RATIO));

	return true;
}

//�s�b�`���擾.
float clsXAudio2PlaySound::GetPitch()const
{
	float pitch = XAUDIO2_MIN_FREQ_RATIO;
	if (m_pSourceVoice == nullptr) return pitch;

	m_pSourceVoice->GetFrequencyRatio(&pitch);

	return pitch;
}

// ���݂̃s�b�`������Zor���Z.
bool clsXAudio2PlaySound::AdjustPitch(float value)
{
	return SetPitch(GetPitch() + value);
}

// �ő�s�b�`�ݒ�.
void  clsXAudio2PlaySound::SetMaxPitch(float value)
{
	m_maxPitch = max(min(value, 2.0f), XAUDIO2_MIN_FREQ_RATIO);
}

bool clsXAudio2PlaySound::FadeOutBGM(float value, bool& isEmergencyCall)
{
	while (GetVolume() > 0.0f)
	{
		if (isEmergencyCall == true) return true;

		AdjustVolume(value);
		Sleep(1);
	}

	return true;
}

bool clsXAudio2PlaySound::FadeInBGM(float value, bool& isEmergencyCall)
{
	while (GetVolume() < 1.0f)
	{
		if (isEmergencyCall == true) return true;

		AdjustVolume(value);
		Sleep(1);
	}

	return true;
}

void clsXAudio2PlaySound::StopSource()
{
	if (m_pSourceVoice == nullptr) return;

	m_pSourceVoice->FlushSourceBuffers();

	XAUDIO2_VOICE_STATE xState;
	m_pSourceVoice->GetState(&xState);
	if (IsPlaying() == true) {
		Stop();
		xState.BuffersQueued = 0;
	}
	while (xState.BuffersQueued != 0)
	{
		m_pSourceVoice->GetState(&xState);
		Sleep(5);
	}
	m_pSourceVoice->DestroyVoice();
	m_pSourceVoice = nullptr;
}

void clsXAudio2PlaySound::DestoroySource()
{
	if (m_pSourceVoice == nullptr) return;

	m_pSourceVoice->DestroyVoice();
	m_pSourceVoice = nullptr;
}
