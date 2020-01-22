#ifndef PLAYER_H
#define PLAYER_H

#include "..\..\ObjectBase\ObjectBase.h"
#include "..\..\Camera\Camera.h"

class CPlayer : public CObjectBase
{

public:
	CPlayer();
	virtual ~CPlayer();

	// 更新関数.
	virtual void Update( shared_ptr<CObjectBase> pObj ) override;
	// 描画関数.
	virtual void Render( D3DXMATRIX& mView, D3DXMATRIX& mProj, Light& stLight, stCAMERA& stCamera ) override;
	// 読み込み関数.
	virtual void Load( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 ) override;
	virtual void Collision( shared_ptr<CObjectBase> pObj ) override
	{}
	virtual LPD3DXMESH GetMeshData() override;

	D3DXVECTOR3 GetObjectPosition() const { return m_vbufPosition; }
	void Control();
private:
	bool StickInclination( float divideValue );
	void RayHits( shared_ptr<CObjectBase> pObj );

	shared_ptr<CDX9Mesh>		m_pStaticMesh;
	unique_ptr<CCollisionRay>	m_pCollRay;

	// 移動中の仮オブジェクト.
	shared_ptr<CDX9Mesh>	m_pbufObject;
	D3DXVECTOR3				m_vbufPosition;

	shared_ptr<CCamera>		m_pCamera;

	bool m_isMove;
	int m_StickCount;

	float		m_fRadian;
	D3DXVECTOR2 m_vMove;
	
	D3DXMATRIX	m_mView;

};

#endif	// #ifndef PLAYER_H.