#ifndef BIG_GHOST_H
#define BIG_GHOST_H

#include "..\..\..\..\ObjectBase\CharacterBase\GhostBase\GhostBase.h"


class CDX9SkinMesh;
class CDX9Mesh;

class CBigGhost : public CGhostBase
{
private:
	const float MOVE_SPEED				= 0.05f;	// �ړ����x.
	const float MAX_WAIT_DISTANCE		= 3.0f;		// �ҋ@���̏㉺�̋���.
	const float MAX_ALPHA				= 2.0f;		// �ő�A���t�@�l.
	const float ATTENUATION_VALUE_ALPH	= 0.02f;	// �A���t�@�̌����l.
	const float MODEL_SCALE				= 0.16f;	// ���f���̑傫��.
	const double ANIM_SPEED				= 0.02;
	const char*	MAIN_MODEL_NAME			= "BigGhost_s";	// �d�l���f����.

public:
	CBigGhost( const stObjectInfo& objInfo );
	~CBigGhost();

	// �X�V�֐�.
	virtual void Update( shared_ptr<CObjectBase> pObj )	override;
	// �`��֐�.
	virtual void Render( D3DXMATRIX& mView, D3DXMATRIX& mProj,
		Light& stLight, stCAMERA& stCamera )	override;
	// �����蔻��p�֐�.
	virtual void Collision( shared_ptr<CObjectBase> pObj ) override;
	virtual void Load( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 )	override;

	// �X�^�e�B�b�N���b�V�����擾�֐�.
	virtual LPD3DXMESH	GetMeshData() override;

private:
	CBigGhost(){};
	shared_ptr<CDX9SkinMesh>	m_pSkinMesh;
	shared_ptr<CDX9Mesh>		m_pCollisionBoxMesh;

	// ����֐�.
	virtual void Action( shared_ptr<CObjectBase> pObj )	override;
	// �t�F�[�h����.
	virtual float FadeChange() override;

	bool m_bEndFlag;

	double m_bAnimSpeed;
};

#endif	// #ifndef BIG_GHOST_H.