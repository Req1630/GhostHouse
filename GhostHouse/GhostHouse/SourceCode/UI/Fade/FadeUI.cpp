#include "FadeUI.h"

CFadeUI::CFadeUI()
	: m_pFadeSprite	( nullptr )
	, m_bFadeIN		( false )
	, m_bFadeOUT	( false )
	, m_ImageAlpha	( MAX_ALPHA )
	, m_isRender	( true )
	, m_isFadeEnd	( false )
{}

CFadeUI::~CFadeUI()
{}

//---------------------------------.
//	�������p�֐�.
//---------------------------------.
void CFadeUI::Init( bool isrender )
{
	GetInstance()->m_bFadeIN	= false;
	GetInstance()->m_bFadeOUT	= false;
	GetInstance()->m_isRender	= isrender;
	GetInstance()->m_ImageAlpha	= isrender == true ? 
		GetInstance()->MAX_ALPHA : GetInstance()->MIN_ALPHA;
	GetInstance()->m_isFadeEnd	= false;
	if( GetInstance()->m_pFadeSprite  == nullptr ) return;
	GetInstance()->m_pFadeSprite->SetAlpha( GetInstance()->m_ImageAlpha );
}

//---------------------------------.
//	�X�V�֐�(Render�ɓ���).
//---------------------------------.
void CFadeUI::Update()
{	
	if( m_bFadeOUT == true ){
		m_isRender = true;
		m_ImageAlpha += FADE_VALUE;
		if( m_ImageAlpha > MAX_ALPHA ){
			m_bFadeOUT	= false;
			m_isRender	= true;
			m_isFadeEnd	= true;
		}
	} else if( m_bFadeIN == true ){
		m_isRender = true;
		m_ImageAlpha -= FADE_VALUE;
		if( m_ImageAlpha < MIN_ALPHA ){
			m_isRender = false;
			m_bFadeIN = false;
		}
	}
}

//---------------------------------.
//	�`�揈���֐�.
//---------------------------------.
void CFadeUI::Render()
{
	GetInstance()->Load();
	GetInstance()->Update();
	if( GetInstance()->m_pFadeSprite == nullptr ) return;
	if( GetInstance()->m_isRender == false ) return;

	GetInstance()->m_pFadeSprite->SetAlpha( GetInstance()->m_ImageAlpha );
	GetInstance()->m_pFadeSprite->Render();
}

//---------------------------------.
//	�Ǎ��֐�.
//---------------------------------.
void CFadeUI::Load()
{
	if( m_pFadeSprite != nullptr ) return;
	m_pFadeSprite = CSpriteResource::GetSprite("Fade");
}

//---------------------------------.
//	�t�F�[�h�C���J�n�֐�.
//---------------------------------.
void CFadeUI::FadeINStart()
{
	GetInstance()->m_bFadeIN	= true;
	GetInstance()->m_bFadeOUT	= false;
}

//---------------------------------.
//	�t�F�[�h�A�E�g�J�n�֐�.
//---------------------------------.
void CFadeUI::FadeOUTStart()
{
	GetInstance()->m_bFadeOUT	= true;
	GetInstance()->m_bFadeIN	= false;
}