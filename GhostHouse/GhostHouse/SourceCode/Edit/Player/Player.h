#ifndef PLAYER_H
#define PLAYER_H

#include "..\..\ObjectBase\ObjectBase.h"
#include "..\..\Camera\Camera.h"

class CPlayer : public CObjectBase
{

public:
	CPlayer();
	virtual ~CPlayer();

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
	// �I�u�W�F�N�g�̍��W�擾�֐�.
	D3DXVECTOR3 GetObjectPosition() const { return m_vbufPosition; }
	// ����֐�.
	void Control();

private:
	// �X�e�B�b�N�̌X���擾.
	bool StickInclination( float divideValue );
	// ���C�ł̓����蔻��.
	void RayHits( shared_ptr<CObjectBase> pObj );

	shared_ptr<CDX9Mesh>		m_pStaticMesh;	// ���b�V���N���X.
	unique_ptr<CCollisionRay>	m_pCollRay;		// ���C�̓����蔻��.

	
	shared_ptr<CDX9Mesh>	m_pbufObject;	// �ړ����̉��I�u�W�F�N�g.
	D3DXVECTOR3				m_vbufPosition;	// ���I�u�W�F�N�g�̍��W.

	shared_ptr<CCamera>		m_pCamera;	// �J�����N���X.

	bool m_isMove;
	int m_StickCount;

	float		m_fRadian;
	D3DXVECTOR2 m_vMove;
	
	D3DXMATRIX	m_mView;

};

#endif	// #ifndef PLAYER_H.