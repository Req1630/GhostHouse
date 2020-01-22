#include "StageSelectUI.h"
#include "..\..\XAudio2\SoundManager.h"
#include "..\..\XAudio2\PlaySEThread.h"

CStageSelectUI::CStageSelectUI()
	: m_pStageSelectTitle	( nullptr )
	, m_pUpArrows			( nullptr )
	, m_pDownArrows			( nullptr )
	, m_pStageNameFrame		( nullptr )
	, m_fArrowSize			( ARROW_INIT_SIZE )
	, m_fSizeCount			( 0.0f )
	, m_iNowStageNumber		( 0 )
	, m_bPlaySE				( false )
{

}

CStageSelectUI::~CStageSelectUI()
{

}

bool CStageSelectUI::Load()
{
	// �摜���ǂݍ��߂Ă�����true��Ԃ�.
	if( LoadAllImage() == true ) return true;
	return false;
}

void CStageSelectUI::Render()
{
	// ���ׂẲ摜���ǂݍ��߂Ă��Ȃ�������I��.
	if( LoadAllImage() == false ) return;

	// �X�e�[�W�Z���N�g�^�C�g���摜�̕\��.
	m_pStageSelectTitle->Render();
	// ����摜�̕`��.
	UpArrowRender();
	// �����摜�̕`��.
	DownArrowRender();
	
	shared_ptr<CSpriteUI> pNowStage = CMapLoad::GetStageImage( m_iNowStageNumber );
	pNowStage->Render();
	
	// �X�e�[�W���̕\��.
	CDebugText::SetPosition( D3DXVECTOR3( 250.0f, 450.0f, 0.0f ) );
	CDebugText::Render( CMapLoad::GetStageName(m_iNowStageNumber) );

}

void CStageSelectUI::MoveUpStage()
{
	// �e�ړ��t���O�������Ă�����I��.
	if( m_bMoveUpStageFlag == true ) return;
	if( m_bMoveDownStageFlag == true ) return;

	// ��ړ��t���O�𗧂Ă�.
	m_bMoveUpStageFlag = true;
}

void CStageSelectUI::MoveDownStage()
{
	// �e�ړ��t���O�������Ă�����I��.
	if( m_bMoveDownStageFlag == true ) return;
	if( m_bMoveUpStageFlag == true ) return;

	// ���ړ��t���O�𗧂Ă�.
	m_bMoveDownStageFlag = true;
}

bool CStageSelectUI::SetStageName()
{
	// �e�ړ��t���O�������Ă�����I��.
	if( m_bMoveDownStageFlag == true ) return false;
	if( m_bMoveUpStageFlag == true ) return false;

	CMapLoad::SetStageName( CMapLoad::GetStageName(m_iNowStageNumber) );
	CMapLoad::SetStageNum( m_iNowStageNumber );

	return true;
}

void CStageSelectUI::UpArrowRender()
{
	// ��ړ��t���O�������Ă�����.
	if( m_bMoveUpStageFlag == true ){
		ArrowSizeChange();	// �摜�T�C�Y��ύX����.
		m_pUpArrows->SetScale( m_fArrowSize );		// �摜�T�C�Y���Z�b�g����.
	} else {
		m_pUpArrows->SetScale( ARROW_INIT_SIZE );	// ���T�C�Y���Z�b�g����.
	}
	// ����摜�̕`��.
	m_pUpArrows->Render();
}

void CStageSelectUI::DownArrowRender()
{
	// ���ړ��t���O�������Ă�����.
	if( m_bMoveDownStageFlag == true ){
		ArrowSizeChange();	// �摜�T�C�Y��ύX����.
		m_pDownArrows->SetScale( m_fArrowSize );	// �摜�T�C�Y���Z�b�g����.
	} else {
		m_pDownArrows->SetScale( ARROW_INIT_SIZE );	// ���T�C�Y���Z�b�g����.
	}
	// �����摜�̕`��.
	m_pDownArrows->Render();
}

void CStageSelectUI::ArrowSizeChange()
{
	m_fSizeCount += ARROW_SIZE_CHNGE_COUNT;
	float fsize = fabsf(cosf( D3DX_PI * 2.0f / 120.0f * m_fSizeCount ));
	if( fsize <= ARROW_SIZE_MINIMUM_VALUE ){
		if (m_bPlaySE == false) {
			m_bPlaySE = true;
			CPlaySEThread::SetSEName("Select");
		}
		fsize = ARROW_SIZE_MINIMUM_VALUE;
	}
	if( m_fSizeCount >= ARROW_SIZE_CHNGE_COUNT_MAX ){
		m_fSizeCount = 0.0f;
		m_bPlaySE = false;
		StageChange();
	}
	m_fArrowSize = fsize;
}

void CStageSelectUI::StageChange()
{
	if( m_bMoveUpStageFlag == true ){
		m_iNowStageNumber++;
		if( m_iNowStageNumber >= CMapLoad::GetMAXStageNumber() ){
			m_iNowStageNumber = 0;
		}
		m_bMoveUpStageFlag = false;

	} else if ( m_bMoveDownStageFlag == true ){
		m_iNowStageNumber--;
		if( m_iNowStageNumber < 0 ){
			m_iNowStageNumber = CMapLoad::GetMAXStageNumber()-1;
		}
		m_bMoveDownStageFlag = false;

	}
}



bool CStageSelectUI::LoadAllImage()
{
	if( LoadStageSelectTitle() == false ) return false;
	if( LoadUpArrow() == false ) return false;
	if( LoadDownArrow() == false ) return false;
	if( LoadStageNameFrame() == false ) return false;

	return true;
}

bool CStageSelectUI::LoadStageSelectTitle()
{
	// "�X�e�[�W�Z���N�g"�̃^�C�g���摜�̓ǂݍ���.
	if( m_pStageSelectTitle != nullptr ) return true;
	m_pStageSelectTitle = CSpriteResource::GetSprite("StageSelectTitle");
	return false;
}

bool CStageSelectUI::LoadUpArrow()
{
	// ����摜�̓ǂݍ���.
	if( m_pUpArrows != nullptr ) return true;
	m_pUpArrows = CSpriteResource::GetSprite("CursorUp");
	return false;
}

bool CStageSelectUI::LoadDownArrow()
{
	// �����摜�̓ǂݍ���.
	if( m_pDownArrows != nullptr ) return true;
	m_pDownArrows = CSpriteResource::GetSprite("CursorDown");
	return false;
}

bool CStageSelectUI::LoadStageNameFrame()
{
	// �X�e�[�W���g�摜�ǂݍ���.
	if( m_pStageNameFrame != nullptr ) return true;
	m_pStageNameFrame = CSpriteResource::GetSprite("StageNameFrame");
	return false;
}