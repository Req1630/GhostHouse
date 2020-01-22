#include "PauseUI.h"
#include "..\..\XInput\XInput.h"
#include "..\..\XAudio2\PlaySEThread.h"

CPauseUI::CPauseUI()
	: m_pPauseTitleImage( nullptr )
	, m_pToTitleImage( nullptr )
	, m_pToGameImage( nullptr )
	, m_pCursor( nullptr )
	, m_vCursorPosition()
	, m_bPushDownKey( false )
	, m_bPushUpKey( false )
	, m_enPauseMenuState( enPauseMenuState::ToGame )
{
}

void CPauseUI::Update()
{
	// �X�e�B�b�N�����.
	if( CXInput::GetLThumbY() > STICK_IDLING ||
		( GetAsyncKeyState( VK_UP ) & 0x8000 ) ){
		if( m_bPushUpKey == false ){
			m_bPushUpKey = true;
			GiveCursorUP();
		}
	}
	// �X�e�B�b�N������.
	if( CXInput::GetLThumbY() < -STICK_IDLING ||
		( GetAsyncKeyState( VK_DOWN ) & 0x8000 ) ){
		if( m_bPushDownKey == false ){
			m_bPushDownKey = true;
			GiveCursorDOWN();
		}
	}

	if( CXInput::GetLThumbY() < STICK_IDLING )	m_bPushUpKey = false;
	if( CXInput::GetLThumbY() > -STICK_IDLING )	m_bPushDownKey = false;
}

// �`��֐�.
void CPauseUI::Render()
{
	if( LoadAllImage() == false ) return;

	// �e�摜�̕`��.
	m_pPauseTitleImage->Render();
	m_pToTitleImage->Render();
	m_pToGameImage->Render();
	m_pCursor->SetPosition( m_vCursorPosition[0] );
	m_pCursor->Render();
	m_pCursor->SetPosition( m_vCursorPosition[1] );
	m_pCursor->Render();
}

// ���ׂẲ摜.
bool CPauseUI::LoadAllImage()
{
	if( LoadPauseTitltImage() == false ) return false;
	if( LoadToTilteImage() == false ) return false;
	if( LoadToGameImage() == false ) return false;
	if( LoadCursorImage() == false ) return false;

	return true;
}

// �^�C�g���摜. 
bool CPauseUI::LoadPauseTitltImage()
{
	if( m_pPauseTitleImage != nullptr ) return true;
	m_pPauseTitleImage = CSpriteResource::GetSprite( "PauseTitle" );

	return false;
}

// "�^�C�g����"�e�L�X�g�摜.
bool CPauseUI::LoadToTilteImage()
{
	if( m_pToTitleImage != nullptr ) return true;
	m_pToTitleImage = CSpriteResource::GetSprite( "ToTitle" );

	if( m_pToTitleImage == nullptr ) return false;
	MAX_CURSOR_POSITION_Y = m_pToTitleImage->GetPosition().y;

	return false;
}

// "�Q�[����"�e�L�X�g�摜.
bool CPauseUI::LoadToGameImage()
{
	if( m_pToGameImage != nullptr ) return true;
	m_pToGameImage = CSpriteResource::GetSprite( "ToGame" );

	if( m_pToGameImage == nullptr ) return false;
	MIN_CURSOR_POSITION_Y = m_pToGameImage->GetPosition().y;

	return false;
}

// �I���J�[�\���̉摜.
bool CPauseUI::LoadCursorImage()
{
	// �ǂݍ��܂�Ă����� "true".
	if( m_pCursor != nullptr ) return true;
	m_pCursor = CSpriteResource::GetSprite( "Cursor" );

	if( m_pCursor == nullptr ) return false;
	m_vCursorPosition[0].z = m_pCursor->GetPosition().z;

	CURSOR_MOVE_SPEED =
		m_pToTitleImage->GetPosition().y - m_pToGameImage->GetPosition().y;

	m_vCursorPosition[0].y = m_vCursorPosition[1].y = m_pToGameImage->GetPosition().y;
	m_vCursorPosition[0].x =
		m_pToGameImage->GetPosition().x - m_pToGameImage->GetSpriteWidth() / 2 +
		m_pCursor->GetSpriteWidth() - 10;
	m_vCursorPosition[1].x =
		m_pToGameImage->GetPosition().x + m_pToGameImage->GetSpriteWidth() / 2 -
		m_pCursor->GetSpriteWidth() + 20;

	CURSOR_MOVE_SPEED =
		MAX_CURSOR_POSITION_Y - MIN_CURSOR_POSITION_Y;

	return false;
}

void CPauseUI::GiveCursorUP()
{
	if( m_pToGameImage == nullptr ) return;
	if( m_pCursor == nullptr ) return;

	// �E�X�e�B�b�N����ɌX�����Ƃ�.
	m_vCursorPosition[0].y -= CURSOR_MOVE_SPEED;	// �J�[�\���̈ړ�.
	if( m_vCursorPosition[0].y != MIN_CURSOR_POSITION_Y ){
		CPlaySEThread::SetSEName( "UnSelect" );
	} else{
		CPlaySEThread::SetSEName( "Select" );
	}
	// �ړ������悪�K��l��菭�Ȃ���������W�̒���.
	if( m_vCursorPosition[0].y <= MIN_CURSOR_POSITION_Y ){

		m_vCursorPosition[0].y = m_vCursorPosition[1].y = MIN_CURSOR_POSITION_Y;

		m_vCursorPosition[0].x =
			m_pToGameImage->GetPosition().x - m_pToGameImage->GetSpriteWidth() / 2 +
			m_pCursor->GetSpriteWidth() - 10;
		m_vCursorPosition[1].x =
			m_pToGameImage->GetPosition().x + m_pToGameImage->GetSpriteWidth() / 2 -
			m_pCursor->GetSpriteWidth() + 20;

		m_enPauseMenuState = enPauseMenuState::ToGame;
	}
}
void CPauseUI::GiveCursorDOWN()
{
	if( m_pToTitleImage == nullptr ) return;
	if( m_pCursor == nullptr ) return;

	m_vCursorPosition[0].y += CURSOR_MOVE_SPEED;	// �J�[�\���̈ړ�.
	if( m_vCursorPosition[0].y != MAX_CURSOR_POSITION_Y ){
		CPlaySEThread::SetSEName( "UnSelect" );
	} else{
		CPlaySEThread::SetSEName( "Select" );
	}
	// �ړ������悪�K��l���傫����������W�̒���.
	if( m_vCursorPosition[0].y >= MAX_CURSOR_POSITION_Y ){

		m_vCursorPosition[0].y = m_vCursorPosition[1].y = MAX_CURSOR_POSITION_Y;

		m_vCursorPosition[0].x =
			m_pToTitleImage->GetPosition().x - m_pToTitleImage->GetSpriteWidth() / 2 +
			m_pCursor->GetSpriteWidth() - 10;
		m_vCursorPosition[1].x =
			m_pToTitleImage->GetPosition().x + m_pToTitleImage->GetSpriteWidth() / 2 -
			m_pCursor->GetSpriteWidth() + 20;

		m_enPauseMenuState = enPauseMenuState::ToTitle;
	}
}