#include "Player.h"
#include "..\..\XInput\XInput.h"

#include "..\..\DebugMode\DebugText\DebugText.h"
#include "..\ModelResource\ModelResource.h"
#include "..\..\Resource\ModelResource\DX9Mesh\CDX9Mesh.h"

CPlayer::CPlayer()
	: m_pCamera		( make_shared<CCamera>() )

	, m_pCollRay	( make_unique<CCollisionRay>( &m_vRotation.y) )
	, m_pbufObject	( nullptr )
	, m_vbufPosition( 0.0f, 0.0f, 0.0f )
	, m_isMove		( false )
	, m_StickCount	( 0 )
	, m_fRadian		( 0.0f )
	, m_vMove		( 0.0f, 0.0f )
	, m_mView		()
{
	m_vPosition = D3DXVECTOR3( 0.0f, -1.0f, 0.0f );
	m_vRotation = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
}

CPlayer::~CPlayer()
{}

//-------------------------.
// �X�V�֐�.
//-------------------------.
void CPlayer::Update( shared_ptr<CObjectBase> pObj )
{
	if( m_pStaticMesh == nullptr ) return;

	m_pCamera->EditUpdateCamera();
}

//-------------------------.
// �`��֐�.
//-------------------------.
void CPlayer::Render( D3DXMATRIX& mView, D3DXMATRIX& mProj, Light& stLight, stCAMERA& stCamera )
{
	m_mView = mView;
	if( m_pStaticMesh == nullptr ) return;

	stCamera.vLook = m_vPosition;
	stCamera.vLook.y += 2.0f;
	stCamera.Pos = m_pCamera->EditMoveCameraPos( stCamera.vLook );
	m_vRotation = D3DXVECTOR3( 0.0f,  m_pCamera->GetCameraRadian() + m_fRadian, 0.0f );
	m_pStaticMesh->SetRotation( m_vRotation ); 
	m_pStaticMesh->SetPosition( m_vPosition );
	m_pStaticMesh->SetScale( 0.08f );
	m_pStaticMesh->Render( mView, mProj, stLight, stCamera );
}

//-------------------------.
// �ǂݍ��݊֐�.
//-------------------------.
void CPlayer::Load( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 )
{
	auto MyMeshLoad = [=]()
	{
		if( m_pStaticMesh != nullptr ) return;
		m_pStaticMesh = CEditModelResource::GetMesh("Sayaka_s");
	};
	
	MyMeshLoad();
}

//-------------------------.
// �X�e�B�b�N�̌X���擾.
//-------------------------.
bool CPlayer::StickInclination( float divideValue )
{
	m_isMove = false;
	// �X�e�B�b�N�̗V�т𒴂�������͌��m.
	if( CXInput::GetLThumbX() > INPUT_THUMB_MAX ||
		CXInput::GetLThumbX() < INPUT_THUMB_MIN ||
		CXInput::GetLThumbY() > INPUT_THUMB_MAX || 
		CXInput::GetLThumbY() < INPUT_THUMB_MIN ){

		m_vMove.x = CXInput::GetLThumbX();			//�R���g���[���̍��E�����̓��͂��擾.
		m_vMove.y = CXInput::GetLThumbY();			//�R���g���[���̏㉺�����̓��͂��擾.

		m_fRadian = atan2f( m_vMove.x, m_vMove.y );	//atan2f��float�^�̃��W�A���ɕϊ�.
		m_isMove = true;
		return true;
	}
	return false;
}

//-------------------------.
// ����֐�.
//-------------------------.
void CPlayer::Control()
{
	float movespd = 1.0f;

	auto SetMoveSpeed = [&]( float spd, int count )
	{
		if( m_StickCount < count ){
			if( StickInclination( 1.0f ) == true ){
				m_StickCount = count-1;
				movespd = spd;
			}
		}
	};
	SetMoveSpeed( 0.2f, 4 );

	if( m_isMove == true ){
		//���x�N�g����p��.
		D3DXVECTOR3 vAxisZ = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );//�ϊ��O�x�N�g��.

		D3DXMATRIX mYaw;//Y����]�s��.
		//Y����].
		D3DXMatrixRotationY( &mYaw, m_fRadian );

		//Z���x�N�g�����̂��̂���]��Ԃɂ��ϊ�����.
		D3DXVec3TransformCoord( &vAxisZ,	//(out)�ϊ���x�N�g��.
			&vAxisZ, &mYaw );

		//-----------------------------------------------------------------------------
		//		�J�����̌����ɑΉ�������.
		//
		D3DXMATRIX	m_SayakaMat;
		D3DXMatrixInverse( &m_SayakaMat, nullptr, &m_mView );
		m_SayakaMat.m[1][1] = 0;	//Y�����̓����Œ�.
		m_SayakaMat.m[2][1] = 0;	//Y�����̓����Œ�.

		D3DXVec3TransformNormal( &vAxisZ, &vAxisZ, &m_SayakaMat );
		
		m_vbufPosition.x = m_vPosition.x = m_vPosition.x + vAxisZ.x * movespd;
		m_vbufPosition.z = m_vPosition.z = m_vPosition.z + vAxisZ.z * movespd;
		m_vbufPosition = D3DXVECTOR3(
			ceilf(m_vbufPosition.x),
			ceilf(m_vbufPosition.y),
			ceilf(m_vbufPosition.z) );

		m_StickCount = 0;
	}
	if( CXInput::IsPress( XINPUT_GAMEPAD_LEFT_SHOULDER ) ){
		m_vbufPosition.y -= 1.0f;
	} else if( CXInput::IsPress( XINPUT_GAMEPAD_RIGHT_SHOULDER ) ){
		m_vbufPosition.y += 1.0f;
	}
}

//-------------------------.
// ���C�ł̓����蔻��.
//-------------------------.
void CPlayer::RayHits( shared_ptr<CObjectBase> pObj )
{
	float fDistance;
	D3DXVECTOR3 vIntersect;
	D3DXVECTOR3 vPos = m_vPosition;

	// ���������C�̐ݒ�.
	m_pCollRay->SetAxis( 0.0f, -1.0f, 0.0f );

	// �ΏۃI�u�W�F�N�g�������Ȃ���(�����鏰)�̏ꍇ�͏I��.
	if( pObj->GetObjectNo() == enObjectNo::SlipThroughFloor )return;

	// �Ώۂ̃I�u�W�F�N�g�Ƃ̃��C����.
	if( m_pCollRay->Intersect( pObj, &fDistance, &vIntersect ) == true ){
		// �ΏۃI�u�W�F�N�g�̏�ɐݒ肷��.
		m_vPosition.y = vIntersect.y;
	} else{
		// ���ɉ���������Ή��Ɉړ�������.
		m_vPosition.y -= 0.001f;
	}
}

//-------------------------.
// ���b�V���擾�֐�.
//-------------------------.
LPD3DXMESH CPlayer::GetMeshData()
{
	if( m_pStaticMesh == nullptr ) return nullptr;
	return m_pStaticMesh->GetMesh();
}