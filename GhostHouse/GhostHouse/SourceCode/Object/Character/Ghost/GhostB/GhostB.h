#ifndef GHOST_B_H
#define GHOST_B_H

#include "..\..\..\..\ObjectBase\CharacterBase\GhostBase\GhostBase.h"
#include "..\HitoDama\HitoDama.h"
#include "..\..\..\..\Resource\MeshResource\MeshResource.h"

class CDX9SkinMesh;

class CGhostB : public CGhostBase
{
private:
	const char*	MAIN_MODEL_NAME		= "GhostB_s";	// �d�l���f����.

	// �����Ȃ邩��. 
	const float INIT_POSITION_Y = 0.0f;

	const float MAX_WAIT_DISTANCE	= 3.0f;		// �ҋ@���̏㉺�̋���.
	const float MAX_SEARCH_DISTANCE	= 5.0f;		// �T�[�`�͈�.

	const float MODEL_SCALE				= 0.08f;	// ���f���̑傫��.
	const float SPHERE_COLLISION_RADIUS = 1.0f;		// �����蔻��p�̔��a�̑傫��.

public:
	CGhostB( const stObjectInfo& objInfo );
	virtual ~CGhostB();

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
	CGhostB(){};

	// ����֐�.
	virtual void Action( shared_ptr<CObjectBase> pObj )	override;
	void AnimInit();
	void AnimAdd();

private:
	// �S�[�X�gB����΂��l���N���X.
	unique_ptr<CHitoDama>			m_pHitoDama;

	shared_ptr<CDX9SkinMesh>		m_pSkinMesh;
	shared_ptr<CDX9SkinMesh::CHANGE_TEXTURES>	m_pTexture;

	LPD3DXANIMATIONCONTROLLER		m_pAc;

	double m_AnimSpd;
	double m_AnimSpeed;
	bool m_bChangeTexture;
};

#endif	// #ifndef GHOST_B_H.