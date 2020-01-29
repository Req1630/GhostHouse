#ifndef INVISIBLE_START_FLOOR_H
#define INVISIBLE_START_FLOOR_H

#include "..\..\..\ObjectBase\GimmickBase\GimmickBase.h"
#include "..\..\..\DebugMode\Sphere\Sphere.h"

class CInvisibleStartFloor : public CGimmickBase
{
	const char*	MAIN_MODEL_NAME = "InvisibleStartFloor";	// �d�l���f����.
	const float MODEL_SCALE = 0.08f;
	const float MODEL_ALPHA = 1.0f;
	const float COLLISION_RADIUS = 0.1f;
public:
	CInvisibleStartFloor( const stObjectInfo& objInfo );
	~CInvisibleStartFloor();

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
	CInvisibleStartFloor();
	// ����֐�.
	virtual void Action( shared_ptr<CObjectBase> pObj )	override;

	shared_ptr<CDX9Mesh>	m_pStaticMesh;
	int m_State;
	bool m_isNowHit;
	bool m_isOlsHit;
};

#endif	// #ifndef INVISIBLE_START_FLOOR_H.