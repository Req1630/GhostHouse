#include "SoundManager.h"
#include <crtdbg.h>

CSoundManager::CSoundManager()
	: CountWavDataSize(0)
	, m_bFristCall ( true )
	, m_bEndGame	( false )
	, m_bEmergencyStop	( false )
{
	// �K�v����Ȃ�����.
//	m_vsBGMNameList.max_size();
//	m_vsSENameList.max_size();
}

CSoundManager::~CSoundManager()
{
	//Release();
}



//========================================================================================
//	BGM.
//====


void CSoundManager::GetBGMList()
{
	WIN32_FIND_DATA FindFileData;					//�t�@�C�������p.
	HANDLE hFind;									//�n���h��.
	std::string filePath = "Data\\Sound\\BGM\\*";	//BGM�������Ă�t�@�C���K�w�p�X.
	std::vector<std::string> BGMNameList;			//BGM���X�g�p.

	hFind = FindFirstFile(filePath.c_str(), &FindFileData);
	if (hFind != FreeLibrary) {
		do {
			BGMNameList.emplace_back(FindFileData.cFileName);
		} while (FindNextFile(hFind, &FindFileData));
		// m_vsStageList : [0]"." [1]".." �̍폜.
		BGMNameList.erase(BGMNameList.begin(), BGMNameList.begin() + 2);
		FindClose(hFind);
	}
	//SoundManager�̃����o�[�ϐ��ɓn��.
	GetInstance()->m_vsBGMNameList = BGMNameList;
	//BGM���X�g�ɓ������T�C�Y�����[�v����WavData��Get���č��.
	for (size_t i = GetInstance()->CountWavDataSize; i < GetInstance()->m_vsBGMNameList.size(); i++) {
		GetInstance()->pWaveData.emplace_back(std::make_shared<clsXAudio2WaveLoad>());
		GetInstance()->pWaveData[i]->GetWaveFormat(BGMNameList[i].c_str());
		GetInstance()->m_sBGMIndex[BGMNameList[i]] = i;
		GetInstance()->CountWavDataSize++;
	}
	BGMNameList.clear();
}

int CSoundManager::GetBGMIndex(std::string Name)
{
	size_t iErrer = 1;	// ���X�g�ɂȂ��ꍇ�̃G���[�p�̐���.
	if (iErrer > GetInstance()->m_sBGMIndex.count(Name)) {
		return -1;
	}
	return GetInstance()->m_sBGMIndex[Name];
}

void CSoundManager::PlayBGM(int index, bool& isEnd, bool LoopFlag)
{
	//GetBGMIndex()��-1(�G���[�l)��Ԃ��Ă��Ă��烊�^�[��.
	if (index == -1) {
		return;
	}
	//index = GetInstance()->Clamp(index, 0, GetInstance()->m_vsBGMNameList.size());
	


	//���[�v�t���O�ݒ�.
	//GetInstance()->pBgmSource[index]->SetLoopFlag(true);
	//�Đ�.
	if (GetInstance()->pBgmSource[index] == nullptr) {
		CreateBGMSource();
	}

	if (LoopFlag == true) {
		GetInstance()->pBgmSource[index]->LoopPlay(GetInstance()->pWaveData[index]->GetFileName(), isEnd, GetInstance()->m_bEmergencyStop);
	}
	else{
		GetInstance()->pBgmSource[index]->Play(GetInstance()->pWaveData[index]->GetFileName(),isEnd, GetInstance()->m_bEmergencyStop);
	}
}

void CSoundManager::PlayBGMByName(std::string Name, bool& isEnd, bool LoopFlag)
{
	if (GetInstance()->m_bEndGame == false) {
		//�w�肵��BGM�̖��O�̌���.wav�ǉ�.
		std::string Cat = Name + ".wav";

		PlayBGM(GetBGMIndex(Cat), isEnd, LoopFlag);
	}
}

void CSoundManager::StopBGM(int index)
{
	if (index == -1) {
		return;
	}

	GetInstance()->pBgmSource[index]->Stop();
}

void CSoundManager::StopBGMByName(std::string Name)
{
	//�w�肵��BGM�̖��O�̌���.wav�ǉ�.
	std::string Cat = Name + ".wav";

	StopBGM(GetBGMIndex(Cat));
}

bool CSoundManager::FadeOutBGM(int index)
{
	if (index == -1) {
		return false;
	}
	if (GetInstance()->pBgmSource[index]->GetVolume() > 0) {
		GetInstance()->pBgmSource[index]->SetFadeOutFlag(true);
	}
	else if (GetInstance()->pBgmSource[index]->GetVolume() == 0) {
		return true;
	}
	return true;
}

void CSoundManager::FadeOutBGMByName(std::string Name)
{
	//�w�肵��BGM�̖��O�̌���.wav�ǉ�.
	std::string Cat = Name + ".wav";

	FadeOutBGM(GetBGMIndex(Cat));
}

bool CSoundManager::FadeInBGM(int index)
{
	if (index == -1) {
		return false;
	}
	if (GetInstance()->pBgmSource[index]->GetVolume() < 1.0f) {
		GetInstance()->pBgmSource[index]->SetFadeInFlag(true);
	}
	else if (GetInstance()->pBgmSource[index]->GetVolume() == 1.0f) {
		return true;
	}
	return true;
}

void CSoundManager::FadeInBGMByName(std::string Name)
{
	//�w�肵��BGM�̖��O�̌���.wav�ǉ�.
	std::string Cat = Name + ".wav";

	FadeInBGM(GetBGMIndex(Cat));
}

void CSoundManager::PauseBGM(int index)
{
	if (index == -1) {
		return;
	}
	GetInstance()->pBgmSource[index]->Pause();
}

void CSoundManager::PauseBGMByName(std::string Name)
{
	//�w�肵��BGM�̖��O�̌���.wav�ǉ�.
	std::string Cat = Name + ".wav";

	PauseBGM(GetBGMIndex(Cat));
}

float CSoundManager::GetBGMVolme(std::string Name)
{
	if (GetInstance()->m_bEndGame == false) {
		std::string Cat = Name + ".wav";

		size_t index = GetBGMIndex(Cat);

		//GetBGMIndex()��-1(�G���[�l)��Ԃ��Ă��Ă��烊�^�[��.
		if (index == -1) {
			return -1;
		}
		if (GetInstance()->pBgmSource[index] == nullptr) {
			return -1;
		}

		return GetInstance()->pBgmSource[index]->GetVolume();
	}
	return -1;
}

void CSoundManager::SetBGMVolme(std::string Name, float Volme)
{
	if (GetInstance()->m_bEndGame == false) {
		std::string Cat = Name + ".wav";

		size_t index = GetBGMIndex(Cat);

		//GetBGMIndex()��-1(�G���[�l)��Ԃ��Ă��Ă��烊�^�[��.
		if (index == -1) {
			return;
		}

		GetInstance()->pBgmSource[index]->SetVolume(Volme);
	}
}

//========================================================================================
//	SE.
//====

void CSoundManager::GetSEList()
{
	WIN32_FIND_DATA FindFileData;					//�t�@�C�������p.
	HANDLE hFind;									//�n���h��.
	std::string filePath = "Data\\Sound\\SE\\*";	//SE�������Ă�t�@�C���K�w�p�X.
	std::vector<std::string> SENameList;			//SE���X�g�p.

	hFind = FindFirstFile(filePath.c_str(), &FindFileData);
	if (hFind != FreeLibrary) {
		do {
			SENameList.emplace_back(FindFileData.cFileName);
		} while (FindNextFile(hFind, &FindFileData));
		// m_vsStageList : [0]"." [1]".." �̍폜.
		SENameList.erase(SENameList.begin(), SENameList.begin() + 2);
		FindClose(hFind);
	}
	GetInstance()->m_vsSENameList = SENameList;

	size_t SEListSize = SENameList.size();			//SoundManager�̃����o�ϐ��ɓn��.
	size_t GetNum;									//SEName���X�g�̔z��ԍ��v�Z��.
	size_t NegativSize = SEListSize;				//SEName���X�g�̔z��ԍ����Z�l.
	//BGM�̃��X�g����SE�̃��X�g���𑫂�.
	size_t BGMandSESize = GetInstance()->m_vsBGMNameList.size() + GetInstance()->m_vsSENameList.size();

	for (size_t i = GetInstance()->CountWavDataSize; i < BGMandSESize; i++) {
		GetNum = SEListSize - NegativSize;	//�z��ԍ����v�Z(SE���X�g�T�C�Y��2�̏ꍇ�A2-2... 2-1... 2-0..).
		GetInstance()->pWaveData.emplace_back(std::make_shared<clsXAudio2WaveLoad>());
		size_t tmp = GetInstance()->pWaveData.size();

		GetInstance()->pWaveData[i]->GetWaveFormat(SENameList[GetNum].c_str());
		GetInstance()->m_sSEIndex[SENameList[GetNum]] = GetNum;
		GetInstance()->CountWavDataSize++;
		NegativSize--;
	}
	SENameList.clear();
}

int CSoundManager::GetSEIndex(std::string Name)
{
	size_t iErrer = 1;	// ���X�g�ɂȂ��ꍇ�̃G���[�p�̐���.
	if (iErrer > GetInstance()->m_sSEIndex.count(Name)) {
		return -1;
	}
	return GetInstance()->m_sSEIndex[Name];
}

void CSoundManager::PlaySEByName(
	std::string& Name, bool & isEnd, bool & ChangeSE, bool LoopFlag,bool& IsPlay, bool& RePlay)
{
	bool Reset = true;			//�T�E���h(SE)���͂��߂���Đ�����.
	bool IsError = false;		//�G���[�΍�.
	bool ReturnTop = false;		//���ōĐ��I������while�ҋ@���Ă�����s�ʒu�����̃u���b�N�̏�ɖ߂��t���O.

	while (isEnd == false)
	{
		Reset = true;			
		IsError = false;		
		ReturnTop = false;		
		//�w�肵��BGM�̖��O�̌���.wav�ǉ�.
		std::string Cat = Name + ".wav";

		size_t index = GetSEIndex(Cat);

		//GetBGMIndex()��-1(�G���[�l)��Ԃ��Ă��Ă��烊�^�[��.
		if (index == -1) {
			IsError = true;
		}

		//index�ŏo�����l����BGM�Ɣ��̂ŁA�ǂݍ���WavData����SE�̓ǂ݂��񂾐�������.
		// ���̌�index�𑫂�.
		if(IsError == false)
		{
			size_t Number = GetInstance()->pWaveData.size() - GetInstance()->m_vsSENameList.size();
			Number += index;
			//���[�v�t���O�ݒ�.
			//GetInstance()->pBgmSource[index]->SetLoopFlag(true);
			//�Đ�.
			if (LoopFlag == true) {
				GetInstance()->pSeSource[index]->LoopPlay(GetInstance()->pWaveData[Number]->GetFileName(), isEnd, GetInstance()->m_bEmergencyStop);
			}
			else {

				GetInstance()->pSeSource[index]->SEPlay(
					GetInstance()->pWaveData[Number]->GetFileName(), isEnd, Reset, IsPlay, RePlay, GetInstance()->m_bEmergencyStop);
			}
			IsPlay = false;
		}
		//�Đ��I����A�V����SE�������ė���܂őҋ@.
		while ( ReturnTop == false)
		{
			//�V����SE�Đ����� or �I��������߂����ŗ����璆��.
			if (ChangeSE == true) {
				ChangeSE = false;
				ReturnTop = true;
			}
			if (isEnd == true || GetInstance()->m_bEmergencyStop == true) {
				break;
			}
		}
		//���while�𔲂��Ă���ꍇ�����ɐ�Γ���.
		if (ReturnTop == true) {
			//������߂��o�Ă����ꍇbreak���āA�X���b�h���������.
			if (isEnd == true || GetInstance()->m_bEmergencyStop == true) {
				break;
			}
			continue;
		}

	}

}

void CSoundManager::StopSE(int index)
{
	if (index == -1) {
		return;
	}

	GetInstance()->pSeSource[index]->Stop();
}

void CSoundManager::StopSEByName(std::string Name)
{
	//�w�肵��BGM�̖��O�̌���.wav�ǉ�.
	std::string Cat = Name + ".wav";

	StopSE(GetSEIndex(Cat));
}

void CSoundManager::PauseSE(int index)
{
	if (index == -1) {
		return;
	}
	GetInstance()->pSeSource[index]->Pause();
}

void CSoundManager::PauseSEByName(std::string Name)
{
	//�w�肵��BGM�̖��O�̌���.wav�ǉ�.
	std::string Cat = Name + ".wav";

	PauseSE(GetSEIndex(Cat));

}

void CSoundManager::InitALL()
{
	if (GetInstance()->m_bFristCall == true) {
		GetBGMList();
		GetSEList();

		CreateBGMSource();
		//==========================SE===============================
		CreateSESource();

		GetInstance()->m_bFristCall = false;
	}
}

void CSoundManager::InitSource()
{
	CreateBGMSource();
	CreateSESource();
}

void CSoundManager::CreateBGMSource()
{
	//BGM�̗v�f�̐���make_shared.
	// ������(pBgmSource������Ă��Ȃ�)�ꍇ�̂ݒʂ�.
	if (GetInstance()->pBgmSource.size() != 0) 
	{
		GetInstance()->pBgmSource.clear();
	}
	for (size_t i = 0; i < GetInstance()->m_vsBGMNameList.size(); i++) {
		GetInstance()->pBgmSource.emplace_back(std::make_shared<clsXAudio2PlaySound>());
	}
	//�G���[�`�F�b�N.
	size_t GetBGMSize = GetInstance()->pBgmSource.size();
	if (GetBGMSize != GetInstance()->m_vsBGMNameList.size()) {
		return;
	}
	for (size_t i = 0; i < GetBGMSize; i++) {
		//wav����ǂݍ��񂾃f�[�^�ŃT�E���h�f�[�^�쐬.
		GetInstance()->pBgmSource[i]->CreateSound(GetInstance()->pWaveData[i], GetInstance()->pWaveData[i]->GetFileName());
	}
}

void CSoundManager::CreateSESource()
{
	//BGM�̗v�f�̐���make_shared.
// ������(pBgmSource������Ă��Ȃ�)�ꍇ�̂ݒʂ�.
	if (GetInstance()->pSeSource.size() != 0)
	{
		GetInstance()->pSeSource.clear();
	}

	for (size_t i = 0; i < GetInstance()->m_vsSENameList.size(); i++) {
		GetInstance()->pSeSource.emplace_back(std::make_shared<clsXAudio2PlaySound>());
	}

	//�G���[�`�F�b�N.
	size_t GetSeSize = GetInstance()->pSeSource.size();
	if (GetSeSize != GetInstance()->m_vsSENameList.size()) {
		return;
	}
	//index�ŏo�����l����BGM�Ɣ��̂ŁA�ǂݍ���WavData����SE�̓ǂ݂��񂾐�������.
	// ���̌�index�𑫂�.
	size_t Number = GetInstance()->pWaveData.size() - GetInstance()->m_vsSENameList.size();

	for (size_t i = 0; i < GetSeSize; i++) {
		size_t Number = GetInstance()->pWaveData.size() - GetInstance()->m_vsSENameList.size();
		Number += i;
		//wav����ǂݍ��񂾃f�[�^�ŃT�E���h�f�[�^�쐬.
		GetInstance()->pSeSource[i]->CreateSound(GetInstance()->pWaveData[Number], GetInstance()->pWaveData[Number]->GetFileName());
	}
}








void CSoundManager::CleanBGM(std::string Name)
{
	std::string Cat = Name + ".wav";

	size_t index = GetBGMIndex(Cat);

	clsXAudio2MasterVoice::Release();

	if (GetInstance()->pBgmSource[index]->IsPlaying() != 0) {
		GetInstance()->pBgmSource[index]->StopSource();
	}
}

void CSoundManager::Clean()
{
	//clsXAudio2MasterVoice::Release();

	int PlaySESoundNum = GetInstance()->pSeSource.size();


	for (int i = 0; i < PlaySESoundNum; i++) {
		if (GetInstance()->pSeSource[i]->IsPlaying() != 0) {
			GetInstance()->pSeSource[i]->StopSource();
		}
	}
}

void CSoundManager::Release()
{
	int WavData = GetInstance()->pWaveData.size();
	for (int i = 0; i < WavData; i++) {
		GetInstance()->pWaveData[i]->Close();
	}
	size_t SourceSize = GetInstance()->pSeSource.size();
	for (size_t i = 0; i < SourceSize; i++) {
		GetInstance()->pSeSource[i]->StopSource();
		GetInstance()->pSeSource[i]->DestoroySource();
	}
	SourceSize = GetInstance()->pBgmSource.size();
	for (size_t i = 0; i < SourceSize; i++) {
		GetInstance()->pBgmSource[i]->DestoroySource();
	}
	GetInstance()->pBgmSource.clear();			//�����Clear���Ȃ��ƃ��[�N����.
	GetInstance()->pSeSource.clear();			//�����Clear���Ȃ��ƃ��[�N����.


	GetInstance()->m_bEndGame = true;
}

int CSoundManager::Clamp(int Num, int Min, int Max)
{
	if (Num <= Min) {
		Num = Min;
	}
	else if (Num >= Max) {
		Num = Max;
	}

	return Num;
}

