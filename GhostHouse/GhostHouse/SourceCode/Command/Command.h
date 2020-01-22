#ifndef COMMAND_H
#define COMMAND_H

#include "..\SingletonBase\SingletonBase.h"

// �R�i�~�R�}���h.
namespace COMMAND{
	enum HashKey
	{
		None,

		Up1,	
		Up2,	
		Down1,	
		Down2,	
		Left1,	
		Right1,	
		Left2,	
		Right2,	
		B,		
		A,		

		// �Ō��'A'���Ō�Ƃ��Ďg�p.
		Max = A,
	};
};

class CCommand : public std::CSingletonBase<CCommand>
{
public:
	CCommand();
	~CCommand();

	// �X�V�֐�.
	static void Update();
	// �������֐� : �^�C�g���V�[���̃R���X�g���N�^�Ŏg�p.
	static void Initialize();
	// �������ǂ����擾.
	static bool GetSuccessCommand(){ return GetInstance()->m_isSuccessCommand; };

private:
	// �������֐�.
	void Init();
	// �R�}���h���͊֐�.
	void KeyInput();
	// �R�}���h�����m�F�֐�.
	void CheckCommand();
	// �㉺�L�[���������ۂ̏���.
	void PushUpDownKey();
	// ���E�L�[���������ۂ̏���.
	void PushLeftRightKey( int cnt, int cnt2 );
	// �{�^�����������ۂ̃J�E���g����.
	void TimeCount();

private:
	int		m_CommandAdd;
	int		m_TimeCount;
	bool	m_isStart;
	int		m_isTwoPush;

	bool m_isSuccessCommand;

};

#endif	// #ifndef COMMAND_H.