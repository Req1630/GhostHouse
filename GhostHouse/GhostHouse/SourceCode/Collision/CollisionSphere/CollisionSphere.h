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

	// 判定処理.
	bool isCollision( CCollisionSphere* collBox );

	// スフィア初期化.
	HRESULT InitSphere( LPD3DXMESH pMesh );
	// デバッグ用ボックス初期化.
	void	InitDebug( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 );
	// デバッグ用描画.
	void	DebugRender( D3DXMATRIX& mView, D3DXMATRIX& mProj );

	// 各情報取得関数.
	D3DXVECTOR3	GetPosition()	{ return *m_pvPosition; }
	float		GetRadius()		{ return m_fRadius; }

	// 半径設定.
	void SetRadius( const float & fRad ) { m_fRadius = fRad; }

private:
	CCollisionSphere();

private:
	D3DXVECTOR3*	m_pvPosition;
	float			m_fRadius;
};

#endif	// #ifndef COLLISION_SPHERE_H.
