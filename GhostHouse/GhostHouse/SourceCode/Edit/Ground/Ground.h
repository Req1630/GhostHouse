#ifndef GROUND_H
#define GROUND_H

#include "..\..\ObjectBase\ObjectBase.h"

class CGround : public CObjectBase
{
public:
	CGround();
	virtual ~CGround();

	// 更新関数.
	virtual void Update( shared_ptr<CObjectBase> pObj ) override;
	// 描画関数.
	virtual void Render( D3DXMATRIX& mView, D3DXMATRIX& mProj, Light& stLight, stCAMERA& stCamera ) override;
	// 読み込み関数.
	virtual void Load( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 ) override;
	virtual void Collision( shared_ptr<CObjectBase> pObj ) override
	{}
	virtual LPD3DXMESH GetMeshData() override;
private:
	shared_ptr<CDX9Mesh>	m_pStaticMesh;
};

#endif	// #ifndef GROUND_H.