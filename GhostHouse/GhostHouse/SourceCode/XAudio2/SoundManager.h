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
	// BGMのリストをフォルダから作る.
	static void GetBGMList();
	// リストに名前が入っていれば、そのBGMのindex番号取得関数.
	static int GetBGMIndex(std::string Name);
	// BGM再生.
	static void PlayBGM(int index, bool& isEnd, bool LoopFlag);
	// BGMを再生する関数をBGMの名前から呼び出す関数.
	static void PlayBGMByName(std::string Name, bool& isEnd, bool LoopFlag);
	// BGM停止.
	static void StopBGM(int index);
	// BGMを停止する関数をBGMの名前から呼び出す関数.
	static void StopBGMByName(std::string Name);

	static bool FadeOutBGM(int index);

	static void FadeOutBGMByName(std::string Name);

	static bool FadeInBGM(int index);

	static void FadeInBGMByName(std::string Name);

	static void PauseBGM(int index);

	static void PauseBGMByName(std::string Name);

	static float GetBGMVolme(std::string Name);

	static void SetBGMVolme(std::string Name, float Volme);
//========================================================================================
//	SE
//====
	static void GetSEList();

	static int GetSEIndex(std::string Name);

	static void PlaySE(int index, bool & isEnd, bool& ChangeSE, bool LoopFlag);

	static void PlaySEByName(std::string& Name, bool & isEnd, bool & ChangeSE, bool LoopFlag,bool& IsPlay,bool& RePlay);

	static void StopSE(int index);

	static void StopSEByName(std::string Name);

	static void PauseSE(int index);

	static void PauseSEByName(std::string Name);

	static void InitALL();

	static void InitSource();

	static void CreateBGMSource();

	static void CreateSESource();

	static void CleanBGM(std::string Name);
	static void Clean();

	static void Release();

	static void SetEmergencyStopFlag(bool StopFlag) { GetInstance()->m_bEmergencyStop = StopFlag; }

private:
	bool m_bFristCall;

	int Clamp(int Num, int Min, int Max);

	std::map<std::string, int>  m_sBGMIndex;
	std::vector<std::shared_ptr<clsXAudio2WaveLoad>> pWaveData;
	std::vector<std::shared_ptr<clsXAudio2PlaySound>> pBgmSource;

	// BGMのリスト.
	std::vector<std::string>	m_vsBGMNameList;


	std::map<std::string, int>  m_sSEIndex;
	// SEのリスト.
	std::vector<std::string>	m_vsSENameList;
	std::vector<std::shared_ptr<clsXAudio2PlaySound>> pSeSource;

	size_t CountWavDataSize;

	bool	m_bEndGame;
	bool	m_bEmergencyStop;
};



#endif // #ifndef C_SOUNDMANAGER_H
