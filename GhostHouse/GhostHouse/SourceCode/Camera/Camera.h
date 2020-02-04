#ifndef CCAMERA_H
#define CCAMERA_H
//�x���ɂ��ẴR�[�h���͂𖳌��ɂ���.4005:�Ē�`.
#pragma warning( disable:4005 )
#include <Windows.h>

#include <D3DX11.h>
#include <D3D11.h>
#include <D3DX10.h>	// �uD3DX�`�v�̒�`�g�p���ɕK�v.
#include <D3D10.h>

#include "..\XInput\XInput.h"
#include "..\Main\Global.h"

#include "..\Collision\CollisionRay\CollisionRay.h"

//���C�u�����ǂݍ���.
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx10.lib")	//�uD3DX�`�v�̒�`�g�p���ɕK�v.

class CCamera
{
private:
	const float CAMERA_ROTATION_SPEED = 2.3f;
	const float MOVE_SPEED = 0.3f;
	const float CAMERA_UP_DISTANCE = 16.0f;
	const float EDITCAMERA_DEGREE = 7.0f;
	const float MAX_GIMMICKCOLPOS_DISTANCE	= 0.3f; // �����Ȃ����̃M�~�b�N�n�������蔻����W�Ƃ̍ő勗��.
	const float MAX_GIMMICKPOS_DISTANCE		= 0.1f;		// �����Ȃ����̒��S���W�Ƃ̍ő勗��.
public:
	CCamera();
	~CCamera();

	struct CameraRotation
	{
		D3DXVECTOR3 m_vPos;
		float m_CenterX;
		float m_CenterY;
		float m_CenterZ;
		float m_Degree;
		float m_Degree_y;

		float m_Length;
	};

	struct OldCameraPosition
	{
		D3DXVECTOR3 m_vPosition;
		float m_fDegree;
		float m_fRadian;
	};

	D3DXVECTOR3 MoveCameraPos( const D3DXVECTOR3& vCenPos );
	D3DXVECTOR3 EditMoveCameraPos( const D3DXVECTOR3& vCenPos );

	void UpdateCamera();
	void EditUpdateCamera();

	void RayColl( shared_ptr<CObjectBase> pObj );
	// �r���[�}�g���b�N�X��Ԃ��֐�.
	D3DXMATRIX GetViewMatrix()
	{
		return mView;
	}
	// �v���W�F�N�V�����}�g���b�N�X��Ԃ��֐�.
	D3DXMATRIX GetProjectionPos()
	{
		return mProj;
	}
	// �J�����̃��W�A����Ԃ��֐�.
	float GetCameraRadian()
	{
		return CameraRotationToRadian;
	}
	// �J�����̃f�O���[�ɉ��Z,���Z����֐�.
	void SetDegree( const float& fdegree )
	{
		stCamera.fDegree += fdegree;
	}
	// �J�����̃f�O���[��ύX����֐�.
	void ChangeDegree( const float& fdegree )
	{
		stCamera.fDegree = fdegree;
	}
	// �J�����̃M�~�b�N��ւ̈ړ��̏I���t���O��Ԃ��֐�.
	bool GetGimickCameraMoveEnd()
	{
		return m_bIsEndMove;
	}
	// �J�����̃M�~�b�N��ւ̈ړ��̏I���t���O�̃Z�b�^�[.
	void SetGimickCameraMoveEnd( bool MoveEndFlag )
	{
		m_bIsEndMove = MoveEndFlag;
	}
	// �J�����̃M�~�b�N��ւ̈ړ��̌��݃X�e�b�v��Ԃ��֐�.
	int GetNowStep()
	{
		return m_Step;
	}
	bool IsCameraUpMove(){ return m_Step == 2 ? true : false; }
	//�M�~�b�N�̏�Ɉړ�����p�֐�.
	D3DXVECTOR3 GimmickUpCamera( const D3DXVECTOR3& vInvisibleCenPos, const D3DXVECTOR3& vInvisibleColPos );
	D3DXVECTOR3 GetCameraPos(){ return stCamera.Pos; }
	void SetCameraPos( const D3DXVECTOR3& pos ){ stCamera.Pos = pos; }

	void SetLookPos( const D3DXVECTOR3& pos )
	{
		Camera.m_CenterX = pos.x;
		Camera.m_CenterY = pos.y;
		Camera.m_CenterZ = pos.z;
	}
	// �I�[�o�[�t���[�`�F�b�N.
	void CheckOverFlow(float CheckNumber) {
		if (CheckNumber >= 360.0f) {
			CheckNumber -= 360.0f;
		}
		else if (CheckNumber <= -360.0f) {
			CheckNumber += 360.0f;
		}
	}
private:

	OldCameraPosition OldCamera;
	CameraRotation	Camera;
	stCAMERA		stCamera;

	D3DXVECTOR3		m_vCamera;
	D3DXVECTOR3		m_vLook;	//�����ʒu.
	D3DXVECTOR3		m_vGimmickPosition;//�����Ȃ����M�~�b�N�̈ʒu.
	D3DXVECTOR3		m_vInvisiColPosiiton;

	D3DXMATRIX mWorld;	//���[���h�s��.
	D3DXMATRIX mView;	//�r���[(�J����)�s��.
	D3DXMATRIX mProj;	//�v���W�F�N�V����(�ˉe)�s��.

	D3DXMATRIX mRot_x;
	D3DXMATRIX mRot_y;
	D3DXMATRIX mRot_z;
	D3DXMATRIX mScale;
	D3DXMATRIX mTrans;

	const int STICKRANGE = 10000;
	float CameraRotationToRadian;
	float CameraRotationToDegree;
	//�J�������ړ������ǂ���.
	bool m_bCameraNowMove;

	bool Oneflow;

	int m_Step;

	float m_fColDistance;
	float m_fColRadian;
	float m_fInvGimmickDistance;
	float m_fInvGimmickRadian;
	bool	m_bIsEndMove;

	unique_ptr<CCollisionRay>	m_pCollRay;

	// 2�_�̋����𑪂�֐�.
	virtual float GetTwoDistance( const D3DXVECTOR3& vMyPos, const D3DXVECTOR3& vtargetPos );
	// 2�_�̊p�x�𑪂�֐�.
	virtual float GetTwoRadian( const D3DXVECTOR3& vMyPos, const D3DXVECTOR3& vtargetPos );


	D3DXVECTOR3 AxisZProc( float y );
	
};



#endif // !C_CAMERA_H
