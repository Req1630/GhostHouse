#include "KeyDoor.h"
#include "..\..\..\XAudio2\PlaySEThread.h"
#include "..\..\..\XInput\XInput.h"
#include "..\..\..\UI\Tutorial\TutorialText.h"
#include "..\..\Resource\MeshResource\MeshResource.h"

CKeyDoor::CKeyDoor( const stObjectInfo& objInfo )
	: m_fCount			( 0.0f )
	, m_bFadeInFlag		( true )
	, m_bPlayNoKeySE	( false )
{
	SetObjectInfo( objInfo );
	m_fScale = MODEL_SCALE;
}

CKeyDoor::~CKeyDoor()
{
}

void CKeyDoor::Update( shared_ptr<CObjectBase> pObj )
{
	if( pObj->GetObjectNo() == enObjectNo::Towa ) return;

	if( m_pStaticMesh == nullptr )	return;

	if( m_bMoveFlag == true ){
		FadeInOut();
		// �t�F�[�h�C�����Ă����Ԃŉ�]������.
		if( m_fCount >= 1.0f ){
			m_pStaticMesh = CMeshResorce::GetStatic("OpenDoor");
		}
	}

	Action( pObj );
}

void CKeyDoor::Render( D3DXMATRIX & mView, D3DXMATRIX & mProj, Light& stLight, stCAMERA& stCamera )
{
	if( m_pStaticMesh == nullptr )	return;
	if( m_fAlpha <= 0.8f ){
		m_fAlpha = 1.0f;
		if( m_bMoveFlag == true ){
			if( m_fCount >= 0.0f ){
				CSpriteResource::GetSprite( "Fade" )->SetAlpha( m_fCount );
			}
		}
		return;
	}

	m_pStaticMesh->SetPosition( m_vPosition );	// ���W��`��p�ɃZ�b�g.
	m_pStaticMesh->SetRotation( m_vRotation );
	m_pStaticMesh->SetAlpha( 1.0f );
	m_pStaticMesh->SetScale( MODEL_SCALE );
	m_pStaticMesh->SetBlend( true );
	m_pStaticMesh->Render( mView, mProj, stLight, stCamera );	// �`��.
	m_pStaticMesh->SetBlend( false );


	// �ړ��t���O������Ă�����I��.
	if( m_bMoveFlag == false ) return;
	// �J�E���g��0���Ⴉ������I��.
	if( m_fCount < 0.0f ) return;
	CSpriteResource::GetSprite("Fade")->SetAlpha( m_fCount );
}

void CKeyDoor::Collision( shared_ptr<CObjectBase> pObj )
{
}

void CKeyDoor::Load( ID3D11Device * pDevice11, ID3D11DeviceContext * pContext11 )
{
	if( m_pStaticMesh == nullptr ){
		m_pStaticMesh = CMeshResorce::GetStatic( MAIN_MODEL_NAME );
		if( m_pStaticMesh != nullptr ){
			m_pCollision->Init( m_pStaticMesh->GetMesh() );
			m_pCollision->GetSphere()->SetRadius( SPHERE_COLLISION_RADIUS );
		}
	}
}

void CKeyDoor::Action( shared_ptr<CObjectBase> pObj )
{
	// ���ɓ����Ă�����I��.
	if( m_bMoveFlag == true ) return;

	// �A�N�V�����{�^���t���O������.

	// �X�t�B�A�̓����蔻��ɂ������Ă��Ȃ���ΏI��.
	if( m_pCollision->isSphereCollision( pObj->GetSphere() ) == false ) return;

	// �A�N�V�����{�^���t���O�𗧂Ă�.

	// �L�[�����󂯎��.
	
	// ���������Ă��Ȃ����
	// �e�L�X�g��\�������� �I��.
	if( pObj->GetBitFlag( BitFlag::isHavKey ) == false ){
		CTutorialText::SetArrayNumber(3);
		if( m_bPlayNoKeySE == false ){
			CPlaySEThread::SetSEName( "NoKey" );
			m_bPlayNoKeySE = true;
		}
		return;
	}
	if( CXInput::IsPress( XINPUT_GAMEPAD_B ) == true ){
		CPlaySEThread::SetSEName( "DoorOpen" );
		BitFlagON( BitFlag::isKeyDoorOpen );
		m_bMoveFlag = true;
		pObj->BitFlagOFF( BitFlag::isHavKey );
	}
}

void CKeyDoor::FadeInOut()
{
	if( m_fCount >= 0.0f ){
		if( m_fCount >= 1.5f ) m_bFadeInFlag = false;
		if( m_bFadeInFlag == true ){
			m_fCount += 0.04f;
		} else{
			m_fCount -= 0.03f;
		}
	}
}

LPD3DXMESH CKeyDoor::GetMeshData()
{
	if( m_pStaticMesh == nullptr ) return nullptr;
	return m_pStaticMesh->GetMesh();
}