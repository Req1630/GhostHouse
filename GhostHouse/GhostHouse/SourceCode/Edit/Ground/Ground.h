#ifndef GROUND_H
#define GROUND_H

#include "..\..\ObjectBase\ObjectBase.h"

class CGround : public CObjectBase
{
public:
	CGround();
	virtual ~CGround();

	// �X�V�֐�.
	virtual void Update( shared_ptr<CObjectBase> pObj ) override;
	// �`��֐�.
	virtual void Render( D3DXMATRIX& mView, D3DXMATRIX& mProj, Light& stLight, stCAMERA& stCamera ) override;
	// �ǂݍ��݊֐�.
	virtual void Load( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 ) override;
	// �����蔻��֐�.
	virtual void Collision( shared_ptr<CObjectBase> pObj ) override
	{}
	// ���b�V���擾�֐�.
	virtual LPD3DXMESH GetMeshData() override;
private:
	shared_ptr<CDX9Mesh>	m_pStaticMesh;	// ���b�V���N���X.
};

#endif	// #ifndef GROUND_H.