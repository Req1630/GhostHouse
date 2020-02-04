#include "SoundManager.h"
#include <crtdbg.h>

CSoundManager::CSoundManager()
	: CountWavDataSize(0)
	, m_bFristCall ( true )
	, m_bEndGame	( false )
	, m_bEmergencyStop	( false )
{
}

CSoundManager::~CSoundManager()
{
}



//========================================================================================
//	BGM.
//====

// BGM�̃��X�g�쐬.
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
		GetInstance()->pWaveData.emplace_back(std::make_shared<clsXAudio2WaveLoad>());	// WavLoad�N���X���쐬.
		GetInstance()->pWaveData[i]->GetWaveFormat(BGMNameList[i].c_str());		// �쐬�����N���X�ŁAWavData�����o��.
		GetInstance()->m_sBGMIndex[BGMNameList[i]] = i;							// ���݂�Wav�t�@�C�����ɑΉ�����
																				// map�ԍ�������.
		GetInstance()->CountWavDataSize++;										// �����WavLoad�N���X���C���N�������g.
	}
	BGMNameList.clear();	// �ꎞ�I�ɍ����BGMName���X�g�N���A.
}
// BGM�̃C���f�b�N�X��Ԃ��֐�.
int CSoundManager::GetBGMIndex(std::string Name)
{
	size_t iErrer = 1;	// ���X�g�ɂȂ��ꍇ�̃G���[�p�̐���.
	if (iErrer > GetInstance()->m_sBGMIndex.count(Name)) {
		return -1;
	}
	return GetInstance()->m_sBGMIndex[Name];
}
// PlaySound�N���X�ɍĐ����߂��΂��֐�.
void CSoundManager::PlayBGM(int index, bool& isEnd, bool LoopFlag)
{
	//GetBGMIndex()��-1(�G���[�l)��Ԃ��Ă��Ă��烊�^�[��.
	if (index == -1) {
		return;
	}

	//�Đ�.
	if (GetInstance()->pBgmSource[index] == nullptr) {
		CreateBGMSource();
	}
	// ���[�v�t���O�������Ă���΁ABGM�����[�v�Đ�.
	if (LoopFlag == true) {
		GetInstance()->pBgmSource[index]->LoopPlay(GetInstance()->pWaveData[index]->GetFileName(), isEnd, GetInstance()->m_bEmergencyStop);
	}
	else{
		GetInstance()->pBgmSource[index]->Play(GetInstance()->pWaveData[index]->GetFileName(),isEnd, GetInstance()->m_bEmergencyStop);
	}
}
// ���O���w�肵�āA���̖��O�ɑΉ������z��ԍ���BGM���Đ����閽�߂��΂��֐�.
void CSoundManager::PlayBGMByName(std::string Name, bool& isEnd, bool LoopFlag)
{
	if (GetInstance()->m_bEndGame == false) {
		//�w�肵��BGM�̖��O�̌���.wav�ǉ�.
		std::string Cat = Name + ".wav";

		PlayBGM(GetBGMIndex(Cat), isEnd, LoopFlag);
	}
}
// PlaySound�N���X�ɒ�~���߂��΂��֐�.
void CSoundManager::StopBGM(int index)
{
	//GetBGMIndex()��-1(�G���[�l)��Ԃ��Ă��Ă��烊�^�[��.
	if (index == -1) {
		return;
	}

	GetInstance()->pBgmSource[index]->Stop();
}
// ���O���w�肵�āA���̖��O�ɑΉ������z��ԍ���BGM���~���閽�߂��΂��֐�.
void CSoundManager::StopBGMByName(std::string Name)
{
	//�w�肵��BGM�̖��O�̌���.wav�ǉ�.
	std::string Cat = Name + ".wav";

	StopBGM(GetBGMIndex(Cat));
}
// BGM�̉��ʂ����X�ɉ�����֐�.
bool CSoundManager::FadeOutBGM(int index)
{
	//GetBGMIndex()��-1(�G���[�l)��Ԃ��Ă��Ă��烊�^�[��.
	if (index == -1) {
		return false;
	}
	// BGM�̉��ʂ�0���傫���ꍇ�A�t�F�[�h�A�E�g�𑱂���.
	if (GetInstance()->pBgmSource[index]->GetVolume() > 0) {
		GetInstance()->pBgmSource[index]->SetFadeOutFlag(true);
	}
	// BGM�̉��ʂ�0�ɂȂ����ꍇ�A�t�F�[�h�A�E�g�I��.
	else if (GetInstance()->pBgmSource[index]->GetVolume() == 0) {
		return true;
	}
	return true;
}
// BGM�̉��ʂ����X�ɉ����閽�߂��΂��֐�.
void CSoundManager::FadeOutBGMByName(std::string Name)
{
	//�w�肵��BGM�̖��O�̌���.wav�ǉ�.
	std::string Cat = Name + ".wav";

	FadeOutBGM(GetBGMIndex(Cat));
}
// BGM�̉��ʂ����X�ɏグ��֐�.
bool CSoundManager::FadeInBGM(int index)
{
	if (index == -1) {
		return false;
	}
	// BGM�̉��ʂ�1.0f��菬�����ꍇ�t�F�[�h�C���𑱂���.
	if (GetInstance()->pBgmSource[index]->GetVolume() < 1.0f) {
		GetInstance()->pBgmSource[index]->SetFadeInFlag(true);
	}
	// BGM�̉��ʂ�1.0f�ɂȂ�����t�F�[�h�C���I��.
	else if (GetInstance()->pBgmSource[index]->GetVolume() == 1.0f) {
		return true;
	}
	return true;
}
// BGM�̉��ʂ����X�ɉ����閽�߂��΂��֐�.
void CSoundManager::FadeInBGMByName(std::string Name)
{
	//�w�肵��BGM�̖��O�̌���.wav�ǉ�.
	std::string Cat = Name + ".wav";

	FadeInBGM(GetBGMIndex(Cat));
}
// �����ɂ��ꂽ���O��BGM�̉��ʂ�Ԃ��֐�.
float CSoundManager::GetBGMVolme(std::string Name)
{
	// �Q�[���������悤�Ƃ��Ă�����ʂ�Ȃ�.
	if (GetInstance()->m_bEndGame == false) {
		std::string Cat = Name + ".wav";

		size_t index = GetBGMIndex(Cat);

		// GetBGMIndex()��-1(�G���[�l)��Ԃ��Ă��Ă��烊�^�[��.
		if (index == -1) {
			return -1;
		}
		// BGMSource������Ă��Ȃ���΃��^�[��.
		if (GetInstance()->pBgmSource[index] == nullptr) {
			return -1;
		}
		// ���ʂ�Ԃ�.
		return GetInstance()->pBgmSource[index]->GetVolume();
	}
	return -1;
}

void CSoundManager::SetBGMVolme(std::string Name, float Volme)
{
	// �Q�[���������悤�Ƃ��Ă�����ʂ�Ȃ�.
	if (GetInstance()->m_bEndGame == false) {
		std::string Cat = Name + ".wav";

		size_t index = GetBGMIndex(Cat);

		// GetBGMIndex()��-1(�G���[�l)��Ԃ��Ă��Ă��烊�^�[��.
		if (index == -1) {
			return;
		}
		// ���ʂ��Z�b�g����.
		GetInstance()->pBgmSource[index]->SetVolume(Volme);
	}
}

//========================================================================================
//	SE.
//====

// SE�̃��X�g�쐬.
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

	// BGM�̃��X�g��������ۂɍ�����AWavData�̒l���烋�[�v���J�n����.
	for (size_t i = GetInstance()->CountWavDataSize; i < BGMandSESize; i++) {
		GetNum = SEListSize - NegativSize;		//�z��ԍ����v�Z(SE���X�g�T�C�Y��2�̏ꍇ�A2-2... 2-1... 2-0..).
		GetInstance()->pWaveData.emplace_back(std::make_shared<clsXAudio2WaveLoad>()); // WavLoad�N���X���쐬.
		GetInstance()->pWaveData[i]->GetWaveFormat(SENameList[GetNum].c_str());	// �쐬�����N���X�ŁAWavData�����o��.
		GetInstance()->m_sSEIndex[SENameList[GetNum]] = GetNum;				// ���݂�Wav�t�@�C�����ɑΉ�����
																			// map�ԍ�������.
		GetInstance()->CountWavDataSize++;									// �����WavLoad�N���X���C���N�������g.
		NegativSize--;														// �z��ԍ��v�Z���A�����l���f�N�������g.							
	}
	SENameList.clear();	// �ꎞ�I�ɍ����SE�̖��O���X�g���N���A.
}
// map�̖��O�ɉ������ASE�̃C���f�b�N�X�ԍ���Ԃ��֐�.
int CSoundManager::GetSEIndex(std::string Name)
{
	size_t iErrer = 1;	// ���X�g�ɂȂ��ꍇ�̃G���[�p�̐���.
	if (iErrer > GetInstance()->m_sSEIndex.count(Name)) {
		return -1;
	}
	return GetInstance()->m_sSEIndex[Name];
}
// SE���Đ�����֐�.
void CSoundManager::PlaySEByName(
	std::string& Name, bool & isEnd, bool & ChangeSE, bool LoopFlag,bool& IsPlay, bool& RePlay)
{
	bool Reset = true;			//�T�E���h(SE)���͂��߂���Đ�����.
	bool IsError = false;		//�G���[���m.
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


		if(IsError == false)
		{
			//index�ŏo�����l����BGM�Ɣ��̂ŁA�ǂݍ���WavData����SE�̓ǂ݂��񂾐�������.
			// ���̌�index�𑫂�.
			size_t Number = GetInstance()->pWaveData.size() - GetInstance()->m_vsSENameList.size();
			Number += index;
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
// PlaySound�N���X�ɒ�~���߂��΂��֐�.
void CSoundManager::StopSE(int index)
{
	if (index == -1) {
		return;
	}

	GetInstance()->pSeSource[index]->Stop();
}
// ���O���w�肵�āA���̖��O�ɑΉ������z��ԍ���SE���~���閽�߂��΂��֐�.
void CSoundManager::StopSEByName(std::string Name)
{
	//�w�肵��BGM�̖��O�̌���.wav�ǉ�.
	std::string Cat = Name + ".wav";

	StopSE(GetSEIndex(Cat));
}



// BGM��SE�̃��X�g��SoundSource���쐬����֐�.
void CSoundManager::InitALL()
{
	// ��񂾂������ʂ�Ȃ��悤�ɂ��Ă���.
	if (GetInstance()->m_bFristCall == true) {		
		GetBGMList();		// BGM�̃��X�g�쐬.
		GetSEList();		// SE�̃��X�g�쐬.

		CreateBGMSource();	// BGM��SoundSource�쐬.
		CreateSESource();	// SE��SoundSource�쐬.
		//��񂾂��ʂ����߂̃t���O������.
		GetInstance()->m_bFristCall = false;
	}
}
// BGM��SE��SoundSource���쐬����֐�.
void CSoundManager::InitSource()
{	
	CreateBGMSource();	// BGM��SoundSource���쐬����֐�.
	CreateSESource();	// SE��SoundSource���쐬����֐�.
}
// BGM��SoundSource���쐬����֐�.
void CSoundManager::CreateBGMSource()
{
	// BGM��SoundSource�����łɍ���Ă��� || 0�ł͂Ȃ��ꍇ.
	// ��x�N���A����.
	if (GetInstance()->pBgmSource.size() != 0) 
	{
		GetInstance()->pBgmSource.clear();
	}
	// BGM�̗v�f�̐���SoundSource�쐬.
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
// SE��SoundSource���쐬����֐�.
void CSoundManager::CreateSESource()
{
	// SE��SoundSource�����łɍ���Ă��� || 0�ł͂Ȃ��ꍇ.
	// ��x�N���A����.
	if (GetInstance()->pSeSource.size() != 0)
	{
		GetInstance()->pSeSource.clear();
	}
	// SE�̗v�f�̐���make_shared.
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
// SE��SoundSource���N���[��.
void CSoundManager::Clean()
{
	int PlaySESoundNum = GetInstance()->pSeSource.size();


	for (int i = 0; i < PlaySESoundNum; i++) {
		if (GetInstance()->pSeSource[i]->IsPlaying() != 0) {
			GetInstance()->pSeSource[i]->StopSource();
		}
	}
}



// �㏈���p�AXAudio2�֌W�J���֐�.
void CSoundManager::Release()
{
	// WavData�����擾.
	int WavData = GetInstance()->pWaveData.size();
	// �������[�v����WavData�����.
	for (int i = 0; i < WavData; i++) {
		GetInstance()->pWaveData[i]->Close();
	}
	// SE��SoundSource�����擾.
	size_t SourceSize = GetInstance()->pSeSource.size();
	// �������[�v����SoundSource���Đ����Ȃ��~������A�j�󂷂�.
	for (size_t i = 0; i < SourceSize; i++) {
		GetInstance()->pSeSource[i]->StopSource();
		GetInstance()->pSeSource[i]->DestoroySource();
	}

	// BGM��SoundSource�����擾.
	SourceSize = GetInstance()->pBgmSource.size();
	for (size_t i = 0; i < SourceSize; i++) {
		// �������[�v����SoundSource���Đ����Ȃ��~������A�j�󂷂�.
		GetInstance()->pBgmSource[i]->DestoroySource();
	}
	GetInstance()->pBgmSource.clear();			
	GetInstance()->pSeSource.clear();			

	// �j���ɃT�E���h�֌W�̊֐����Ă΂Ȃ����߂�.
	// �Q�[���I���t���O�𗧂Ă�.
	// ������ʂ�����A�T�E���h�֌W�̊֐���ʂ邱�Ƃ͂Ȃ�.
	GetInstance()->m_bEndGame = true;
}
