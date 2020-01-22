#include "InvisibleStartFloor.h"
#include "..\..\Resource\MeshResource\MeshResource.h"

CInvisibleStartFloor::CInvisibleStartFloor()
{
	m_vPosition = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_vRotation = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
}

CInvisibleStartFloor::CInvisibleStartFloor( const stObjectInfo& objInfo )
	: m_State( 0 )
	, m_isNowHit( false )
	, m_isOlsHit( false )
	, m_pDebugSphere( make_unique<CSphere>() )
{
	SetObjectInfo( objInfo );
	m_fScale = MODEL_SCALE;
}

CInvisibleStartFloor::~CInvisibleStartFloor()
{
}

void CInvisibleStartFloor::Update( shared_ptr<CObjectBase> pObj )
{
	if( m_pStaticMesh == nullptr )	return;

	Action( pObj );
}

void CInvisibleStartFloor::Render( D3DXMATRIX & mView, D3DXMATRIX & mProj, Light& stLight, stCAMERA& stCamera )
{
	if( m_pStaticMesh == nullptr )	return;
	if( m_bMoveFlag == true )	return;

	m_pStaticMesh->SetPosition( m_vPosition );	// 座標を描画用にセット.
	m_pStaticMesh->SetRotation( m_vRotation );
	m_pStaticMesh->SetAlpha( MODEL_ALPHA );
	m_pStaticMesh->SetScale( MODEL_SCALE );
	m_pStaticMesh->SetBlend( true );
	m_pStaticMesh->Render( mView, mProj, stLight, stCamera );	// 描画.
	m_pStaticMesh->SetBlend( false );

	m_pDebugSphere->SetPosition( m_vPosition );
	m_pDebugSphere->Render( mView, mProj );
}

void CInvisibleStartFloor::Collision( shared_ptr<CObjectBase> pObj )
{
	if( pObj == nullptr ) return;
	if( pObj->GetObjectNo() == enObjectNo::Towa ) return;

	switch( m_State ){
		case 0:
			if( pObj->m_CameraState == 1 ){
				m_State++;
			}
			if( m_pCollision->isSphereCollision( pObj->GetSphere() ) == true ){
				pObj->BitFlagON( BitFlag::isHitSignboard );
				pObj->BitFlagON( BitFlag::isCameraMoveUp );
				m_State++;
				pObj->m_CameraState++;
			}
			break;
		case 1:
			if( m_pCollision->isSphereCollision( pObj->GetSphere() ) == false ){
				if( pObj->m_CameraState == 1 ){
					m_State++;
				}
			}
			break;
		case 2:
			if( pObj->m_CameraState == 2 ){
				m_State++;
			}
			if( m_pCollision->isSphereCollision( pObj->GetSphere() ) == true ){
				pObj->BitFlagOFF( BitFlag::isHitSignboard );
				pObj->BitFlagOFF( BitFlag::isCameraMoveUp );
				m_State++;
				pObj->m_CameraState++;
			}
			break;
		case 3:
			if( m_pCollision->isSphereCollision( pObj->GetSphere() ) == false ){
				m_State = 0;
				pObj->m_CameraState = 0;
			}
			break;
		default:
			break;
	}
}

void CInvisibleStartFloor::Load( ID3D11Device * pDevice11, ID3D11DeviceContext * pContext11 )
{
	if( m_pStaticMesh == nullptr ){
		m_pStaticMesh = CMeshResorce::GetStatic( MAIN_MODEL_NAME );
		if( m_pStaticMesh != nullptr ){
			m_pCollision->Init( m_pStaticMesh->GetMesh() );
			m_pCollision->GetSphere()->SetRadius( COLLISION_RADIUS );
			m_pDebugSphere->Init( pDevice11, pContext11, COLLISION_RADIUS );
		}
	}
}


void CInvisibleStartFloor::Action( shared_ptr<CObjectBase> pObj )
{
}

LPD3DXMESH CInvisibleStartFloor::GetMeshData()
{
	if( m_pStaticMesh == nullptr ) return nullptr;
	return m_pStaticMesh->GetMesh();
}