#ifndef HITO_DAMA_H
#define HITO_DAMA_H

#include "..\..\..\..\ObjectBase\CharacterBase\GhostBase\GhostBase.h"

class CHitoDama : public CGhostBase
{
private:
	const char*		MAIN_MODEL_NAME = "HitoDama";	// �d�l���f����.

	const float		MOVE_SPEED		= 0.04f;	// �ړ����x.
	const float		MAX_SHOT_COUNT	= 40.0f;	// �V���b�g�J�E���g�ő吔.
	const float		MODEL_SCALE		= 0.08f;	// ���f���̑傫��.
	const float		SPHERE_COLLISION_RADIUS = 0.05f;		// �����蔻��p�̔��a�̑傫��.
	const D3DXVECTOR3 WND_OUT_PISITION = { 100.0f, -100.0f, 100.0f };
public:
	CHitoDama();
	virtual ~CHitoDama();

	// �X�V�֐�.
	virtual void Update( shared_ptr<CObjectBase> pObj )	override;
	// �`��֐�.
	virtual void Render( D3DXMATRIX& mView, D3DXMATRIX& mProj,
		Light& stLight, stCAMERA& stCamera )	override;
	// �����蔻��p�֐�.
	virtual void Collision( shared_ptr<CObjectBase> pObj ) override;
	// ���[�h�֐�.
	virtual void Load( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 )	override;
	// �e�̃Z�b�g.
	void SetShot( const D3DXVECTOR3& vPos );

	bool IsAction(){ return m_bMoveFlag; }

	void GhostBLink( CObjectBase* pObj ){ m_pGhostB = pObj; }

	virtual LPD3DXMESH GetMeshData() override;

private:
	// ����֐�.
	virtual void Action( shared_ptr<CObjectBase> pObj )	override;
	// �ҋ@�֐�.
	virtual void IdleState()	override;

	// �����菈��.
	void HitProc( shared_ptr<CObjectBase> pObj );

	shared_ptr<CDX9Mesh>	m_pStaticMesh;

	CObjectBase* m_pGhostB;

	// �V���b�g�J�E���g.
	float	m_fShotCount;
	float	m_fScaleChangeCount;

	D3DXVECTOR3 m_vPos;
	float		m_fColl_y;
};

#endif	// #ifndef HITO_DAMA_H.
