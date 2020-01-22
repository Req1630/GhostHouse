#ifndef KEY_H
#define KEY_H

#include "..\..\..\ObjectBase\GimmickBase\GimmickBase.h"
#include "..\..\..\Resource\EffectResource\EffectResource.h"

class CKey : public CGimmickBase
{
	const char*	MAIN_MODEL_NAME = "Key";	// �d�l���f����.
	const float MODEL_SCALE = 0.08f;
	const float SPHERE_COLLISION_RADIUS = 1.0f;		// �����蔻��p�̔��a�̑傫��.
	const D3DXVECTOR3 WND_OUT = { 100.0f, -100.0f, 100.0f };
public:
	CKey( const stObjectInfo& objInfo );
	~CKey();

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
	// ����֐�.
	virtual void Action( shared_ptr<CObjectBase> pObj )	override;

private:
	shared_ptr<CDX9Mesh>	m_pStaticMesh;
	shared_ptr<CEffect>		m_pEffect;
	CEffect::Handle			m_Handle;
};

#endif	// #ifndef KEY_H.