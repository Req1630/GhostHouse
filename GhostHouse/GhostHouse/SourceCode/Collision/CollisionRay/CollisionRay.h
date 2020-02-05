#ifndef COLLISION_RAY_H
#define COLLISION_RAY_H

#include "..\..\Main\Global.h"

class CDX9Mesh;
class CObjectBase;

class CCollisionRay
{
public:
	CCollisionRay( float* pfYaw );
	~CCollisionRay();

	// ���C�ƃ��b�V���̓����蔻��.
	bool Intersect( shared_ptr<CObjectBase> pTargetObj,
		float* pfDistance,			// (out)����.
		D3DXVECTOR3* pvIntersect, D3DXVECTOR3 endpos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );	// (out)�������W.

	// �����ʒu�̃|���S���̒��_��������.
	HRESULT FindVerticesOnPoly(
		LPD3DXMESH pMeshForRay, DWORD dwPolyIndex,
		D3DXVECTOR3* pVecVertices );	// (out)���_���W(���[�J�����W),

	// ��]���ݒ�֐�.
	void SetAxis( const float& fx, const float& fy, const float& fz )
	{ m_vAxis = D3DXVECTOR3( fx, fy, fz ); }
	// ��]���ݒ�֐�.
	void SetAxis( const D3DXVECTOR3& vAxis ) { m_vAxis = vAxis; }
	// ���C�̐ݒ�.
	void SetRay( const D3DXVECTOR3& vRay ) { m_vRay = vRay; }

private:
	CCollisionRay(){};
	float*			m_pfYaw;			// Y����]�l.
	D3DXVECTOR3		m_vAxis;			// ��].
	D3DXVECTOR3		m_vRay;				// ���C.
};

#endif	// #ifndef COLLISION_RAY_H.
