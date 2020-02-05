#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

#include "..\CollisionBox\CollisionBox.h"
#include "..\CollisionSphere\CollisionSphere.h"
#include "..\CollisionRay\CollisionRay.h"

class CCollisionManager
{
public:
	CCollisionManager( D3DXVECTOR3* pvPos, float* pfRot_y );
	~CCollisionManager();

	// �e�평�����֐�.
	void Init( LPD3DXMESH pMesh );

	// �{�b�N�X���m�ł̓����蔻��.
	bool isBoxCollision( CCollisionBox* collBox )
	{ return m_pCollBox->isCollision( collBox ); }
	// �{�b�N�X�擾�֐�.
	CCollisionBox* GetBox(){ return m_pCollBox.get(); };

	// �X�t�B�A���m�ł̓����蔻��.
	bool isSphereCollision( CCollisionSphere* collSphere )
	{ return m_pCollSphere->isCollision( collSphere ); };
	// �X�t�B�A�擾�֐�.
	CCollisionSphere* GetSphere(){ return m_pCollSphere.get(); };

	// ���C�ł̓����蔻��.
	bool isRayCollision( shared_ptr<CObjectBase> pTargetObj, float* pDistance, D3DXVECTOR3* pIntersect )
	{ return m_pCollRay->Intersect( pTargetObj, pDistance, pIntersect ); };
	// ��]���ݒ�֐�.
	void RaySetAxis( const float& fx, const float& fy, const float& fz )
	{ m_pCollRay->SetAxis( fx, fy, fz ); }
	void RaySetAxis( const D3DXVECTOR3& vAxis )
	{ m_pCollRay->SetAxis( vAxis ); }
	// ���C�̕����ݒ�֐�.
	void RaySetRay( const D3DXVECTOR3& vRay )
	{ m_pCollRay->SetRay( vRay ); }

private:
	CCollisionManager();
	shared_ptr<CCollisionBox>		m_pCollBox;		// �{�b�N�X.
	shared_ptr<CCollisionSphere>	m_pCollSphere;	// �X�t�B�A.
	shared_ptr<CCollisionRay>		m_pCollRay;		// ���C.
};

#endif	// #ifndef COLLISION_MANAGER_H.