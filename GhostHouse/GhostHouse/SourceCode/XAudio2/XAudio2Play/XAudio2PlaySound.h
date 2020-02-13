#ifndef C_XAUDIO2_PLAYSOUND_H
#define C_XAUDIO2_PLAYSOUND_H


#include <vector>

#include "..\XAudio2.h"
#include "..\XAudio2WaveLoad\XAudio2WaveLoad.h"
#include "..\XAudio2Master\XAudio2MasterVoice.h"

struct IXAudio2SourceVoice;

class clsXAudio2PlaySound
{
private:
	const float FADE_VOLUME = 0.001f;
public:
	// �R���X�g���N�^.
	clsXAudio2PlaySound();

	// �f�X�g���N�^.
	virtual ~clsXAudio2PlaySound();

	// ���[�v�����邩�ǂ����擾�֐�.
	bool IsLoop()const { return m_isLoop; }

	// �Đ����Ă邩�ǂ����擾�֐�.
	bool IsPlaying()const;

	// �T�E���h�Đ��A�O�Ɉꎞ��~����Ă����ꍇ�͋L���ʒu����ĊJ.
	virtual bool Play(const char* filename, bool& isEnd, bool& isEmergencyCall);

	// �T�E���h�Đ��A�O�Ɉꎞ��~����Ă����ꍇ�͋L���ʒu����ĊJ.
	virtual bool SEPlay(
		const char* filename, bool& isEnd, bool& Reset, bool& IsPlay, bool& RePlaySE, bool& isEmergencyCall);

	// �T�E���h�Đ��A�O�Ɉꎞ��~����Ă����ꍇ�͋L���ʒu����ĊJ.
	virtual bool LoopPlay(const char* filename, bool& isEnd, bool& isEmergencyCall);

	// �T�E���h�����S��~.(�ʒu��ێ����Ȃ�).
	bool Stop();

	// �T�E���h�ꎞ��~.
	bool Pause();

	// ���݂̃��[�v�ŃT�E���h���[�v���I��.
	bool ExitLoop();

	// �s�b�`�ݒ�֐�.
	bool SetPitch(float value);

	// �s�b�`���擾�֐�.
	float GetPitch()const;

	// ���݂̃s�b�`������Z�܂��͌��Z.
	bool AdjustPitch(float value);

	// �ő剹�ʂ�ݒ肵�܂�.
	void  SetMaxPitch(float value);

	// wav�t�@�C�����特���f�[�^��ǂݍ���.
	bool CreateSound(std::shared_ptr< clsXAudio2WaveLoad> pWavData, const char* filename);

	// ���ʂ��擾���܂�.
	float GetVolume()const;

	// ���ʂ�ݒ肵�܂�.
	bool  SetVolume(float value);

	// �ő剹�ʂ�ݒ肵�܂�.
	void  SetMaxVolume(float value);

	// ���݂̉��ʂ�����Z�܂��͌��Z.
	bool  AdjustVolume(float value);

	// Reverb(�����G�t�F�N�g)�̗L���t���O��ݒ肵�܂�.
	bool UsingReverb(bool flag);
	// BGM���t�F�[�h�A�E�g����֐�.
	bool FadeOutBGM(float value, bool& isEmergencyCall);
	// BGM���t�F�[�h�C������֐�.
	bool FadeInBGM(float value, bool& isEmergencyCall);
	// �Đ����Ă���SoundSource���~����֐�.
	void StopSource();
	// SoundSource��j�󂷂�֐�.
	void DestoroySource();
	// �t�F�[�h�A�E�g�t���O�̃Z�b�^�[.
	void SetFadeOutFlag(bool BGMFadeOutFlag) { m_FadeOutStart = BGMFadeOutFlag; }
	// �t�F�[�h�A�E�g�t���O�̃Q�b�^�[.
	bool GetFadeOutFlag() { return m_FadeOutStart; }
	// �t�F�[�h�C���t���O�̃Z�b�^�[.
	void SetFadeInFlag(bool BGMFadeInFlag) { m_FadeInStart = BGMFadeInFlag; }
	// �t�F�[�h�C���t���O�̃Q�b�^�[.
	bool GetFadeInFlag() { return m_FadeInStart; }

protected:
	//! �����f�[�^���X�g���[���ɗ������݂܂�.
	//  ���̊֐��͊O������Ăяo���܂���.
	bool Submit(std::shared_ptr<clsXAudio2WaveLoad> pWaveData, const char* filename);
protected:

	//! �ϐ�
	clsXAudio2WaveLoad*       m_pOriginData;  //  �T�E���h�����f�[�^.
	IXAudio2SourceVoice*   m_pSourceVoice; //  �T�E���h���Đ�����X�g���[��.

	size_t nextFirstSample;
	size_t submitCount;
	bool                    m_isLoop;       //  ���[�v�t���O.
	float                   m_maxVolume;    //  �ő剹��.
	float                   m_maxPitch;     //  �ő�s�b�`.
	std::vector< BYTE >		m_Primary;		// �v���C�}���o�b�t�@.
	std::vector< BYTE >		m_Secondary;	// �Z�J���h�o�b�t�@.

	bool					m_FadeOutStart;	// �t�F�[�h�A�E�g�J�n�t���O.
	bool					m_FadeInStart;	// �t�F�[�h�C���J�n�t���O.

};

#endif // !C_XAUDIO2_PLAYSOUND_H