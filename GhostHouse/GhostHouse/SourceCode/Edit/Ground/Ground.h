#ifndef GROUND_H
#define GROUND_H

#include "..\..\ObjectBase\ObjectBase.h"

class CGround : public CObjectBase
{
public:
	CGround();
	virtual ~CGround();

	// çXêVä÷êî.
	virtual void Update( shared_ptr<CObjectBase> pObj ) override;
	// ï`âÊä÷êî.
	virtual void Render( D3DXMATRIX& mView, D3DXMATRIX& mProj, Light& stLight, stCAMERA& stCamera ) override;
	// ì«Ç›çûÇ›ä÷êî.
	virtual void Load( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 ) override;
	virtual void Collision( shared_ptr<CObjectBase> pObj ) override
	{}
	virtual LPD3DXMESH GetMeshData() override;
private:
	shared_ptr<CDX9Mesh>	m_pStaticMesh;
};

#endif	// #ifndef GROUND_H.