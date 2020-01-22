#include "PitfallLeft.h"
#include "..\..\Resource\MeshResource\MeshResource.h"


CPitfallLeft::CPitfallLeft( stObjectInfo objInfo )
	: m_iCount	( 0 )
	, m_enState	( enPitfallSatate::CLOSE )
{
	SetObjectInfo( objInfo );
	m_fScale = MODEL_SCALE;
}

CPitfallLeft::~CPitfallLeft()
{
}

void CPitfallLeft::Update( shared_ptr<CObjectBase> pObj )
{
	// �X�^�e�B�b�N���b�V����nullptr��������I��.
	if( m_pStaticMesh == nullptr )	return;

	// ���쏈��.
	Action( pObj );
}

void CPitfallLeft::Render( D3DXMATRIX & mView, D3DXMATRIX & mProj, Light& stLight, stCAMERA& stCamera )
{
	// �X�^�e�B�b�N���b�V����nullptr��������I��.
	if( m_pStaticMesh == nullptr )	return;

	m_pStaticMesh->SetPosition( m_vPosition );	// ���W��`��p�ɃZ�b�g.
	m_pStaticMesh->SetRotation( m_vRotation );
	m_pStaticMesh->SetAlpha( 1.0f );
	m_pStaticMesh->SetBlend( true );
	m_pStaticMesh->Render( mView, mProj, stLight, stCamera );	// �`��.
	m_pStaticMesh->SetBlend( false );


#ifdef _DEBUG
	m_pCollision->DebugRender( mView, mProj );
#endif	// #ifndef _DEBUG.
}

void CPitfallLeft::Collision( shared_ptr<CObjectBase> pObj )
{
}

void CPitfallLeft::Load( ID3D11Device * pDevice11, ID3D11DeviceContext * pContext11 )
{
	if( m_pStaticMesh == nullptr ){
		m_pStaticMesh = CMeshResorce::GetStatic( "Pitfall" );
		if( m_pStaticMesh != nullptr ){
			m_pCollision->Init( m_pStaticMesh->GetMesh() );
		}
	}
#ifdef _DEBUG
	if( m_pStaticMesh == nullptr )	return;

	static bool onLoop = false;
	if( onLoop == false ){
		m_pCollision->InitDebug( pDevice11, pContext11 );
		onLoop = true;
	}
#endif	// #ifndef _DEBUG.
}


void CPitfallLeft::Action( shared_ptr<CObjectBase> pObj )
{
	switch( m_enState )
	{
	case enPitfallSatate::NOW_OPEN:
		m_vRotation.z = D3DXToRadian( m_iCount );
		m_iCount += MOVE_SPEED;
		if( m_iCount >= MAX_ANGLE ){
			m_enState = enPitfallSatate::OPEN;
			m_iCount = 0;
		}
		break;
	case enPitfallSatate::OPEN:
		m_vRotation.z = D3DXToRadian( MAX_ANGLE );
		m_iCount++;
		if( m_iCount >= MAX_WAIT_TIME ){
			m_enState = enPitfallSatate::NOW_CLOSE;
			m_iCount = MAX_ANGLE;
		}
		break;
	case enPitfallSatate::NOW_CLOSE:
		m_vRotation.z = D3DXToRadian( m_iCount );
		m_iCount--;
		if( m_iCount <= 0 ){
			m_enState = enPitfallSatate::CLOSE;
			m_iCount = 0;
		}
		break;
	case enPitfallSatate::CLOSE:
		m_vRotation.z = D3DXToRadian( 0 );
		m_iCount++;
		if( m_iCount >= MAX_WAIT_TIME / 2 ){
			m_vPosition.y = sinf( static_cast<float>( m_iCount ) ) * 0.01f;
		}
		if( m_iCount >= MAX_WAIT_TIME ){
			m_enState = enPitfallSatate::NOW_OPEN;
			m_iCount = 0;
		}
		break;
	}
}
