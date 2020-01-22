#include "FadeUI.h"

CFadeUI::CFadeUI()
	: m_pFadeSprite	( nullptr )
	, m_bFadeIN		( false )
	, m_bFadeOUT	( false )
	, m_fAlpha		( MAX_ALPHA )
	, m_isRender	( true )
	, m_isEnd		( false )
{}

CFadeUI::~CFadeUI()
{}

void CFadeUI::Init( bool isrender )
{
	GetInstance()->m_bFadeIN	= false;
	GetInstance()->m_bFadeOUT	= false;
	GetInstance()->m_isRender	= isrender;
	GetInstance()->m_fAlpha		= isrender == true ? 1.0f : 0.0f;
	GetInstance()->m_isEnd		= false;
	if( GetInstance()->m_pFadeSprite  == nullptr ) return;
	GetInstance()->m_pFadeSprite->SetAlpha( GetInstance()->m_fAlpha );
}

void CFadeUI::Update()
{	
	if( m_bFadeOUT == true ){
		m_isRender = true;
		m_fAlpha += 0.02f;
		if( m_fAlpha > 1.0f ){
			m_bFadeOUT	= false;
			m_isRender	= true;
			m_isEnd		= true;
		}
	} else if( m_bFadeIN == true ){
		m_isRender = true;
		m_fAlpha -= 0.02f;
		if( m_fAlpha < 0.0f ){
			m_isRender = false;
			m_bFadeIN = false;
		}
	}
}

void CFadeUI::Render()
{
	GetInstance()->Load();
	GetInstance()->Update();
	if( GetInstance()->m_pFadeSprite == nullptr ) return;
	if( GetInstance()->m_isRender == false ) return;

	GetInstance()->m_pFadeSprite->SetAlpha( GetInstance()->m_fAlpha );
	GetInstance()->m_pFadeSprite->Render();
}

void CFadeUI::Load()
{
	if( m_pFadeSprite != nullptr ) return;
	m_pFadeSprite = CSpriteResource::GetSprite("Fade");
}

void CFadeUI::FadeINStart()
{
	GetInstance()->m_bFadeIN	= true;
	GetInstance()->m_bFadeOUT	= false;
}

void CFadeUI::FadeOUTStart()
{
	GetInstance()->m_bFadeOUT	= true;
	GetInstance()->m_bFadeIN	= false;
}