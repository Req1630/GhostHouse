#ifndef COLLISION_SPHERE_H
#define COLLISION_SPHERE_H

#include "..\..\Main\Global.h"
#include "..\..\DebugMode\Sphere\Sphere.h"

class CDX9Mesh;

class CCollisionSphere
{
public:
	CCollisionSphere( D3DXVECTOR3* pvPosition );
	~CCollisionSphere();

	// ���菈��.
	bool isCollision( CCollisionSphere* collBox );

	// �X�t�B�A������.
	HRESULT InitSphere( LPD3DXMESH pMesh );

	// �e���擾�֐�.
	D3DXVECTOR3	GetPosition()	{ return *m_pvPosition; }
	float		GetRadius()		{ return m_fRadius; }

	// ���a�ݒ�.
	void SetRadius( const float & fRad ) { m_fRadius = fRad; }

private:
	CCollisionSphere();

private:
	D3DXVECTOR3*	m_pvPosition;	// ���W.
	float			m_fRadius;		// ���a.
};

#endif	// #ifndef COLLISION_SPHERE_H.
