#include "PlaySEThread.h"
#include "SoundManager.h"
CPlaySEThread::CPlaySEThread()
	: m_isEnd			( false )
	, m_ChangeSE		( false )
	, m_ChangeSE2		( false )
	, m_ChangeSE3		( false )
	, m_bIsPlay			( false )
	, m_bIsPlay2		( false )
	, m_bIsPlay3		( false )
	, m_bReplaySE		( false )
	, m_bReplaySE2		( false )
	, m_bReplaySE3		( false )
	, m_sSEName			("")
	, m_sSEName2		("")
	, m_sSEName3		("")
	, m_bFristCreate	( true )
{	 
	m_dThreadExitCode_1 = -1;
	m_dThreadExitCode_2 = -1;
	m_dThreadExitCode_3 = -1;
	m_bReleaseOK_1		= false;
	m_bReleaseOK_2		= false;
	m_bReleaseOK_3		= false;
}

CPlaySEThread::~CPlaySEThread()
{
}

void CPlaySEThread::SetUpThread()
{
	if (GetInstance()->m_bFristCreate == true) {
		auto SE1 = [&]()
		{
			CSoundManager::PlaySEByName(
				GetInstance()->m_sSEName, GetInstance()->m_isEnd, GetInstance()->m_ChangeSE,
				false, GetInstance()->m_bIsPlay, GetInstance()->m_bReplaySE);
		};
		GetInstance()->SEThread = std::thread(SE1);
		GetInstance()->InSEThreadID = GetInstance()->SEThread.get_id();
		auto SE2 = [&]()
		{
			CSoundManager::PlaySEByName(
				GetInstance()->m_sSEName2, GetInstance()->m_isEnd, GetInstance()->m_ChangeSE2,
				false, GetInstance()->m_bIsPlay2, GetInstance()->m_bReplaySE2);
		};
		GetInstance()->SEThread2 = std::thread(SE2);
		GetInstance()->InSEThreadID2 = GetInstance()->SEThread2.get_id();

		auto SE3 = [&]()
		{
			CSoundManager::PlaySEByName(
				GetInstance()->m_sSEName3, GetInstance()->m_isEnd, GetInstance()->m_ChangeSE3,
				false, GetInstance()->m_bIsPlay3, GetInstance()->m_bReplaySE3);
		};
		GetInstance()->SEThread3 = std::thread(SE3);
		GetInstance()->InSEThreadID3 = GetInstance()->SEThread3.get_id();
		

		GetInstance()->m_bFristCreate = false;
	}
}

void CPlaySEThread::Updata()
{
	Sound();
}

void CPlaySEThread::Sound()
{
	if (GetInstance()->WaitInSEName != "") {
		if (GetInstance()->m_bIsPlay == false) {
			GetInstance()->m_sSEName = GetInstance()->WaitInSEName;
			GetInstance()->m_sOldSEName = GetInstance()->m_sSEName;
			GetInstance()->m_ChangeSE = true;
		}
		else {
			if (GetInstance()->WaitInSEName == GetInstance()->m_sOldSEName)
			{
				GetInstance()->m_sSEName = GetInstance()->WaitInSEName;
				GetInstance()->m_ChangeSE = true;
				GetInstance()->m_bReplaySE = true;
			}
			else {
				if (GetInstance()->m_bIsPlay2 == false) {
					GetInstance()->m_sSEName2 = GetInstance()->WaitInSEName;
					GetInstance()->m_sOldSEName2 = GetInstance()->m_sSEName2;
					GetInstance()->m_ChangeSE2 = true;
				}
				else {
					if (GetInstance()->WaitInSEName == GetInstance()->m_sOldSEName2)
					{
						GetInstance()->m_sSEName2 = GetInstance()->WaitInSEName;
						GetInstance()->m_ChangeSE2 = true;
						GetInstance()->m_bReplaySE2 = true;
					}
					else {
						if (GetInstance()->m_bIsPlay3 == false)
						{
							GetInstance()->m_sSEName3 = GetInstance()->WaitInSEName;
							GetInstance()->m_ChangeSE3 = true;
							GetInstance()->m_bReplaySE3 = true;
						}
					}
				}
			}
		}
		GetInstance()->WaitInSEName = "";
		return;
	}
}

bool CPlaySEThread::Release()
{
	GetInstance()->m_isEnd = true;				//曲をストップした場合、ストップを命令したコードの中で
	while (GetInstance()->m_bReleaseOK_1 == false)
	{
		GetExitCodeThread(GetInstance()->SEThread.native_handle(), &GetInstance()->m_dThreadExitCode_1);
		if (GetInstance()->m_dThreadExitCode_1 == 0) {
			if (GetInstance()->InSEThreadID == GetInstance()->SEThread.get_id()) {
				CSoundManager::Clean();
															//falseの間ループで待たせているので、SoundManagerを解放してから
															//trueでスレッド内のwhileループからでる。
				GetInstance()->SEThread.detach();
				GetInstance()->m_bReleaseOK_1 = true;
			}

		}
		return false;
	}

	while (GetInstance()->m_bReleaseOK_2 == false)
	{
		GetExitCodeThread(GetInstance()->SEThread2.native_handle(), &GetInstance()->m_dThreadExitCode_2);
		if (GetInstance()->m_dThreadExitCode_2 == 0) {
			if (GetInstance()->InSEThreadID2 == GetInstance()->SEThread2.get_id()) {
				GetInstance()->SEThread2.detach();
				GetInstance()->m_bReleaseOK_2 = true;
			}
		}
		return false;
	}
	while (GetInstance()->m_bReleaseOK_3 == false)
	{
		GetExitCodeThread(GetInstance()->SEThread3.native_handle(), &GetInstance()->m_dThreadExitCode_3);
		if (GetInstance()->m_dThreadExitCode_3 == 0) {
			if (GetInstance()->InSEThreadID3 == GetInstance()->SEThread3.get_id()) {							
				GetInstance()->SEThread3.detach();
				GetInstance()->m_bReleaseOK_3 = true;
			}
		}
		return false;
	}
	return true;
}
