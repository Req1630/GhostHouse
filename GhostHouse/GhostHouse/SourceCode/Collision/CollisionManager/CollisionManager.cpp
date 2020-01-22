#include "CollisionManager.h"

CCollisionManager::CCollisionManager( D3DXVECTOR3* pvPos, float* pfRot_y )
	: m_pCollBox	( make_shared<CCollisionBox>( pvPos ) )
	, m_pCollSphere	( make_shared<CCollisionSphere>( pvPos ) )
	, m_pCollRay	( make_shared<CCollisionRay>( pfRot_y ) )
{
}

CCollisionManager::CCollisionManager()
	: m_pCollBox	( nullptr )
	, m_pCollSphere	( nullptr )
	, m_pCollRay	( nullptr )
{
}

CCollisionManager::~CCollisionManager()
{
}

void CCollisionManager::Init( LPD3DXMESH pMesh )
{
	m_pCollBox->InitBBox( pMesh );
	m_pCollSphere->InitSphere( pMesh );
}

void CCollisionManager::InitDebug( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 )
{
	m_pCollBox->InitDebug( pDevice11, pContext11 );
	m_pCollSphere->InitDebug( pDevice11, pContext11 );
}

void CCollisionManager::DebugRender( D3DXMATRIX& mView, D3DXMATRIX& mProj )
{
	m_pCollBox->DebugRender( mView, mProj );
	m_pCollSphere->DebugRender( mView, mProj );
}