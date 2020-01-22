#ifndef CCAMERA_H
#define CCAMERA_H
//警告についてのコード分析を無効にする.4005:再定義.
#pragma warning( disable:4005 )
#include <Windows.h>

#include <D3DX11.h>
#include <D3D11.h>
#include <D3DX10.h>	// 「D3DX～」の定義使用時に必要.
#include <D3D10.h>

#include "..\XInput\XInput.h"
#include "..\Main\Global.h"

#include "..\Collision\CollisionRay\CollisionRay.h"

//ライブラリ読み込み.
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx10.lib")	//「D3DX～」の定義使用時に必要.

class CCamera
{
private:
	float A = 111;
	const float CAMERA_ROTATION_SPEED = 2.3f;
	const float MOVE_SPEED = 0.3f;
	const float CAMERA_UP_DISTANCE = 16.0f;
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

	D3DXMATRIX GetViewMatrix()
	{
		return mView;
	}

	D3DXMATRIX GetProjectionPos()
	{
		return mProj;
	}

	float GetCameraRadian()
	{
		return CameraRotationToRadian;
	}

	void SetDegree( const float& fdegree )
	{
		stCamera.fDegree += fdegree;
	}
	void ChangeDegree( const float& fdegree )
	{
		stCamera.fDegree = fdegree;
	}

	bool GetGimickCameraMoveEnd()
	{
		return m_bIsEndMove;
	}
	void SetGimickCameraMoveEnd( bool MoveEndFlag )
	{
		m_bIsEndMove = MoveEndFlag;
	}

	int GetNowStep()
	{
		return m_Step;
	}
	bool IsCameraUpMove(){ return m_Step == 2 ? true : false; }
	//ギミックの上に移動する用関数.
	D3DXVECTOR3 GimmickUpCamera( const D3DXVECTOR3& vInvisibleCenPos, const D3DXVECTOR3& vInvisibleColPos );
	D3DXVECTOR3 GetCameraPos(){ return stCamera.Pos; }
	void SetCameraPos( const D3DXVECTOR3& pos ){ stCamera.Pos = pos; }

	void SetLookPos( const D3DXVECTOR3& pos )
	{
		Camera.m_CenterX = pos.x;
		Camera.m_CenterY = pos.y;
		Camera.m_CenterZ = pos.z;
	}
private:

	OldCameraPosition OldCamera;
	CameraRotation	Camera;
	stCAMERA		stCamera;

	D3DXVECTOR3		m_vCamera;
	D3DXVECTOR3		m_vLook;	//注視位置.
	D3DXVECTOR3		m_vGimmickPosition;//見えない床ギミックの位置.
	D3DXVECTOR3		m_vInvisiColPosiiton;

	D3DXMATRIX mWorld;	//ワールド行列.
	D3DXMATRIX mView;	//ビュー(カメラ)行列.
	D3DXMATRIX mProj;	//プロジェクション(射影)行列.

	D3DXMATRIX mRot_x;
	D3DXMATRIX mRot_y;
	D3DXMATRIX mRot_z;
	D3DXMATRIX mScale;
	D3DXMATRIX mTrans;

	const int STICKRANGE = 10000;
	float CameraRotationToRadian;
	float CameraRotationToDegree;
	//カメラが移動中かどうか.
	bool m_bCameraNowMove;

	bool Oneflow;

	int m_Step;

	float m_fColDistance;
	float m_fColRadian;
	float m_fInvGimmickDistance;
	float m_fInvGimmickRadian;
	bool	m_bIsEndMove;

	unique_ptr<CCollisionRay>	m_pCollRay;

	// 2点の距離を測る関数.
	virtual float GetTwoDistance( const D3DXVECTOR3& vMyPos, const D3DXVECTOR3& vtargetPos );
	// 2点の角度を測る関数.
	virtual float GetTwoRadian( const D3DXVECTOR3& vMyPos, const D3DXVECTOR3& vtargetPos );


	D3DXVECTOR3 AxisZProc( float y );
	
};



#endif // !C_CAMERA_H
