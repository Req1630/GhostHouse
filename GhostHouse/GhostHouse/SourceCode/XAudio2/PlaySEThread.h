#ifndef C_PLAYSE_THREAD_H
#define C_PLAYSE_THREAD_H
#include <process.h>
#include <thread>
#include <Windows.h>

class CPlaySEThread
{
public:
	CPlaySEThread();
	~CPlaySEThread();

	// �C���X�^���X�̎擾.
	static CPlaySEThread* GetInstance();

	// �X���b�h�𗧂��グ��֐�(���̂�).
	static void SetUpThread();
	// �A�b�v�f�[�g�֐�.
	static void Updata();
	// �A�b�v�f�[�g�֐��ŏ�ɉ񂷁ASE��炷���߂̊֐�.
	static void Sound();
	// �㏈��.
	static bool Release();
	// SE�̖��O���Z�b�g����֐�.
	static void SetSEName(std::string SEName) {
		GetInstance()->WaitInSEName = SEName;
	}

private:
	// ��񂾂��֐���ʂ�悤�ɂ���t���O.
	bool m_bFristCreate;



	std::string m_sOldSEName;	// �O��1�ڂ̃X���b�h�ōĐ�����SE�̖��O������string�^�ϐ�.
	std::string m_sOldSEName2;  // �O��2�ڂ̃X���b�h�ōĐ�����SE�̖��O������string�^�ϐ�.
	std::string WaitInSEName;	// �Đ����悤�Ƃ��Ă���SE�̖��O������string�^�ϐ�.

	std::string m_sSEName;		// 1�ڂ̃X���b�h�ōĐ�����SE�̖��O������string�^�ϐ�.
	std::string m_sSEName2;		// 2�ڂ̃X���b�h�ōĐ�����SE�̖��O������string�^�ϐ�.
	std::string m_sSEName3;		// 3�ڂ̃X���b�h�ōĐ�����SE�̖��O������string�^�ϐ�.

	bool	m_ChangeSE;				// �ҋ@���Ă���X���b�h�ɁA�V����SE�Đ����߂��������ۂ�true�ɂȂ�ϐ�.
	bool	m_bIsPlay;				// SE�Đ����̃X���b�h��true.
	std::thread SEThread;			// SE��炷�X���b�h.
	std::thread::id InSEThreadID;	// SE�X���b�hID�ޔ�.

	bool	m_ChangeSE2;			// �ҋ@���Ă���X���b�h�ɁA�V����SE�Đ����߂��������ۂ�true�ɂȂ�ϐ�.
	bool	m_bIsPlay2;				// SE�Đ����̃X���b�h��true.
	std::thread SEThread2;			// SE��炷�X���b�h.
	std::thread::id InSEThreadID2;	// SE�X���b�hID�ޔ�.

	bool	m_ChangeSE3;			// �ҋ@���Ă���X���b�h�ɁA�V����SE�Đ����߂��������ۂ�true�ɂȂ�ϐ�.
	bool	m_bIsPlay3;				// SE�Đ����̃X���b�h��true.
	std::thread SEThread3;			// SE��炷�X���b�h.
	std::thread::id InSEThreadID3;	// SE�X���b�hID�ޔ�.

	bool m_isEnd;					// �Q�[�����I�����悤�Ƃ��Ă�����true.

	bool m_bReplaySE;				// SE���͂��߂���炵�����Ƃ���true�ɂ���ϐ�.
	bool m_bReplaySE2;				// SE���͂��߂���炵�����Ƃ���true�ɂ���ϐ�.
	bool m_bReplaySE3;				// SE���͂��߂���炵�����Ƃ���true�ɂ���ϐ�.

	DWORD m_dThreadExitCode_1;		// �X���b�h1�̏������I�����āA�J���ł��邩�ǂ����̒l������DWORD�^�ϐ�.
	DWORD m_dThreadExitCode_2;		// �X���b�h2�̏������I�����āA�J���ł��邩�ǂ����̒l������DWORD�^�ϐ�.
	DWORD m_dThreadExitCode_3;		// �X���b�h3�̏������I�����āA�J���ł��邩�ǂ����̒l������DWORD�^�ϐ�.

	bool m_bReleaseOK_1;			// �X���b�h1�̊J�������S�ɂł��邩�ǂ����̃t���O.
	bool m_bReleaseOK_2;			// �X���b�h2�̊J�������S�ɂł��邩�ǂ����̃t���O.
	bool m_bReleaseOK_3;			// �X���b�h3�̊J�������S�ɂł��邩�ǂ����̃t���O.
private:
	// �R�s�[�E���[�u�R���X�g���N�^, ������Z�q�̍폜.
	CPlaySEThread(const CPlaySEThread &) = delete;
	CPlaySEThread& operator = (const CPlaySEThread &) = delete;
	CPlaySEThread(CPlaySEThread &&) = delete;
	CPlaySEThread& operator = (CPlaySEThread &&) = delete;
};

#endif // !C_PLAYSE_THREAD_H