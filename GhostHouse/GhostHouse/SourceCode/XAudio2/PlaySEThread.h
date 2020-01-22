#ifndef C_PLAYSE_THREAD_H
#define C_PLAYSE_THREAD_H
#include <process.h>
#include <thread>
#include <Windows.h>
#include "..\SingletonBase\SingletonBase.h"

class CPlaySEThread : public std::CSingletonBase<CPlaySEThread>
{
public:
	CPlaySEThread();
	~CPlaySEThread();

	static void SetUpThread();

	static void Updata();

	static void Sound();

	static bool Release();

	static void SetSEName(std::string SEName) {
		GetInstance()->WaitInSEName = SEName;
	}

private:
	bool m_bFristCreate;



	std::string m_sOldSEName;
	std::string m_sOldSEName2;
	std::string WaitInSEName;

	std::string m_sSEName;
	std::string m_sSEName2;
	std::string m_sSEName3;

	bool	m_ChangeSE;
	bool	m_bIsPlay;
	std::thread SEThread;		//BGMを鳴らすスレッド.
	std::thread::id InSEThreadID;	//BGMスレッドID退避.

	bool	m_ChangeSE2;
	bool	m_bIsPlay2;
	std::thread SEThread2;		//BGMを鳴らすスレッド.
	std::thread::id InSEThreadID2;	//BGMスレッドID退避.

	bool	m_ChangeSE3;
	bool	m_bIsPlay3;
	std::thread SEThread3;		//BGMを鳴らすスレッド.
	std::thread::id InSEThreadID3;	//BGMスレッドID退避.

	bool m_isEnd;

	bool m_bReplaySE;
	bool m_bReplaySE2;
	bool m_bReplaySE3;

	DWORD m_dThreadExitCode_1;
	DWORD m_dThreadExitCode_2;
	DWORD m_dThreadExitCode_3;

	bool m_bReleaseOK_1;
	bool m_bReleaseOK_2;
	bool m_bReleaseOK_3;
};









#endif // !C_PLAYSE_THREAD_H


