#include "Camera.h"
#include <D3DX10Math.h>
#include "..\Main\MyMacro.h"
#include "..\ObjectBase\ObjectBase.h"


CCamera::CCamera()
	: m_vCamera( 0.0f, 0.0f, 0.0f )
	, m_vLook( 0.0f, 0.0f, 0.0f )
	, OldCamera()
	, m_bCameraNowMove( true )
	, m_vGimmickPosition( 0.0f, 0.0f, 11.0f )
	, m_vInvisiColPosiiton()
	, CameraRotationToRadian( 0.0f )
	, CameraRotationToDegree( 0.0f )
	, Oneflow( false )
	, m_pCollRay( make_unique<CCollisionRay>( &CameraRotationToRadian ) )
	, m_Step( 2 )
	, m_fColDistance( 0.0f )
	, m_fColRadian( 0.0f )
	, m_fInvGimmickDistance( 0.0f )
	, m_fInvGimmickRadian( 0.0f )
{

	Camera.m_CenterX = 0.0f;
	Camera.m_CenterY = 0.0f;
	Camera.m_CenterZ = 0.0f;
	Camera.m_Length = 5.0f;
	Camera.m_Degree = 0.0f;
	Camera.m_Degree_y = 2.0f;
	CameraRotationToRadian = 0.0f;
	Camera.m_vPos.x = 0.0f;
	Camera.m_vPos.y = 1.0f;
	Camera.m_vPos.z = 0.0f;

	stCamera.fDegree = 0.0f;
	stCamera.fLength = 18.0f;
	stCamera.Pos = D3DXVECTOR3( 0.0f, 10.0f, 0.0f );
	stCamera.vRot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	stCamera.InvisibleCenter = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
}

CCamera::~CCamera()
{
}
// �Q�[�����C���̎�����܂��J�����̃A�b�v�f�[�g.
void CCamera::UpdateCamera()
{
	if( CXInput::GetRThumbX() > STICKRANGE || GetAsyncKeyState( VK_RIGHT ) & 0x8000 ){
		stCamera.fDegree += CAMERA_ROTATION_SPEED;
	}

	if( CXInput::GetRThumbX() < -STICKRANGE || GetAsyncKeyState( VK_LEFT ) & 0x8000 ){
		stCamera.fDegree -= CAMERA_ROTATION_SPEED;
	}
}
// �}�b�v�G�f�B�b�g���[�h�J�����A�b�v�f�[�g.
void CCamera::EditUpdateCamera()
{
	if( GetAsyncKeyState( VK_RIGHT ) & 0x8000 || CXInput::GetRThumbX() > INPUT_THUMB_MAX ){
		Camera.m_Degree += EDITCAMERA_DEGREE;
	}

	if( GetAsyncKeyState( VK_LEFT ) & 0x8000 || CXInput::GetRThumbX() < INPUT_THUMB_MIN ){
		Camera.m_Degree -= EDITCAMERA_DEGREE;
	}

	if( GetAsyncKeyState( VK_UP ) & 0x8000 || CXInput::GetRThumbY() > INPUT_THUMB_MAX ){
		Camera.m_Degree_y -= 1.0f;
		if( Camera.m_Degree_y <= 2.0f ){
			Camera.m_Degree_y = 2.0f;
		}
	}

	if( GetAsyncKeyState( VK_DOWN ) & 0x8000 || CXInput::GetRThumbY() < INPUT_THUMB_MIN ){
		Camera.m_Degree_y += 1.0f;
		if( Camera.m_Degree_y >= 80.0f ){
			Camera.m_Degree_y = 80.0f;
		}
	}

	if( GetAsyncKeyState( 'Q' ) & 0x0001 || CXInput::GetRTrigger() > 200 ){
		Camera.m_Length += 0.5f;
		if( Camera.m_Length >= 50.0f ){
			Camera.m_Length = 50.0f;
		}
	}
	if( GetAsyncKeyState( 'E' ) & 0x0001 || CXInput::GetLTrigger() > 200 ){
		Camera.m_Length -= 0.5f;
		if( Camera.m_Length <= 3.0f ){
			Camera.m_Length = 3.0f;
		}
	}
}

D3DXVECTOR3 CCamera::GimmickUpCamera( const D3DXVECTOR3& vInvisibleCenPos, const D3DXVECTOR3& vInvisibleColPos )
{
	if( m_fInvGimmickDistance > 0.8f ){
		if( Oneflow == false ){
			m_Step = 0;
			Oneflow = true;
			//m_bIsEndMove = false;
			SetGimickCameraMoveEnd( false );
		}
	}
	// �����Ȃ����M�~�b�N�ŃJ��������Ɍ����킹��X�e�b�v.
	switch( m_Step ){
		case 0:		
			m_vInvisiColPosiiton = vInvisibleColPos;			// �����Ȃ����̃M�~�b�N�n�������蔻����W�擾.
			// �J�����̍��W�ƌ����Ȃ����̓����蔻����W�̋������擾.
			m_fColDistance = GetTwoDistance( stCamera.Pos, m_vInvisiColPosiiton );
			// �J�����̍��W�ƌ����Ȃ����̓����蔻��p�x���擾.
			m_fColRadian = GetTwoRadian( stCamera.Pos, m_vInvisiColPosiiton );
			// �������w��l���傫���Ԓ��ɓ���.
			if( m_fColDistance > MAX_GIMMICKCOLPOS_DISTANCE){
				// �J�����̕����������Ȃ����Ɍ�����.
				stCamera.vRot.y = m_fColRadian;
				// �J�����ړ�.
				stCamera.Pos += AxisZProc( stCamera.vRot.y ) * MOVE_SPEED;
			} else{
				// �������\���Ȃ̂ŃX�e�b�v��i�߂�.
				m_Step++;
			}
			CameraRotationToRadian = m_fColRadian;
			// �J�����̍��W�X�V.
			m_vCamera = { stCamera.Pos.x, stCamera.Pos.y + CAMERA_UP_DISTANCE, stCamera.Pos.z };

			return m_vCamera;
		case 1:
			m_vGimmickPosition = vInvisibleCenPos;	// �����Ȃ����̒��S���W�擾.
			// �J�����̍��W�ƌ����Ȃ����̒��S���W�̋������擾.
			m_fInvGimmickDistance = GetTwoDistance( stCamera.Pos, m_vGimmickPosition );
			// �J�������߂����ۂɐi�s�����ɃJ�����������悤�Ɋp�x��ύX.
			stCamera.fDegree = static_cast<float>(D3DXToDegree(GetTwoRadian(stCamera.Pos, m_vGimmickPosition)));
			// �J�����̍��W�ƌ����Ȃ����̒��S���W�̊p�x���擾.
			m_fInvGimmickRadian = GetTwoRadian( stCamera.Pos, m_vGimmickPosition );
			// �������w��l���傫���Ԓ��ɓ���.
			if( m_fInvGimmickDistance > MAX_GIMMICKPOS_DISTANCE){
				// �J�����̕����������Ȃ����Ɍ�����.
				stCamera.vRot.y = m_fInvGimmickRadian;
				// �J�����ړ�.
				stCamera.Pos += AxisZProc( stCamera.vRot.y ) * (0.1f);
			} else {
				// �����͏\���Ȃ̂ŃX�e�b�v��i�߂�.
				m_Step++;
				// ��Őݒ肵���z��180�x��
				stCamera.fDegree += 180.0f;
			}
			CameraRotationToRadian = m_fInvGimmickRadian;
			// �J�����̍��W�X�V.
			m_vCamera = { stCamera.Pos.x, stCamera.Pos.y + CAMERA_UP_DISTANCE, stCamera.Pos.z };
			// �J�����̒����_�������Ȃ����̒��S���W��.
			stCamera.vLook = vInvisibleCenPos;
			return m_vCamera;
		case 2:
			m_vGimmickPosition = vInvisibleCenPos;
			m_fInvGimmickDistance = GetTwoDistance( stCamera.Pos, m_vGimmickPosition );
			m_fInvGimmickRadian = GetTwoRadian( stCamera.Pos, m_vGimmickPosition );
			
			Oneflow = false;
			CameraRotationToRadian = m_fInvGimmickRadian;
			m_vCamera = { stCamera.Pos.x, stCamera.Pos.y + CAMERA_UP_DISTANCE, stCamera.Pos.z };
			stCamera.vLook = vInvisibleCenPos;
			// �J�����̈ړ����I������.
			SetGimickCameraMoveEnd( true );
			return m_vCamera;
		default:
			break;
	}

	return m_vCamera;
}

D3DXVECTOR3 CCamera::MoveCameraPos( const D3DXVECTOR3& vCenPos )
{
	// �Q�[���S�̂̎��_�̍��W���擾����.
	Camera.m_CenterX = vCenPos.x;
	Camera.m_CenterY = vCenPos.y;
	Camera.m_CenterZ = vCenPos.z;

	//�I�[�o�[�t���[�΍�.
	CheckOverFlow(stCamera.fDegree);
	//if( stCamera.fDegree >= 360.0f ){
	//	stCamera.fDegree -= 360.0f;
	//} else if( stCamera.fDegree <= -360.0f ){
	//	stCamera.fDegree += 360.0f;
	//}

	//�p�x�����W�A���l�ɕϊ�.
	CameraRotationToRadian =
		stCamera.fDegree * ( 3.14f / 180.0f );


	//sin,cos:-1.0�`1.0f.
	stCamera.Pos.x =
		Camera.m_CenterX + ( sinf( CameraRotationToRadian ) * stCamera.fLength );

	stCamera.Pos.z =
		Camera.m_CenterZ + ( cosf( CameraRotationToRadian ) * stCamera.fLength );

	m_vCamera = { stCamera.Pos.x, stCamera.Pos.y, stCamera.Pos.z };

	return m_vCamera;
}

D3DXVECTOR3 CCamera::EditMoveCameraPos( const D3DXVECTOR3& vCenPos )
{
	// �Q�[���S�̂̎��_�̍��W���擾����.
	Camera.m_CenterX = vCenPos.x;
	Camera.m_CenterY = vCenPos.y;
	Camera.m_CenterZ = vCenPos.z;

	//�I�[�o�[�t���[�΍�.
	CheckOverFlow(Camera.m_Degree);
	CheckOverFlow(Camera.m_Degree_y);

	//if( Camera.m_Degree >= 360.0f ){
	//	Camera.m_Degree -= 360.0f;
	//} else if( Camera.m_Degree <= -360.0f ){
	//	Camera.m_Degree += 360.0f;
	//}
	//if( Camera.m_Degree_y >= 360.0f ){
	//	Camera.m_Degree_y -= 360.0f;
	//} else if( Camera.m_Degree_y <= -360.0f ){
	//	Camera.m_Degree_y += 360.0f;
	//}

	//�p�x�����W�A���l�ɕϊ�.
	CameraRotationToRadian =
		Camera.m_Degree * ( 3.14f / 180.0f );

	float y = Camera.m_Degree_y * ( 3.14f / 180.0f );


	//sin,cos:-1.0�`1.0f.
	Camera.m_vPos.x =
		Camera.m_CenterX + ( sinf( CameraRotationToRadian ) * Camera.m_Length );

	Camera.m_vPos.y =
		Camera.m_CenterY + ( sinf( y ) * Camera.m_Length );

	Camera.m_vPos.z =
		Camera.m_CenterZ + ( cosf( CameraRotationToRadian ) * Camera.m_Length );

	m_vCamera = { Camera.m_vPos.x, Camera.m_vPos.y, Camera.m_vPos.z };

	return m_vCamera;
}

void CCamera::RayColl( shared_ptr<CObjectBase> pObj )
{
	if( pObj->GetObjectNo() == enObjectNo::Ground ) return;
	float fDistance;
	D3DXVECTOR3 vIntersect;
	D3DXVECTOR3 vEndPos;
	D3DXVECTOR3 vCameraPos = stCamera.Pos;
	D3DXVECTOR3 vLookPos = D3DXVECTOR3( Camera.m_CenterX, Camera.m_CenterY, Camera.m_CenterZ );
	m_pCollRay->SetRay( vLookPos );

	float range = -0.5f;
	for( int i = 0; i < 50; i++ ){
		vCameraPos.x =
			Camera.m_CenterX + ( sinf( CameraRotationToRadian + range ) * 18.0f );
		vCameraPos.z =
			Camera.m_CenterZ + ( cosf( CameraRotationToRadian + range ) * 18.0f );

		if( m_pCollRay->Intersect( pObj, &fDistance, &vIntersect, vCameraPos ) == true ){
			pObj->SetAlpha( 0.51f );
		}
		range += 0.02f;
	}
}


// 2�_�̋����𑪂�֐�.
float CCamera::GetTwoDistance( const D3DXVECTOR3& vMyPos, const D3DXVECTOR3& vtargetPos )
{
	float distance = sqrtf( ( vtargetPos.x - vMyPos.x ) * ( vtargetPos.x - vMyPos.x )
		+ ( vtargetPos.z - vMyPos.z ) * ( vtargetPos.z - vMyPos.z ) );

	return distance;
}

// 2�_�̊p�x�𑪂�֐�.
float CCamera::GetTwoRadian( const D3DXVECTOR3& vMyPos, const D3DXVECTOR3& vtargetPos )
{
	float radian = atan2f( vtargetPos.x - vMyPos.x, vtargetPos.z - vMyPos.z );

	return radian;
}

D3DXVECTOR3 CCamera::AxisZProc( float y )
{
	// ���x�N�g����p��.
	D3DXVECTOR3 vecAxisZ( 0.0f, 0.0f, 1.0f );	// �ϊ��O�x�N�g��.

	D3DXMATRIX mYaw;	// Y����]�s��.

	// Y����].
	D3DXMatrixRotationY( &mYaw, y );

	// Z���x�N�g�����̂��̂���]��Ԃɂ��ϊ�����.
	D3DXVec3TransformCoord( &vecAxisZ,	// (out)�ԊҌ�x�N�g��.
		&vecAxisZ, &mYaw );
	
	return vecAxisZ;
}