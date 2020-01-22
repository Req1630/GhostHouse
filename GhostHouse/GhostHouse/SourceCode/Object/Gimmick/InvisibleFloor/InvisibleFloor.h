#ifndef INVISIBLE_FLOOR_H
#define INVISIBLE_FLOOR_H

#include "..\..\..\ObjectBase\GimmickBase\GimmickBase.h"

class CInvisibleFloor : public CGimmickBase
{
	const char*	MAIN_MODEL_NAME = "InvisibleFloor";	// �d�l���f����.
	const float MODEL_SCALE = 0.08f;
	const float MODEL_ALPHA = 0.02f;
public:
	CInvisibleFloor( const stObjectInfo& objInfo );
	~CInvisibleFloor();

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
	CInvisibleFloor();
	// ����֐�.
	virtual void Action( shared_ptr<CObjectBase> pObj )	override;

	shared_ptr<CDX9Mesh>	m_pStaticMesh;
	float m_AlphaCount;
};

#endif	// #ifndef INVISIBLE_FLOOR_H.
