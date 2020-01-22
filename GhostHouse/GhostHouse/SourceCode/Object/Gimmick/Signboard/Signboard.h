#ifndef SINGNBOARD_H
#define SINGNBOARD_H

#include "..\..\..\ObjectBase\GimmickBase\GimmickBase.h"
#include "MultiTexture/MultiTexture.h"

class CSignboard : public CGimmickBase
{
	const char*	MAIN_MODEL_NAME = "Signboard";	// �d�l���f����.
	const float MODEL_SCALE		= 0.08f;
public:
	CSignboard( const stObjectInfo& objInfo );
	~CSignboard();

	// �X�V�֐�.
	virtual void Update( shared_ptr<CObjectBase> pObj )	override;
	// �`��֐�.
	virtual void Render( D3DXMATRIX& mView, D3DXMATRIX& mProj,
		Light& stLight, stCAMERA& stCamera )	override;
	// �����蔻��p�֐�.
	virtual void Collision( shared_ptr<CObjectBase> pObj ) override;
	virtual void Load( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 )	override;

	virtual LPD3DXMESH GetMeshData() override;

private:
	CSignboard();
	// ����֐�.
	virtual void Action( shared_ptr<CObjectBase> pObj )	override;

	shared_ptr<CDX9Mesh>		m_pStaticMesh;
};

#endif	// #ifndef SINGNBOARD_H.