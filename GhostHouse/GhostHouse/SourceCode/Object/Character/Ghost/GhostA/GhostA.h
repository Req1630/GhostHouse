#ifndef GHOST_A_H
#define GHOST_A_H

#include "..\..\..\..\ObjectBase\CharacterBase\GhostBase\GhostBase.h"
#include "..\..\..\..\Resource\MeshResource\MeshResource.h"
class CGhostA : public CGhostBase
{
private:
	const char*	MAIN_MODEL_NAME		= "GhostA";	// �d�l���f����.

	// �����Ȃ邩��. 
	const float INIT_POSITION_Y		= 0.0f;		

	const float MOVE_SPEED				= 0.03f;	// �ړ����x.
	const float MAX_WAIT_DISTANCE		= 3.0f;		// �ҋ@���̏㉺�̋���.
	const float MIN_SEARCH_DISTANCE		= 5.0f;		// �ǔ��O�̃T�[�`�͈�.
	const float MAX_SEARCH_DISTANCE		= 6.0f;		// �ǔ���̃T�[�`�͈�.
	const float MODEL_SCALE				= 0.08f;	// ���f���̑傫��.
	const float SPHERE_COLLISION_RADIUS = 1.0f;		// �����蔻��p�̔��a�̑傫��.

public:
	CGhostA( const stObjectInfo& objInfo );
	virtual ~CGhostA();

	// �X�V�֐�.
	virtual void Update( shared_ptr<CObjectBase> pObj )	override;
	// �`��֐�.
	virtual void Render( D3DXMATRIX& mView, D3DXMATRIX& mProj,
		Light& stLight, stCAMERA& stCamera )	override;
	// �����蔻��p�֐�.
	virtual void Collision( shared_ptr<CObjectBase> pObj ) override;
	virtual void Load( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 )	override;
	virtual LPD3DXMESH	GetMeshData() override;

private:
	CGhostA(){};

	// ����֐�.
	virtual void Action( shared_ptr<CObjectBase> pObj )	override;
	// ���G�͈͂̓����蔻�菈��.
	bool SearchCollision( shared_ptr<CObjectBase> pObj );

	shared_ptr<CDX9Mesh>	m_pStaticMesh;

	shared_ptr<CDX9Mesh::CHANGE_TEXTURES>	m_pTexture;

	bool m_bChangeTexture;
};

#endif	// #ifndef GHOST_A_H.