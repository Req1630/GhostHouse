#ifndef C_SOUNDMANAGER_H
#define C_SOUNDMANAGER_H

#include "..\SingletonBase\SingletonBase.h"

#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <iostream>
#include "XAudio2Play/XAudio2PlaySound.h"
#include "XAudio2WaveLoad/XAudio2WaveLoad.h"
class CSoundManager : public std::CSingletonBase<CSoundManager>
{
public:
	CSoundManager();
	~CSoundManager();


//========================================================================================
//	BGM
//====
	// BGM�̃��X�g���t�@�C����T�����č��.
	static void GetBGMList();
	// ���X�g�ɖ��O�������Ă���΁A����BGM��index�ԍ��擾�֐�.
	// ���݂��Ă��Ȃ������w�肵���ꍇ�A�G���[�l -1 ���Ԃ��ė���.
	static int GetBGMIndex(std::string Name);
	// BGM�Đ�.
	static void PlayBGM(int index, bool& isEnd, bool LoopFlag);
	// BGM���Đ�����֐���BGM�̖��O����Ăяo���֐�.
	static void PlayBGMByName(std::string Name, bool& isEnd, bool LoopFlag);
	// BGM��~.
	static void StopBGM(int index);
	// BGM���~����֐���BGM�̖��O����Ăяo���֐�.
	static void StopBGMByName(std::string Name);
	// BGM�̉��ʂ����X�ɏ��������Ă������߂�PlaySound.cpp�ɔ�΂��֐�.
	static bool FadeOutBGM(int index);
	// FadeOutBGM�֐����Ăяo���֐�.
	static void FadeOutBGMByName(std::string Name);
	// BGM�̉��ʂ����X�ɑ傫�����Ă������߂�PlaySound.cpp�ɔ�΂��֐�.
	static bool FadeInBGM(int index);
	// FadeInBGM�֐����Ăяo���֐�.
	static void FadeInBGMByName(std::string Name);	
	// BGM�̉��ʂ��擾����֐�.
	static float GetBGMVolme(std::string Name);
	// BGM�̉��ʂ��Z�b�g����֐�.
	static void SetBGMVolme(std::string Name, float Volme);
//========================================================================================
//	SE
//====
	// SE�̃��X�g���t�@�C����T�����č��.
	static void GetSEList();
	// ���X�g�ɖ��O�������Ă���΁A����SE��index�ԍ��擾�֐�.
	// ���݂��Ă��Ȃ������w�肵���ꍇ�A�G���[�l -1 ���Ԃ��ė���.
	static int GetSEIndex(std::string Name);
	// SE�Đ��֐�.
	static void PlaySEByName(std::string& Name, bool & isEnd, bool & ChangeSE, bool LoopFlag,bool& IsPlay,bool& RePlay);
	// SE��~�֐�.
	static void StopSE(int index);
	// StopSE�֐����Ăяo���֐�.
	static void StopSEByName(std::string Name);
	// BGM��SE�̃��X�g��SoundSource���쐬����֐�.
	static void InitALL();
	// BGM��SE��SoundSource���쐬����֐�.
	static void InitSource();
	// BGM��SoundSource���쐬����֐�.
	static void CreateBGMSource();
	// SE��SoundSource���쐬����֐�.
	static void CreateSESource();

	static void CleanBGM(std::string Name);
	// SE��SoundSource���N���[��.
	static void Clean();

	// �㏈���p�AXAudio2�֌W�J���֐�.
	static void Release();
	// �A�v�����}�ɋ����I�����������ɁABGM���~�����邽�߂̃t���O���Z�b�g����֐�.
	// ����Ńt���O���Z�b�g���Ē�~�����Ȃ��ƁAXAudio2��BGM���Đ����ɊJ�����悤�Ƃ��āA�G���[���o��.
	static void SetEmergencyStopFlag(bool StopFlag) { GetInstance()->m_bEmergencyStop = StopFlag; }

private:
	// ��񂾂��֐����ĂԂ��߂̃t���O
	bool m_bFristCall;
	// wav�t�@�C���̃f�[�^������N���X�̕ϐ�.
	std::vector<std::shared_ptr<clsXAudio2WaveLoad>> pWaveData;
///////////////////////////////////////////////////////////////////////////
//	BGM�֌W.
	// BGM�̃C���f�b�N�X���X�g.
	std::map<std::string, int>  m_sBGMIndex;
	// BGM��SoundSource������.
	std::vector<std::shared_ptr<clsXAudio2PlaySound>> pBgmSource;
	// BGM�̃��X�g.
	std::vector<std::string>	m_vsBGMNameList;

///////////////////////////////////////////////////////////////////////////
//	SE�֌W.

	// SE�̃C���f�b�N�X���X�g.
	std::map<std::string, int>  m_sSEIndex;
	// SE�̃��X�g.
	std::vector<std::string>	m_vsSENameList;
	// SE��SoundSource������.
	std::vector<std::shared_ptr<clsXAudio2PlaySound>> pSeSource;
	// SE�̃��X�g�����ہABGM�ō����WavData�̒l����
	// for���[�v���񂷂��߂̕ϐ�.
	size_t CountWavDataSize;
	// �Q�[�����I������ۂɗ����ATrue����XAudio2�֌W�̊֐������^�[������悤�ɂȂ�ϐ�.
	bool	m_bEndGame;
	// �����I������BGM���~�߂�ϐ�.
	bool	m_bEmergencyStop;
};



#endif // #ifndef C_SOUNDMANAGER_H
