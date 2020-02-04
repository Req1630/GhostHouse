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
	// BGMのリストをファイルを探索して作る.
	static void GetBGMList();
	// リストに名前が入っていれば、そのBGMのindex番号取得関数.
	// 存在していない物を指定した場合、エラー値 -1 が返って来る.
	static int GetBGMIndex(std::string Name);
	// BGM再生.
	static void PlayBGM(int index, bool& isEnd, bool LoopFlag);
	// BGMを再生する関数をBGMの名前から呼び出す関数.
	static void PlayBGMByName(std::string Name, bool& isEnd, bool LoopFlag);
	// BGM停止.
	static void StopBGM(int index);
	// BGMを停止する関数をBGMの名前から呼び出す関数.
	static void StopBGMByName(std::string Name);
	// BGMの音量を徐々に小さくしていく命令をPlaySound.cppに飛ばす関数.
	static bool FadeOutBGM(int index);
	// FadeOutBGM関数を呼び出す関数.
	static void FadeOutBGMByName(std::string Name);
	// BGMの音量を徐々に大きくしていく命令をPlaySound.cppに飛ばす関数.
	static bool FadeInBGM(int index);
	// FadeInBGM関数を呼び出す関数.
	static void FadeInBGMByName(std::string Name);	
	// BGMの音量を取得する関数.
	static float GetBGMVolme(std::string Name);
	// BGMの音量をセットする関数.
	static void SetBGMVolme(std::string Name, float Volme);
//========================================================================================
//	SE
//====
	// SEのリストをファイルを探索して作る.
	static void GetSEList();
	// リストに名前が入っていれば、そのSEのindex番号取得関数.
	// 存在していない物を指定した場合、エラー値 -1 が返って来る.
	static int GetSEIndex(std::string Name);
	// SE再生関数.
	static void PlaySEByName(std::string& Name, bool & isEnd, bool & ChangeSE, bool LoopFlag,bool& IsPlay,bool& RePlay);
	// SE停止関数.
	static void StopSE(int index);
	// StopSE関数を呼び出す関数.
	static void StopSEByName(std::string Name);
	// BGMとSEのリストとSoundSourceを作成する関数.
	static void InitALL();
	// BGMとSEのSoundSourceを作成する関数.
	static void InitSource();
	// BGMのSoundSourceを作成する関数.
	static void CreateBGMSource();
	// SEのSoundSourceを作成する関数.
	static void CreateSESource();

	static void CleanBGM(std::string Name);
	// SEのSoundSourceをクリーン.
	static void Clean();

	// 後処理用、XAudio2関係開放関数.
	static void Release();
	// アプリを急に強制終了させた時に、BGMを停止させるためのフラグをセットする関数.
	// これでフラグをセットして停止させないと、XAudio2がBGMを再生中に開放しようとして、エラーを出す.
	static void SetEmergencyStopFlag(bool StopFlag) { GetInstance()->m_bEmergencyStop = StopFlag; }

private:
	// 一回だけ関数を呼ぶためのフラグ
	bool m_bFristCall;
	// wavファイルのデータを入れるクラスの変数.
	std::vector<std::shared_ptr<clsXAudio2WaveLoad>> pWaveData;
///////////////////////////////////////////////////////////////////////////
//	BGM関係.
	// BGMのインデックスリスト.
	std::map<std::string, int>  m_sBGMIndex;
	// BGMのSoundSourceを入れる.
	std::vector<std::shared_ptr<clsXAudio2PlaySound>> pBgmSource;
	// BGMのリスト.
	std::vector<std::string>	m_vsBGMNameList;

///////////////////////////////////////////////////////////////////////////
//	SE関係.

	// SEのインデックスリスト.
	std::map<std::string, int>  m_sSEIndex;
	// SEのリスト.
	std::vector<std::string>	m_vsSENameList;
	// SEのSoundSourceを入れる.
	std::vector<std::shared_ptr<clsXAudio2PlaySound>> pSeSource;
	// SEのリストを作る際、BGMで作ったWavDataの値から
	// forループを回すための変数.
	size_t CountWavDataSize;
	// ゲームを終了する際に立ち、TrueだとXAudio2関係の関数をリターンするようになる変数.
	bool	m_bEndGame;
	// 強制終了時にBGMを止める変数.
	bool	m_bEmergencyStop;
};



#endif // #ifndef C_SOUNDMANAGER_H
