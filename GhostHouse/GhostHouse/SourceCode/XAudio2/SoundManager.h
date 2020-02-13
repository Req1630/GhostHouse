#ifndef C_SOUNDMANAGER_H
#define C_SOUNDMANAGER_H

#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <iostream>
#include "XAudio2Play/XAudio2PlaySound.h"
#include "XAudio2WaveLoad/XAudio2WaveLoad.h"
class CSoundManager
{
private:
	// Sound�f�[�^�������Ă���f�B���N�g���p�X.
	const char* FILE_PATH = "Data\\Sound";
public:
	CSoundManager();
	~CSoundManager();

	// �C���X�^���X�̎擾.
	static CSoundManager* GetInstance();

	// �T�E���h�f�[�^�쐬.
	static void CreateSoundData();
//========================================================================================
//	BGM
//====
	// BGM�Đ��֐�.
	static void PlayBGM(std::string Name, bool& isEnd, bool LoopFlag);
	// BGM��~�֐�.
	static void StopBGM(std::string Name);
	// BGM�t�F�[�h�A�E�g�֐�.
	static bool FadeOutBGM(std::string Name);
	// BGM�t�F�[�h�C���֐�.
	static bool FadeInBGM(std::string Name);
	// BGM�̉��ʎ擾�֐�.
	static float GetBGMVolme(std::string Name);
	// BGM�̉��ʃZ�b�g�֐�.
	static void SetBGMVolme(std::string Name, float Volme);
//========================================================================================
//	SE
//====
	// SE�Đ��֐�.
	static void PlaySE(std::string& Name, bool & isEnd, bool & ChangeSE,bool& IsPlay,bool& RePlay);
	// SE��~�֐�.
	static void StopSE(std::string Name);
	// ��������֐�.
	static void Release();
	// �ُ��~�t���O�Z�b�g�֐�.
	static void SetEmergencyStopFlag(bool StopFlag) { GetInstance()->m_bEmergencyStop = StopFlag; }

private:
	// Wav�f�[�^�N���X�̔�.
	std::map <std::string, std::shared_ptr<clsXAudio2WaveLoad>> pWaveData;
	// BGM�pPlaySound�N���X�̔�.
	std::map <std::string, std::shared_ptr<clsXAudio2PlaySound>> pBgmSource;

	// BGM�̖��O���X�g.
	// ������ɕK�v.
	std::vector<std::string>	m_vsBGMNameList;

	// SE�̖��O���X�g.
	// ������ɕK�v.
	std::vector<std::string>	m_vsSENameList;
	// SE�pPlaySound�N���X�̔�.
	std::map <std::string, std::shared_ptr<clsXAudio2PlaySound>> pSeSource;
	// �T�E���h�f�[�^�擾����񂾂��ʂ邽�߃t���O.
	bool m_bFristCall;
	// �I���p�t���O.
	bool	m_bEndGame;
	// �ُ�I���p�t���O.
	bool	m_bEmergencyStop;
private:
	// �R�s�[�E���[�u�R���X�g���N�^, ������Z�q�̍폜.
	CSoundManager(const CSoundManager &) = delete;
	CSoundManager& operator = (const CSoundManager &) = delete;
	CSoundManager(CSoundManager &&) = delete;
	CSoundManager& operator = (CSoundManager &&) = delete;
};



#endif // #ifndef C_SOUNDMANAGER_H
