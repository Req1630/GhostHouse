#include "SceneManager.h"

clsSceneManager::clsSceneManager()
{
}
clsSceneManager::~clsSceneManager()
{
}

void clsSceneManager::Load( HWND hWnd, ID3D11Device* pDevice11,
	ID3D11DeviceContext* pContext11, LPDIRECT3DDEVICE9 pDevice9 )
{
	m_pStackScene.top()->Load( hWnd, pDevice11, pContext11, pDevice9 );
}

void clsSceneManager::LoadRender( D3DXMATRIX& mView, D3DXMATRIX& mProj,
	Light& stLight, stCAMERA& stCamera )
{
	m_pStackScene.top()->Render( mView, mProj, stLight, stCamera );
}

void clsSceneManager::Update()
{
	m_pStackScene.top()->Updata();
}

void clsSceneManager::Render( D3DXMATRIX& mView, D3DXMATRIX& mProj,
	Light& stLight, stCAMERA& stCamera )
{
	m_pStackScene.top()->Render( mView, mProj, stLight, stCamera );
}


void clsSceneManager::Push( shared_ptr<clsSceneBase> pScenebase )
{
	if( pScenebase == nullptr ) return;
	m_pStackScene.push( pScenebase );
}

void clsSceneManager::Pop()
{
	m_pStackScene.pop();
}
void clsSceneManager::TwoPop()
{
	m_pStackScene.pop();
	m_pStackScene.pop();
}

void clsSceneManager::Change( shared_ptr<clsSceneBase> pScenebase )
{
	m_pStackScene.pop();
	m_pStackScene.push( pScenebase );
}

void clsSceneManager::Swap( shared_ptr<clsSceneBase> pScenebase )
{
	stack<shared_ptr<clsSceneBase>> temp;
	temp.push( pScenebase );
	m_pStackScene.swap( temp );
}

void clsSceneManager::StackRelease()
{
	size_t size_max = m_pStackScene.size();
	for( size_t i = 0; i < size_max; i++  ){
		m_pStackScene.pop();
	}
}
