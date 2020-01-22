#ifndef UP_DOWN_FLOOR_H
#define UP_DOWN_FLOOR_H

#include "..\..\..\ObjectBase\GimmickBase\GimmickBase.h"

#include "..\..\..\DebugMode\DebugText\DebugText.h"

class CUpDownFloor : public CGimmickBase
{
	const char*	MAIN_MODEL_NAME = "UpDownFloor";	// �d�l���f����.
	const float MODEL_SCALE		= 0.08f;
	const float RADIUS			= 0.008f;
	const int	MAX_WAIT_TIME	= 100;				// �ҋ@����,

public:
	CUpDownFloor( const stObjectInfo& objInfo );
	~CUpDownFloor();
	
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

	shared_ptr<CDX9Mesh>	m_pStaticMesh;
	// �ړ��p�x(�㉺).
	float	m_fAngleCount;
	// �ҋ@�J�E���g.
	int		m_iWaitCount;
};

#endif	// #ifndef UP_DOWN_FLOOR_H.