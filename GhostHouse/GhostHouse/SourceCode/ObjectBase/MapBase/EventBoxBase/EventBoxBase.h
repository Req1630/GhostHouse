#ifndef EVENT_BOX_BASE_H
#define EVENT_BOX_BASE_H

#include "..\MapBase.h"
#include "..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\DebugMode\Box\Box.h"

class CEventBoxBase : public CMapBase
{
public:
	const float MODEL_SCALE = 0.08f;
public:
	CEventBoxBase()
		: m_pStaticMesh	( nullptr )
		, m_pBox		( make_unique<CBox>() )
	{}
	virtual ~CEventBoxBase(){}

	// �X�V�֐�.
	virtual void Update( shared_ptr<CObjectBase> pObj ) override
	{}

	// �`��֐�.
	virtual void Render( D3DXMATRIX& mView, D3DXMATRIX& mProj, Light& stLight, stCAMERA& stCamera ) override
	{
		if( m_pStaticMesh == nullptr )	return;

//#ifdef _DEBUG
//		m_pStaticMesh->SetPosition( m_vPosition );	// ���W����`��p�ɃZ�b�g.
//		m_pStaticMesh->SetRotation( m_vRotation );	// ��]����`��p�ɃZ�b�g.
//		m_pStaticMesh->SetScale( MODEL_SCALE );
//		m_pStaticMesh->SetAlpha( m_fAlpha );		// �A���t�@�l���Z�b�g.
//		m_pStaticMesh->SetAlpha( true );			// �A���t�@�`�悳���Ȃ�.
//		m_pStaticMesh->Render( mView, mProj, stLight, stCamera );	// �`��.
//		m_pStaticMesh->SetAlpha( false );
//#endif	// #ifdef _DEBUG.
//
//#ifdef _DEBUG
//		m_pBox->SetPosition( m_vPosition );
//		m_pBox->Render( mView, mProj );
//#endif	// #ifdef _DEBUG.
	}

	// �����蔻��p�֐�.
	virtual void Collision( shared_ptr<CObjectBase> pObj ) override
	{}

	// �ǂݍ��݊֐�.
	virtual void Load( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 ) override
	{
		// ���f���̓ǂݍ���.
		if( m_pStaticMesh == nullptr ){
			m_pStaticMesh = CMeshResorce::GetStatic( m_ObjName );
			if( m_pStaticMesh != nullptr ){
				m_pCollision->Init( m_pStaticMesh->GetMesh() );
				m_pCollision->GetSphere()->SetRadius( 2.0f );
				m_pCollision->GetBox()->BoxScaleSet( m_fScale );
				m_pBox->Init(
					pDevice11, pContext11, 
					m_pCollision->GetBox()->GetPositionMax(),
					m_pCollision->GetBox()->GetPositionMin());
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
	unique_ptr<CBox>		m_pBox;
};

#endif	// #ifndef EVENT_BOX_BASE_H.