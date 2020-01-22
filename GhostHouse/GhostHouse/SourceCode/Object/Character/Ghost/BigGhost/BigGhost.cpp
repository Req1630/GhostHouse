#include "BigGhost.h"
#include "..\..\Resource\MeshResource\MeshResource.h"

CBigGhost::CBigGhost( const stObjectInfo& objInfo )
	: m_pSkinMesh			( nullptr )
	, m_pCollisionBoxMesh	( nullptr )
	, m_bEndFlag			( false )
	, m_bAnimSpeed			( ANIM_SPEED )
{
	SetObjectInfo( objInfo );
	m_fWaitDistance = MAX_WAIT_DISTANCE;
	m_fCount		= MAX_ALPHA;
	m_fScale		= MODEL_SCALE;
}

CBigGhost::~CBigGhost()
{
}

// �X�V�֐�.
void CBigGhost::Update( shared_ptr<CObjectBase> pObj )
{
	if( m_pSkinMesh == nullptr )	return;
	if( m_pCollisionBoxMesh == nullptr ) return;

//	Action( pObj );	// ����.

	IdleState();	// �ҋ@.
}

// �`��֐�.
void CBigGhost::Render( D3DXMATRIX& mView, D3DXMATRIX& mProj,
	Light& stLight, stCAMERA& stCamera )
{
	if( m_pSkinMesh == nullptr )	return;
	if( m_pCollisionBoxMesh == nullptr ) return;
	if( m_bEndFlag == true )	return;

	m_bAnimSpeed = m_isGamePause == false ? ANIM_SPEED : 0.0;

	m_pSkinMesh->SetPosition( m_vPosition );	// ���W��`��p�ɃZ�b�g.
	m_pSkinMesh->SetRotation( m_vRotation );
	m_pSkinMesh->SetAnimSpeed( m_bAnimSpeed );
	m_pSkinMesh->SetScale( m_fScale );
	m_pSkinMesh->SetAlpha( FadeChange() );
	m_pSkinMesh->SetBlend( true );
	m_pSkinMesh->Render( mView, mProj, stLight, stCamera );	// �`��.
	m_pSkinMesh->SetBlend( false );
}

void CBigGhost::Collision( shared_ptr<CObjectBase> pObj )
{
}

void CBigGhost::Load( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 )
{
	if( m_pSkinMesh == nullptr ){
		m_pSkinMesh = CMeshResorce::GetSkin( MAIN_MODEL_NAME );
		if( m_pSkinMesh != nullptr ){
			m_pCollision->Init( m_pSkinMesh->GetMesh() );
			m_pSkinMesh->ChangeAnimSet(0);
		}
	}
	if( m_pCollisionBoxMesh == nullptr ){
		m_pCollisionBoxMesh = CMeshResorce::GetStatic( "BigGhostCollsion" );
	}
}
// ����֐�.
void CBigGhost::Action( shared_ptr<CObjectBase> pObj )
{
	if( GetAsyncKeyState( 'G' ) & 0x0001 ) m_bMoveFlag = true;

	if( m_bMoveFlag == false ) return;

	if( m_bAlphaFlag == false ){
		m_bAlphaFlag = true;
		m_fCount = MAX_ALPHA;
	}
	m_vPosition	+= AxisZProc( m_vRotation.y ) * MOVE_SPEED;
}

float CBigGhost::FadeChange()
{
	if( m_bAlphaFlag == true ){
		if( m_fCount <= 0.0f ){
			m_bAlphaFlag = false;
			m_bMoveFlag = false;
			m_bEndFlag = true;
		}
		m_fCount -= ATTENUATION_VALUE_ALPH;
	}
	return m_fCount;
}

LPD3DXMESH	CBigGhost::GetMeshData()
{
	if( m_pSkinMesh == nullptr ) return nullptr;
	if( m_pCollisionBoxMesh == nullptr ) return nullptr;
	return m_pCollisionBoxMesh->GetMesh();
}