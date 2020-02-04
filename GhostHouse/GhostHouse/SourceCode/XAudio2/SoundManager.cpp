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

// BGMのリスト作成.
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
		GetInstance()->pWaveData.emplace_back(std::make_shared<clsXAudio2WaveLoad>());	// WavLoadクラスを作成.
		GetInstance()->pWaveData[i]->GetWaveFormat(BGMNameList[i].c_str());		// 作成したクラスで、WavDataを取り出す.
		GetInstance()->m_sBGMIndex[BGMNameList[i]] = i;							// 現在のWavファイル名に対応する
																				// map番号を入れる.
		GetInstance()->CountWavDataSize++;										// 作ったWavLoadクラス分インクリメント.
	}
	BGMNameList.clear();	// 一時的に作ったBGMNameリストクリア.
}
// BGMのインデックスを返す関数.
int CSoundManager::GetBGMIndex(std::string Name)
{
	size_t iErrer = 1;	// リストにない場合のエラー用の数字.
	if (iErrer > GetInstance()->m_sBGMIndex.count(Name)) {
		return -1;
	}
	return GetInstance()->m_sBGMIndex[Name];
}
// PlaySoundクラスに再生命令を飛ばす関数.
void CSoundManager::PlayBGM(int index, bool& isEnd, bool LoopFlag)
{
	//GetBGMIndex()が-1(エラー値)を返してきてたらリターン.
	if (index == -1) {
		return;
	}

	//再生.
	if (GetInstance()->pBgmSource[index] == nullptr) {
		CreateBGMSource();
	}
	// ループフラグが立っていれば、BGMをループ再生.
	if (LoopFlag == true) {
		GetInstance()->pBgmSource[index]->LoopPlay(GetInstance()->pWaveData[index]->GetFileName(), isEnd, GetInstance()->m_bEmergencyStop);
	}
	else{
		GetInstance()->pBgmSource[index]->Play(GetInstance()->pWaveData[index]->GetFileName(),isEnd, GetInstance()->m_bEmergencyStop);
	}
}
// 名前を指定して、その名前に対応した配列番号のBGMを再生する命令を飛ばす関数.
void CSoundManager::PlayBGMByName(std::string Name, bool& isEnd, bool LoopFlag)
{
	if (GetInstance()->m_bEndGame == false) {
		//指定したBGMの名前の後ろに.wav追加.
		std::string Cat = Name + ".wav";

		PlayBGM(GetBGMIndex(Cat), isEnd, LoopFlag);
	}
}
// PlaySoundクラスに停止命令を飛ばす関数.
void CSoundManager::StopBGM(int index)
{
	//GetBGMIndex()が-1(エラー値)を返してきてたらリターン.
	if (index == -1) {
		return;
	}

	GetInstance()->pBgmSource[index]->Stop();
}
// 名前を指定して、その名前に対応した配列番号のBGMを停止する命令を飛ばす関数.
void CSoundManager::StopBGMByName(std::string Name)
{
	//指定したBGMの名前の後ろに.wav追加.
	std::string Cat = Name + ".wav";

	StopBGM(GetBGMIndex(Cat));
}
// BGMの音量を徐々に下げる関数.
bool CSoundManager::FadeOutBGM(int index)
{
	//GetBGMIndex()が-1(エラー値)を返してきてたらリターン.
	if (index == -1) {
		return false;
	}
	// BGMの音量が0より大きい場合、フェードアウトを続ける.
	if (GetInstance()->pBgmSource[index]->GetVolume() > 0) {
		GetInstance()->pBgmSource[index]->SetFadeOutFlag(true);
	}
	// BGMの音量が0になった場合、フェードアウト終了.
	else if (GetInstance()->pBgmSource[index]->GetVolume() == 0) {
		return true;
	}
	return true;
}
// BGMの音量を徐々に下げる命令を飛ばす関数.
void CSoundManager::FadeOutBGMByName(std::string Name)
{
	//指定したBGMの名前の後ろに.wav追加.
	std::string Cat = Name + ".wav";

	FadeOutBGM(GetBGMIndex(Cat));
}
// BGMの音量を徐々に上げる関数.
bool CSoundManager::FadeInBGM(int index)
{
	if (index == -1) {
		return false;
	}
	// BGMの音量が1.0fより小さい場合フェードインを続ける.
	if (GetInstance()->pBgmSource[index]->GetVolume() < 1.0f) {
		GetInstance()->pBgmSource[index]->SetFadeInFlag(true);
	}
	// BGMの音量が1.0fになったらフェードイン終了.
	else if (GetInstance()->pBgmSource[index]->GetVolume() == 1.0f) {
		return true;
	}
	return true;
}
// BGMの音量を徐々に下げる命令を飛ばす関数.
void CSoundManager::FadeInBGMByName(std::string Name)
{
	//指定したBGMの名前の後ろに.wav追加.
	std::string Cat = Name + ".wav";

	FadeInBGM(GetBGMIndex(Cat));
}
// 引数にいれた名前のBGMの音量を返す関数.
float CSoundManager::GetBGMVolme(std::string Name)
{
	// ゲームが閉じられようとしていたら通らない.
	if (GetInstance()->m_bEndGame == false) {
		std::string Cat = Name + ".wav";

		size_t index = GetBGMIndex(Cat);

		// GetBGMIndex()が-1(エラー値)を返してきてたらリターン.
		if (index == -1) {
			return -1;
		}
		// BGMSourceが作られていなければリターン.
		if (GetInstance()->pBgmSource[index] == nullptr) {
			return -1;
		}
		// 音量を返す.
		return GetInstance()->pBgmSource[index]->GetVolume();
	}
	return -1;
}

void CSoundManager::SetBGMVolme(std::string Name, float Volme)
{
	// ゲームが閉じられようとしていたら通らない.
	if (GetInstance()->m_bEndGame == false) {
		std::string Cat = Name + ".wav";

		size_t index = GetBGMIndex(Cat);

		// GetBGMIndex()が-1(エラー値)を返してきてたらリターン.
		if (index == -1) {
			return;
		}
		// 音量をセットする.
		GetInstance()->pBgmSource[index]->SetVolume(Volme);
	}
}

//========================================================================================
//	SE.
//====

// SEのリスト作成.
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

	// BGMのリストを作った際に作った、WavDataの値からループを開始する.
	for (size_t i = GetInstance()->CountWavDataSize; i < BGMandSESize; i++) {
		GetNum = SEListSize - NegativSize;		//配列番号を計算(SEリストサイズが2の場合、2-2... 2-1... 2-0..).
		GetInstance()->pWaveData.emplace_back(std::make_shared<clsXAudio2WaveLoad>()); // WavLoadクラスを作成.
		GetInstance()->pWaveData[i]->GetWaveFormat(SENameList[GetNum].c_str());	// 作成したクラスで、WavDataを取り出す.
		GetInstance()->m_sSEIndex[SENameList[GetNum]] = GetNum;				// 現在のWavファイル名に対応する
																			// map番号を入れる.
		GetInstance()->CountWavDataSize++;									// 作ったWavLoadクラス分インクリメント.
		NegativSize--;														// 配列番号計算時、引く値をデクリメント.							
	}
	SENameList.clear();	// 一時的に作ったSEの名前リストをクリア.
}
// mapの名前に沿った、SEのインデックス番号を返す関数.
int CSoundManager::GetSEIndex(std::string Name)
{
	size_t iErrer = 1;	// リストにない場合のエラー用の数字.
	if (iErrer > GetInstance()->m_sSEIndex.count(Name)) {
		return -1;
	}
	return GetInstance()->m_sSEIndex[Name];
}
// SEを再生する関数.
void CSoundManager::PlaySEByName(
	std::string& Name, bool & isEnd, bool & ChangeSE, bool LoopFlag,bool& IsPlay, bool& RePlay)
{
	bool Reset = true;			//サウンド(SE)をはじめから再生する.
	bool IsError = false;		//エラー検知.
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


		if(IsError == false)
		{
			//indexで出した値だとBGMと被るので、読み込んだWavDataからSEの読みこんだ数引いて.
			// その後indexを足す.
			size_t Number = GetInstance()->pWaveData.size() - GetInstance()->m_vsSENameList.size();
			Number += index;
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
// PlaySoundクラスに停止命令を飛ばす関数.
void CSoundManager::StopSE(int index)
{
	if (index == -1) {
		return;
	}

	GetInstance()->pSeSource[index]->Stop();
}
// 名前を指定して、その名前に対応した配列番号のSEを停止する命令を飛ばす関数.
void CSoundManager::StopSEByName(std::string Name)
{
	//指定したBGMの名前の後ろに.wav追加.
	std::string Cat = Name + ".wav";

	StopSE(GetSEIndex(Cat));
}



// BGMとSEのリストとSoundSourceを作成する関数.
void CSoundManager::InitALL()
{
	// 一回だけしか通れないようにしておく.
	if (GetInstance()->m_bFristCall == true) {		
		GetBGMList();		// BGMのリスト作成.
		GetSEList();		// SEのリスト作成.

		CreateBGMSource();	// BGMのSoundSource作成.
		CreateSESource();	// SEのSoundSource作成.
		//一回だけ通すためのフラグを下す.
		GetInstance()->m_bFristCall = false;
	}
}
// BGMとSEのSoundSourceを作成する関数.
void CSoundManager::InitSource()
{	
	CreateBGMSource();	// BGMのSoundSourceを作成する関数.
	CreateSESource();	// SEのSoundSourceを作成する関数.
}
// BGMのSoundSourceを作成する関数.
void CSoundManager::CreateBGMSource()
{
	// BGMのSoundSourceがすでに作られている || 0個ではない場合.
	// 一度クリアする.
	if (GetInstance()->pBgmSource.size() != 0) 
	{
		GetInstance()->pBgmSource.clear();
	}
	// BGMの要素の数分SoundSource作成.
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
// SEのSoundSourceを作成する関数.
void CSoundManager::CreateSESource()
{
	// SEのSoundSourceがすでに作られている || 0個ではない場合.
	// 一度クリアする.
	if (GetInstance()->pSeSource.size() != 0)
	{
		GetInstance()->pSeSource.clear();
	}
	// SEの要素の数分make_shared.
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
// SEのSoundSourceをクリーン.
void CSoundManager::Clean()
{
	int PlaySESoundNum = GetInstance()->pSeSource.size();


	for (int i = 0; i < PlaySESoundNum; i++) {
		if (GetInstance()->pSeSource[i]->IsPlaying() != 0) {
			GetInstance()->pSeSource[i]->StopSource();
		}
	}
}



// 後処理用、XAudio2関係開放関数.
void CSoundManager::Release()
{
	// WavData数を取得.
	int WavData = GetInstance()->pWaveData.size();
	// 数分ループしてWavDataを閉じる.
	for (int i = 0; i < WavData; i++) {
		GetInstance()->pWaveData[i]->Close();
	}
	// SEのSoundSource数を取得.
	size_t SourceSize = GetInstance()->pSeSource.size();
	// 数分ループしてSoundSourceを再生中なら停止した後、破壊する.
	for (size_t i = 0; i < SourceSize; i++) {
		GetInstance()->pSeSource[i]->StopSource();
		GetInstance()->pSeSource[i]->DestoroySource();
	}

	// BGMのSoundSource数を取得.
	SourceSize = GetInstance()->pBgmSource.size();
	for (size_t i = 0; i < SourceSize; i++) {
		// 数分ループしてSoundSourceを再生中なら停止した後、破壊する.
		GetInstance()->pBgmSource[i]->DestoroySource();
	}
	GetInstance()->pBgmSource.clear();			
	GetInstance()->pSeSource.clear();			

	// 破壊後にサウンド関係の関数を呼ばないために.
	// ゲーム終了フラグを立てる.
	// ここを通った後、サウンド関係の関数を通ることはない.
	GetInstance()->m_bEndGame = true;
}
