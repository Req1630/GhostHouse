#include "Command.h"
#include "..\XInput\XInput.h"
#include <Windows.h>

CCommand::CCommand()
	: m_TimeCount			( 0 )
	, m_CommandAdd			( 0 )
	, m_isTwoPush			( 0 )
	, m_isStart				( false )
	, m_isSuccessCommand	( false )
{
}

CCommand::~CCommand()
{
}

//----------------------------------.
// �������֐�.
//----------------------------------.
void CCommand::Initialize()
{
	GetInstance()->Init();
	GetInstance()->m_isSuccessCommand = false;
}

//----------------------------------.
// �������֐�.
//----------------------------------.
void CCommand::Init()
{
	m_TimeCount		= 0;
	m_CommandAdd	= 0;
	m_isTwoPush		= 0;
	m_isStart		= false;
}

//----------------------------------.
// �X�V�֐�.
//----------------------------------.
void CCommand::Update()
{
	GetInstance()->KeyInput();

	GetInstance()->TimeCount();

	GetInstance()->CheckCommand();
}

//----------------------------------.
// �R�}���h���͊֐�.
//----------------------------------.
void CCommand::KeyInput()
{
	auto CheckInit = []( const COMMAND::HashKey& command )
	{
		if( GetInstance()->m_CommandAdd > command ){
			GetInstance()->Init();
		}
	};

	if( ( GetAsyncKeyState( VK_UP ) & 0x0001 ) ||
		( CXInput::IsPress(XINPUT_GAMEPAD_DPAD_UP)) ){
		PushUpDownKey();
		CheckInit( COMMAND::Up2 );
	}
	if( ( GetAsyncKeyState( VK_DOWN ) & 0x0001 ) ||
		( CXInput::IsPress(XINPUT_GAMEPAD_DPAD_DOWN)) ){
		PushUpDownKey();
		CheckInit( COMMAND::Down2 );
	}
	if( ( GetAsyncKeyState( VK_LEFT ) & 0x0001 ) ||
		( CXInput::IsPress(XINPUT_GAMEPAD_DPAD_LEFT)) ){
		PushLeftRightKey( 1, 2 );
		CheckInit( COMMAND::Left2 );
	}
	if( ( GetAsyncKeyState( VK_RIGHT ) & 0x0001 ) ||
		( CXInput::IsPress(XINPUT_GAMEPAD_DPAD_RIGHT)) ){
		PushLeftRightKey( 2, 3 );
		CheckInit( COMMAND::Right2 );
	}
	if( ( GetAsyncKeyState('B') & 0x0001 ) ||
		( CXInput::IsPress(XINPUT_GAMEPAD_RIGHT_SHOULDER)) ){
		m_CommandAdd++;
		if( m_CommandAdd < COMMAND::B ){
			Init();
		}
	}
	if( ( GetAsyncKeyState('A') & 0x0001 ) ||
		( CXInput::IsPress( XINPUT_GAMEPAD_LEFT_SHOULDER )) ){
		m_CommandAdd++;
		if( m_CommandAdd < COMMAND::A ){
			Init();
		}
	}
}

//----------------------------------.
// �R�}���h�����m�F�֐�.
//----------------------------------.
void CCommand::CheckCommand()
{
	if( m_CommandAdd >= COMMAND::Max ){
		m_isSuccessCommand = true;
	}
}

//----------------------------------.
// �㉺�L�[���������ۂ̏���.
//----------------------------------.
void CCommand::PushUpDownKey()
{
	m_isStart = true;
	m_isTwoPush++;
	// ���ڂ������ꂽ��J�E���g.
	if( m_isTwoPush == 1 ){
		m_TimeCount = 0;
		m_CommandAdd++;
	// ���ڂ߂������ꂽ��J�E���g.
	} else if( m_isTwoPush == 2 ){
		m_TimeCount = 0;
		m_CommandAdd++;
		m_isTwoPush = 0;
	// �O��ڈȍ~�Ȃ�I��.
	} else{
		Init();
	}
}

//----------------------------------.
// ���E�L�[���������ۂ̏���.
//----------------------------------.
void CCommand::PushLeftRightKey( int cnt, int cnt2 )
{
	m_isStart = true;
	m_isTwoPush++;
	if( m_isTwoPush == cnt ){
		m_TimeCount = 0;
		m_CommandAdd++;
		m_isStart = true;
		if( cnt == 2 ){
			m_isTwoPush = 0;
		}
	} else if( m_isTwoPush == cnt2 ){
		Init();
	}
}

//----------------------------------.
// �{�^�����������ۂ̃J�E���g����.
//----------------------------------.
void CCommand::TimeCount()
{
	if( m_isStart == false ) return;
	m_TimeCount++;

	if( m_TimeCount <= 60 ) return;
	Init();
}