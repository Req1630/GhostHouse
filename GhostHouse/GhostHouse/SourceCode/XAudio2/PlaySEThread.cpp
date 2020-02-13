#include "PlaySEThread.h"
#include "SoundManager.h"
CPlaySEThread::CPlaySEThread()
	: m_isEnd(false)
	, m_ChangeSE(false)
	, m_ChangeSE2(false)
	, m_ChangeSE3(false)
	, m_bIsPlay(false)
	, m_bIsPlay2(false)
	, m_bIsPlay3(false)
	, m_bReplaySE(false)
	, m_bReplaySE2(false)
	, m_bReplaySE3(false)
	, m_sSEName("")
	, m_sSEName2("")
	, m_sSEName3("")
	, m_bFristCreate(true)
{
	m_dThreadExitCode_1 = -1;
	m_dThreadExitCode_2 = -1;
	m_dThreadExitCode_3 = -1;
	m_bReleaseOK_1 = false;
	m_bReleaseOK_2 = false;
	m_bReleaseOK_3 = false;
}

CPlaySEThread::~CPlaySEThread()
{

}
// �C���X�^���X�̍쐬.
CPlaySEThread* CPlaySEThread::GetInstance()
{
	static std::unique_ptr<CPlaySEThread> pInstance =
		std::make_unique<CPlaySEThread>();	// �C���X�^���X�̍쐬.
	return pInstance.get();
}

void CPlaySEThread::SetUpThread()
{
	// 1�񂾂��ʂ�.
	if (GetInstance()->m_bFristCreate == true) {
		// �����_��SE���Đ����邽�߂̊֐�������.
		auto SE1 = [&]()
		{
			CSoundManager::PlaySE(
				GetInstance()->m_sSEName, GetInstance()->m_isEnd, GetInstance()->m_ChangeSE,
					GetInstance()->m_bIsPlay, GetInstance()->m_bReplaySE);
		};
		GetInstance()->SEThread = std::thread(SE1);						// SE�Đ��p�X���b�h1�쐬.
		GetInstance()->InSEThreadID = GetInstance()->SEThread.get_id();	// �X���b�h1�̃X���b�hID�ޔ�.
		auto SE2 = [&]()
		{
			CSoundManager::PlaySE(
				GetInstance()->m_sSEName2, GetInstance()->m_isEnd, GetInstance()->m_ChangeSE2,
					GetInstance()->m_bIsPlay2, GetInstance()->m_bReplaySE2);
		};
		GetInstance()->SEThread2 = std::thread(SE2);						// SE�Đ��p�X���b�h2�쐬.
		GetInstance()->InSEThreadID2 = GetInstance()->SEThread2.get_id();	// �X���b�h2�̃X���b�hID�ޔ�.
		auto SE3 = [&]()
		{
			CSoundManager::PlaySE(
				GetInstance()->m_sSEName3, GetInstance()->m_isEnd, GetInstance()->m_ChangeSE3,
					GetInstance()->m_bIsPlay3, GetInstance()->m_bReplaySE3);
		};
		GetInstance()->SEThread3 = std::thread(SE3);						// SE�Đ��p�X���b�h3�쐬.
		GetInstance()->InSEThreadID3 = GetInstance()->SEThread3.get_id();	// �X���b�h3�̃X���b�hID�ޔ�.

		// 1�񂾂��ĂԂ��߂̃t���O������.
		GetInstance()->m_bFristCreate = false;
	}
}

void CPlaySEThread::Updata()
{
	Sound();
}
// �X���b�h�ŉ񂵂Ă���֐��̈����ɃZ�b�g����Ă���ϐ���ύX���āA����炳����֐�.
void CPlaySEThread::Sound()
{
	// �ϐ��ɖ��O������Β��ɓ���.
	if (GetInstance()->WaitInSEName != "") {
		// 1�߂̃X���b�h�������Đ����Ă��Ȃ���Β��ɓ���.
		if (GetInstance()->m_bIsPlay == false) {
			GetInstance()->m_sSEName = GetInstance()->WaitInSEName;	// PlaySE�̈����ɂȂ��Ă���ϐ��ɖ��O������.
			GetInstance()->m_sOldSEName = GetInstance()->m_sSEName;	// ���O���ߋ��Đ��������O������ϐ��ɓ����.
			GetInstance()->m_ChangeSE = true;						// PlaySE�̈����ɂȂ��Ă���ϐ���true�ɂ���.
		}
		else {
			// 1�߂̃X���b�h���Đ����Ă��邪�A�O�ɍĐ�����SE�Ɩ��O�������ꍇ���ɓ���.
			if (GetInstance()->WaitInSEName == GetInstance()->m_sOldSEName)
			{
				GetInstance()->m_sSEName = GetInstance()->WaitInSEName;	// PlaySE�̈����ɂȂ��Ă���ϐ��ɖ��O������.
				GetInstance()->m_ChangeSE = true;						// PlaySE�̈����ɂȂ��Ă���ϐ���true�ɂ���.
				GetInstance()->m_bReplaySE = true;						// PlaySE�̈����ɂȂ��Ă���ϐ���true�ɂ���.
			}
			else {
				// 2�߂̃X���b�h�������Đ����Ă��Ȃ���Β��ɓ���.
				if (GetInstance()->m_bIsPlay2 == false) {
					GetInstance()->m_sSEName2 = GetInstance()->WaitInSEName;
					GetInstance()->m_sOldSEName2 = GetInstance()->m_sSEName2;
					GetInstance()->m_ChangeSE2 = true;
				}
				else {
					// 2�߂̃X���b�h���Đ����Ă��邪�A�O�ɍĐ�����SE�Ɩ��O�������ꍇ���ɓ���.
					if (GetInstance()->WaitInSEName == GetInstance()->m_sOldSEName2)
					{
						GetInstance()->m_sSEName2 = GetInstance()->WaitInSEName;
						GetInstance()->m_ChangeSE2 = true;
						GetInstance()->m_bReplaySE2 = true;
					}
					else {
						// 3�߂̃X���b�h�������Đ����Ă��Ȃ���Β��ɓ���.
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
		// �Đ����悤�Ƃ��Ă���SE�̖��O������ϐ���������.
		GetInstance()->WaitInSEName = "";
		return;
	}
}
// �X���b�h�J�������֐�.
// ���̊֐���while�ŁA���̊֐���true��Ԃ��܂ŉ�.
bool CPlaySEThread::Release()
{
	GetInstance()->m_isEnd = true;
	// �X���b�h1���܂��J������Ă��Ȃ��ꍇ���ɓ���.
	while (GetInstance()->m_bReleaseOK_1 == false)
	{

		GetExitCodeThread(GetInstance()->SEThread.native_handle(), &GetInstance()->m_dThreadExitCode_1);
		// �X���b�h1����A���Ă���handle��0��������A���̃X���b�h�͈��S�ɊJ���ł���̂Œ��ɓ���.
		if (GetInstance()->m_dThreadExitCode_1 == 0) {
			// �X���b�h���쐬�����ۂɑޔ�����ID�ƌ��݂�ID�������Ȃ璆�ɓ���.
			if (GetInstance()->InSEThreadID == GetInstance()->SEThread.get_id()) {
				GetInstance()->SEThread.detach();		// �X���b�h���f�^�b�`.
				GetInstance()->m_bReleaseOK_1 = true;	// �X���b�h1�̊J�����I��������Ƃ�����.
			}

		}
		return false;	// �J�����܂��ł��Ȃ����false��������.
	}
	// �X���b�h2���܂��J������Ă��Ȃ��ꍇ���ɓ���.
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
	// �X���b�h3���܂��J������Ă��Ȃ��ꍇ���ɓ���.
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
	return true;	// �S�ẴX���b�h�̊J�����I������̂�true��Ԃ�.
}
