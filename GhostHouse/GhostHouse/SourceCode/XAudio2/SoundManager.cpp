#include "SoundManager.h"
#include <crtdbg.h>

CSoundManager::CSoundManager()
	: CountWavDataSize(0)
	, m_bFristCall ( true )
	, m_bEndGame	( false )
	, m_bEmergencyStop	( false )
{
	// 必要じゃないかも.
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
	WIN32_FIND_DATA FindFileData;					//ファイル検索用.
	HANDLE hFind;									//ハンドル.
	std::string filePath = "Data\\Sound\\BGM\\*";	//BGMが入ってるファイル階層パス.
	std::vector<std::string> BGMNameList;			//BGMリスト用.

	hFind = FindFirstFile(filePath.c_str(), &FindFileData);
	if (hFind != FreeLibrary) {
		do {
			BGMNameList.emplace_back(FindFileData.cFileName);
		} while (FindNextFile(hFind, &FindFileData));
		// m_vsStageList : [0]"." [1]".." の削除.
		BGMNameList.erase(BGMNameList.begin(), BGMNameList.begin() + 2);
		FindClose(hFind);
	}
	//SoundManagerのメンバー変数に渡す.
	GetInstance()->m_vsBGMNameList = BGMNameList;
	//BGMリストに入ったサイズ分ループしてWavDataをGetして作る.
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
	size_t iErrer = 1;	// リストにない場合のエラー用の数字.
	if (iErrer > GetInstance()->m_sBGMIndex.count(Name)) {
		return -1;
	}
	return GetInstance()->m_sBGMIndex[Name];
}

void CSoundManager::PlayBGM(int index, bool& isEnd, bool LoopFlag)
{
	//GetBGMIndex()が-1(エラー値)を返してきてたらリターン.
	if (index == -1) {
		return;
	}
	//index = GetInstance()->Clamp(index, 0, GetInstance()->m_vsBGMNameList.size());
	


	//ループフラグ設定.
	//GetInstance()->pBgmSource[index]->SetLoopFlag(true);
	//再生.
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
		//指定したBGMの名前の後ろに.wav追加.
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
	//指定したBGMの名前の後ろに.wav追加.
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
	//指定したBGMの名前の後ろに.wav追加.
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
	//指定したBGMの名前の後ろに.wav追加.
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
	//指定したBGMの名前の後ろに.wav追加.
	std::string Cat = Name + ".wav";

	PauseBGM(GetBGMIndex(Cat));
}

float CSoundManager::GetBGMVolme(std::string Name)
{
	if (GetInstance()->m_bEndGame == false) {
		std::string Cat = Name + ".wav";

		size_t index = GetBGMIndex(Cat);

		//GetBGMIndex()が-1(エラー値)を返してきてたらリターン.
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

		//GetBGMIndex()が-1(エラー値)を返してきてたらリターン.
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
	WIN32_FIND_DATA FindFileData;					//ファイル検索用.
	HANDLE hFind;									//ハンドル.
	std::string filePath = "Data\\Sound\\SE\\*";	//SEが入ってるファイル階層パス.
	std::vector<std::string> SENameList;			//SEリスト用.

	hFind = FindFirstFile(filePath.c_str(), &FindFileData);
	if (hFind != FreeLibrary) {
		do {
			SENameList.emplace_back(FindFileData.cFileName);
		} while (FindNextFile(hFind, &FindFileData));
		// m_vsStageList : [0]"." [1]".." の削除.
		SENameList.erase(SENameList.begin(), SENameList.begin() + 2);
		FindClose(hFind);
	}
	GetInstance()->m_vsSENameList = SENameList;

	size_t SEListSize = SENameList.size();			//SoundManagerのメンバ変数に渡す.
	size_t GetNum;									//SENameリストの配列番号計算箱.
	size_t NegativSize = SEListSize;				//SENameリストの配列番号減算値.
	//BGMのリスト数とSEのリスト数を足す.
	size_t BGMandSESize = GetInstance()->m_vsBGMNameList.size() + GetInstance()->m_vsSENameList.size();

	for (size_t i = GetInstance()->CountWavDataSize; i < BGMandSESize; i++) {
		GetNum = SEListSize - NegativSize;	//配列番号を計算(SEリストサイズが2の場合、2-2... 2-1... 2-0..).
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
	size_t iErrer = 1;	// リストにない場合のエラー用の数字.
	if (iErrer > GetInstance()->m_sSEIndex.count(Name)) {
		return -1;
	}
	return GetInstance()->m_sSEIndex[Name];
}

void CSoundManager::PlaySEByName(
	std::string& Name, bool & isEnd, bool & ChangeSE, bool LoopFlag,bool& IsPlay, bool& RePlay)
{
	bool Reset = true;			//サウンド(SE)をはじめから再生する.
	bool IsError = false;		//エラー対策.
	bool ReturnTop = false;		//下で再生終了してwhile待機している実行位置をこのブロックの上に戻すフラグ.

	while (isEnd == false)
	{
		Reset = true;			
		IsError = false;		
		ReturnTop = false;		
		//指定したBGMの名前の後ろに.wav追加.
		std::string Cat = Name + ".wav";

		size_t index = GetSEIndex(Cat);

		//GetBGMIndex()が-1(エラー値)を返してきてたらリターン.
		if (index == -1) {
			IsError = true;
		}

		//indexで出した値だとBGMと被るので、読み込んだWavDataからSEの読みこんだ数引いて.
		// その後indexを足す.
		if(IsError == false)
		{
			size_t Number = GetInstance()->pWaveData.size() - GetInstance()->m_vsSENameList.size();
			Number += index;
			//ループフラグ設定.
			//GetInstance()->pBgmSource[index]->SetLoopFlag(true);
			//再生.
			if (LoopFlag == true) {
				GetInstance()->pSeSource[index]->LoopPlay(GetInstance()->pWaveData[Number]->GetFileName(), isEnd, GetInstance()->m_bEmergencyStop);
			}
			else {

				GetInstance()->pSeSource[index]->SEPlay(
					GetInstance()->pWaveData[Number]->GetFileName(), isEnd, Reset, IsPlay, RePlay, GetInstance()->m_bEmergencyStop);
			}
			IsPlay = false;
		}
		//再生終了後、新たなSEが入って来るまで待機.
		while ( ReturnTop == false)
		{
			//新たなSE再生命令 or 終了解放命令が飛んで来たら中へ.
			if (ChangeSE == true) {
				ChangeSE = false;
				ReturnTop = true;
			}
			if (isEnd == true || GetInstance()->m_bEmergencyStop == true) {
				break;
			}
		}
		//上のwhileを抜けている場合ここに絶対入る.
		if (ReturnTop == true) {
			//解放命令が出ていた場合breakして、スレッドを解放する.
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
	//指定したBGMの名前の後ろに.wav追加.
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
	//指定したBGMの名前の後ろに.wav追加.
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
	//BGMの要素の数分make_shared.
	// ※初回(pBgmSourceが作られていない)場合のみ通る.
	if (GetInstance()->pBgmSource.size() != 0) 
	{
		GetInstance()->pBgmSource.clear();
	}
	for (size_t i = 0; i < GetInstance()->m_vsBGMNameList.size(); i++) {
		GetInstance()->pBgmSource.emplace_back(std::make_shared<clsXAudio2PlaySound>());
	}
	//エラーチェック.
	size_t GetBGMSize = GetInstance()->pBgmSource.size();
	if (GetBGMSize != GetInstance()->m_vsBGMNameList.size()) {
		return;
	}
	for (size_t i = 0; i < GetBGMSize; i++) {
		//wavから読み込んだデータでサウンドデータ作成.
		GetInstance()->pBgmSource[i]->CreateSound(GetInstance()->pWaveData[i], GetInstance()->pWaveData[i]->GetFileName());
	}
}

void CSoundManager::CreateSESource()
{
	//BGMの要素の数分make_shared.
// ※初回(pBgmSourceが作られていない)場合のみ通る.
	if (GetInstance()->pSeSource.size() != 0)
	{
		GetInstance()->pSeSource.clear();
	}

	for (size_t i = 0; i < GetInstance()->m_vsSENameList.size(); i++) {
		GetInstance()->pSeSource.emplace_back(std::make_shared<clsXAudio2PlaySound>());
	}

	//エラーチェック.
	size_t GetSeSize = GetInstance()->pSeSource.size();
	if (GetSeSize != GetInstance()->m_vsSENameList.size()) {
		return;
	}
	//indexで出した値だとBGMと被るので、読み込んだWavDataからSEの読みこんだ数引いて.
	// その後indexを足す.
	size_t Number = GetInstance()->pWaveData.size() - GetInstance()->m_vsSENameList.size();

	for (size_t i = 0; i < GetSeSize; i++) {
		size_t Number = GetInstance()->pWaveData.size() - GetInstance()->m_vsSENameList.size();
		Number += i;
		//wavから読み込んだデータでサウンドデータ作成.
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
	GetInstance()->pBgmSource.clear();			//これはClearしないとリークする.
	GetInstance()->pSeSource.clear();			//これはClearしないとリークする.


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

