#include "SoundManager.h"
#include <crtdbg.h>
#include <filesystem>	// C++17 �K�{.
namespace fs = std::filesystem;
CSoundManager::CSoundManager()
	: m_bEndGame		( false )
	, m_bEmergencyStop	( false )
	, m_bFristCall		( true )
{
}

CSoundManager::~CSoundManager()
{
}
// �C���X�^���X�̍쐬.
CSoundManager* CSoundManager::GetInstance()
{
	static std::unique_ptr<CSoundManager> pInstance =
		std::make_unique<CSoundManager>();	// �C���X�^���X�̍쐬.
	return pInstance.get();
}

//========================================================================================
//	�T�E���h�f�[�^�쐬.
//====
void CSoundManager::CreateSoundData()
{
	if (GetInstance()->m_bFristCall == true) {

		auto eachLoad = [&](const fs::directory_entry& entry)
		{
			const std::string extension = entry.path().extension().string();	// �g���q.
			const std::string filePath = entry.path().string();					// �t�@�C���p�X.
			const std::string fileName = entry.path().stem().string();			// �t�@�C����.

			if (extension != ".wav" && extension != ".WAV") return;	// �g���q�� "wav�t�@�C��" ����Ȃ���ΏI��.
			//�t�@�C���p�X����BGM�������BGM�A�Ȃ����SE.
			if (filePath.find("BGM") == std::string::npos) {
				// WavLoad�N���X�쐬.
				GetInstance()->pWaveData[fileName] = std::make_shared<clsXAudio2WaveLoad>();
				// Wav�t�H�[�}�b�g�擾.
				GetInstance()->pWaveData[fileName]->GetWaveFormat(fileName.c_str());
				// PlaySound�N���X�쐬.
				GetInstance()->pSeSource[fileName] = std::make_shared<clsXAudio2PlaySound>();
				// SE��SoundSource�쐬.
				GetInstance()->pSeSource[fileName]->CreateSound(GetInstance()->pWaveData[fileName], GetInstance()->pWaveData[fileName]->GetFileName());
				// �t�H���_�̖��O��SE���O���X�g�ɓ����.
				GetInstance()->m_vsSENameList.emplace_back(fileName);
			}
			else {
				// WavLoad�N���X�쐬.
				GetInstance()->pWaveData[fileName] = std::make_shared<clsXAudio2WaveLoad>();
				// Wav�t�H�[�}�b�g�擾.
				GetInstance()->pWaveData[fileName]->GetWaveFormat(fileName.c_str());
				// PlaySound�N���X�쐬.
				GetInstance()->pBgmSource[fileName] = std::make_shared<clsXAudio2PlaySound>();
				// BGM��SoundSource�쐬.
				GetInstance()->pBgmSource[fileName]->CreateSound(GetInstance()->pWaveData[fileName], GetInstance()->pWaveData[fileName]->GetFileName());
				// �t�H���_�̖��O��SE���O���X�g�ɓ����.
				GetInstance()->m_vsBGMNameList.emplace_back(fileName);
			}
		};

		try {
			//�w��p�X(FILE_PATH)�f�B���N�g�������ċA�I�ɑ���
			fs::recursive_directory_iterator dir_itr(GetInstance()->FILE_PATH), end_itr;
			// eachLoad�֐����s��.
			std::for_each(dir_itr, end_itr, eachLoad);

		}
		// �G���[�L���b�`.
		catch (const fs::filesystem_error& e) {
			const char* errorMessage = e.path1().string().c_str();
			_ASSERT_EXPR(false, L"�T�E���h�f�[�^�쐬���s");
			MessageBox(nullptr, errorMessage, "�T�E���h�f�[�^�쐬���s", MB_OK);
		}

		GetInstance()->m_bFristCall = false;
	}
}
//========================================================================================
//	BGM.
//====

// BGM�Đ��֐�.
void CSoundManager::PlayBGM(std::string Name, bool& isEnd, bool LoopFlag)
{
	// ���݂��Ȃ���΃��^�[��.
	if (GetInstance()->pBgmSource[Name] == nullptr) return;
	// �Q�[���I���t���O�������Ă���΃��^�[��.
	if (GetInstance()->m_bEndGame == true) return;
	// ���[�v�t���O��true�Ȃ烋�[�v�ōĐ�.
	if (LoopFlag == true) {
		GetInstance()->pBgmSource[Name]->LoopPlay(GetInstance()->pWaveData[Name]->GetFileName(), isEnd, GetInstance()->m_bEmergencyStop);
	}
	else {
		GetInstance()->pBgmSource[Name]->Play(GetInstance()->pWaveData[Name]->GetFileName(), isEnd, GetInstance()->m_bEmergencyStop);
	}
}
// BGM��~�֐�.
void CSoundManager::StopBGM(std::string Name)
{
	// ���݂��Ȃ���΃��^�[��.
	if (GetInstance()->pBgmSource[Name] == nullptr) return;
	// �Q�[���I���t���O�������Ă���΃��^�[��.
	if (GetInstance()->m_bEndGame == true) return;
	// �w�肵��BGM��~.
	GetInstance()->pBgmSource[Name]->Stop();
}
// BGM�t�F�[�h�A�E�g�֐�.
bool CSoundManager::FadeOutBGM(std::string Name)
{
	// ���݂��Ȃ���΃��^�[��.
	if (GetInstance()->pBgmSource[Name] == nullptr) return true;

	// �Q�[���I���t���O�������Ă���΃��^�[��.
	if (GetInstance()->m_bEndGame == true) return true;
	// �w�肵��BGM�̉��ʂ�0���傫����΃t�F�[�h�A�E�g�t���O�𗧂Ă�.
	if (GetInstance()->pBgmSource[Name]->GetVolume() > 0) {
		GetInstance()->pBgmSource[Name]->SetFadeOutFlag(true);
	}
	else if (GetInstance()->pBgmSource[Name]->GetVolume() == 0) {
		return true;
	}

	return true;
}
// BGM�t�F�[�h�C���֐�.
bool CSoundManager::FadeInBGM(std::string Name)
{
	// ���݂��Ȃ���΃��^�[��.
	if (GetInstance()->pBgmSource[Name] == nullptr) return true;

	// �Q�[���I���t���O�������Ă���΃��^�[��.
	if (GetInstance()->m_bEndGame == true) return true;	
	// �w�肵��BGM�̉��ʂ�1.0f(�ő�)��菬������΃t�F�[�h�C���t���O�𗧂Ă�.
	if (GetInstance()->pBgmSource[Name]->GetVolume() < 1.0f) {
		GetInstance()->pBgmSource[Name]->SetFadeInFlag(true);
	}
	else if (GetInstance()->pBgmSource[Name]->GetVolume() == 1.0f) {
		return true;
	}

	return true;
}
// �w�肵��BGM�̉��ʂ��擾����֐�.
float CSoundManager::GetBGMVolme(std::string Name)
{
	// �Q�[���I���t���O�������Ă���΃��^�[�� -1.
	if (GetInstance()->m_bEndGame == true) return -1;	
	// ���݂��Ȃ���΃��^�[��.
	if (GetInstance()->pBgmSource[Name] == nullptr) return -1;

	return GetInstance()->pBgmSource[Name]->GetVolume();
}
// �w�肵��BGM�̉��ʂ��Z�b�g����֐�.
void CSoundManager::SetBGMVolme(std::string Name, float Volme)
{
	// �Q�[���I���t���O�������Ă���΃��^�[��.
	if (GetInstance()->m_bEndGame == true) return;

	GetInstance()->pBgmSource[Name]->SetVolume(Volme);
}

//========================================================================================
//	SE.
//====

// SE�Đ��֐�.
void CSoundManager::PlaySE(
	std::string& Name, bool & isEnd, bool & ChangeSE, bool& IsPlay, bool& RePlay)
{
	bool Reset = true;			// �T�E���h(SE)���͂��߂���Đ�����.
	bool IsError = false;		// �G���[�΍�.
	bool ReturnTop = false;		// ���ōĐ��I������while�ҋ@���Ă�����s�ʒu�����̃u���b�N�̏�ɖ߂��t���O.
	// �I���t���O�������Ă��Ȃ��ꍇ���[�v����.
	while (isEnd == false)
	{
		// �ϐ����Z�b�g.
		Reset = true;			
		IsError = false;		
		ReturnTop = false;		

		// ���݂��Ȃ���΃G���[.
		if (GetInstance()->pSeSource[Name] == nullptr) {
			IsError = true;
		}
		if(IsError == false)
		{
			size_t Number = GetInstance()->pWaveData.size() - GetInstance()->m_vsSENameList.size();
			// �Đ�.
			GetInstance()->pSeSource[Name]->SEPlay(
				GetInstance()->pWaveData[Name]->GetFileName(), isEnd, Reset, IsPlay, RePlay, GetInstance()->m_bEmergencyStop);
			// �����ɗ�����SE�̍Đ����I����Ă���̂ŁA�Đ����ɗ����Ă���t���O������.
			IsPlay = false;
		}
		// �Đ��I����A�V����SE�������ė���܂őҋ@.
		while ( ReturnTop == false)
		{
			// �V����SE�Đ����� or �I��������߂����ŗ����璆��.
			if (ChangeSE == true) {
				ChangeSE = false;
				ReturnTop = true;
			}
			if (isEnd == true || GetInstance()->m_bEmergencyStop == true) {
				break;
			}
		}
		// ���while�𔲂��Ă���ꍇ�����ɐ�Γ���.
		if (ReturnTop == true) {
			// ������߂��o�Ă����ꍇbreak���āA�X���b�h���������.
			if (isEnd == true || GetInstance()->m_bEmergencyStop == true) {
				break;
			}
			continue;
		}

	}

}
// SE��~�֐�.
void CSoundManager::StopSE(std::string Name)
{
	GetInstance()->pSeSource[Name]->Stop();
}
// ����֐�.
void CSoundManager::Release()
{
	// SE�̖��O���X�g�������[�v����.
	for (size_t i = 0; i < GetInstance()->m_vsSENameList.size(); i++) {
		GetInstance()->pWaveData[GetInstance()->m_vsSENameList[i]]->Close();			//WavData�����.
		GetInstance()->pSeSource[GetInstance()->m_vsSENameList[i]]->DestoroySource();	// SoundSource�����.
	}
	// BGM�̖��O���X�g�������[�v����.
	for (size_t i = 0; i < GetInstance()->m_vsBGMNameList.size(); i++) {	
		GetInstance()->pWaveData[GetInstance()->m_vsBGMNameList[i]]->Close();			//WavData�����.
		GetInstance()->pBgmSource[GetInstance()->m_vsBGMNameList[i]]->DestoroySource();	// SoundSource�����.
	}
	// �Q�[���I���t���O�𗧂Ă�.
	GetInstance()->m_bEndGame = true;
}
