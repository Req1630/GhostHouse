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

	void Init( LPD3DXMESH pMesh );

	bool isBoxCollision( CCollisionBox* collBox )
	{ return m_pCollBox->isCollision( collBox ); }
	CCollisionBox* GetBox(){ return m_pCollBox.get(); };

	bool isSphereCollision( CCollisionSphere* collSphere )
	{ return m_pCollSphere->isCollision( collSphere ); };
	CCollisionSphere* GetSphere(){ return m_pCollSphere.get(); };

	bool isRayCollision( shared_ptr<CObjectBase> pTargetObj, float* pDistance, D3DXVECTOR3* pIntersect )
	{ return m_pCollRay->Intersect( pTargetObj, pDistance, pIntersect ); };
	void RaySetAxis( const float& fx, const float& fy, const float& fz )
	{ m_pCollRay->SetAxis( fx, fy, fz ); }
	void RaySetAxis( const D3DXVECTOR3& vAxis )
	{ m_pCollRay->SetAxis( vAxis ); }
	void RaySetRay( const D3DXVECTOR3& vRay )
	{ m_pCollRay->SetRay( vRay ); }


	void InitDebug( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 );
	void DebugRender( D3DXMATRIX& mView, D3DXMATRIX& mProj );

private:
	CCollisionManager();
	shared_ptr<CCollisionBox>		m_pCollBox;
	shared_ptr<CCollisionSphere>	m_pCollSphere;
	shared_ptr<CCollisionRay>		m_pCollRay;
};

#endif	// #ifndef COLLISION_MANAGER_H.