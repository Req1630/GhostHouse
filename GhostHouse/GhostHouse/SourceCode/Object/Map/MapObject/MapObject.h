#ifndef MAP_OBJECT_H
#define MAP_OBJECT_H

#include "..\..\..\ObjectBase\MapBase\MapBase.h"
#include "..\..\Resource\MeshResource\MeshResource.h"

class CMapObject : public CMapBase
{
	const float MODEL_ALPHA = 1.0f;
	const float MODEL_SCALE = 0.08f;
public:
	CMapObject( const stObjectInfo& objInfo )
	{
		SetObjectInfo( objInfo );
		m_pCollision = make_shared<CCollisionManager>( &m_vPosition, &m_vRotation.y );
		m_fScale = MODEL_SCALE;
	}
	virtual ~CMapObject()
	{
	};

	// �X�V�֐�.
	virtual void Update( shared_ptr<CObjectBase> pObj )	override
	{
	}
	// �`��֐�.
	virtual void Render( D3DXMATRIX& mView, D3DXMATRIX& mProj,
		Light& stLight, stCAMERA& stCamera )	override
	{
		if( m_pStaticMesh == nullptr )	return;

		m_pStaticMesh->SetPosition( m_vPosition );	// ���W����`��p�ɃZ�b�g.
		m_pStaticMesh->SetRotation( m_vRotation );	// ��]����`��p�ɃZ�b�g.
		m_pStaticMesh->SetAlpha( MODEL_ALPHA );		// �A���t�@�l���Z�b�g.
		m_pStaticMesh->SetScale( MODEL_SCALE );		// �T�C�Y�̃Z�b�g.
		m_pStaticMesh->SetCoverage( true );
		m_pStaticMesh->SetCullBackFlag( true );
		m_pStaticMesh->Render( mView, mProj, stLight, stCamera );	// �`��.
		m_pStaticMesh->SetCullBackFlag( false );
		m_pStaticMesh->SetCoverage( false );
	}
	// �����蔻��p�֐�.
	virtual void Collision( shared_ptr<CObjectBase> pObj ) override
	{
	}

	virtual void Load( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 )	override
	{
		// ���f���̓ǂݍ���.
		if( m_pStaticMesh == nullptr ){
			m_pStaticMesh = CMeshResorce::GetStatic( m_ObjName );
			if( m_pStaticMesh != nullptr ){
				m_pCollision->Init( m_pStaticMesh->GetMesh() );
			}
		}
	}
	virtual LPD3DXMESH GetMeshData() override
	{
		if( m_pStaticMesh == nullptr ) return nullptr;
		return m_pStaticMesh->GetMesh();
	}
private:
	shared_ptr<CDX9Mesh>	m_pStaticMesh;
};

#endif	// #ifndef MAP_OBJECT_H.