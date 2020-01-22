#include "CollisionSphere.h"
#include "..\..\Resource\ModelResource\DX9Mesh\CDX9Mesh.h"

CCollisionSphere::CCollisionSphere( D3DXVECTOR3* pvPosition )
	: m_pvPosition		( pvPosition )
	, m_fRadius			( 0.0f )
{
}

CCollisionSphere::~CCollisionSphere()
{
	m_pvPosition	= nullptr;
}

HRESULT CCollisionSphere::InitSphere( LPD3DXMESH pMesh )
{
	LPDIRECT3DVERTEXBUFFER9 pVB = nullptr;//���_�ޯ̧.
	void* pVertices = nullptr;	//���_.
	D3DXVECTOR3 vCenter;		//���S.

	//���_�ޯ̧���擾.
	if( FAILED(
		pMesh->GetVertexBuffer( &pVB ) ) ){
		return E_FAIL;
	}
	//ү���̒��_�ޯ̧��ۯ�����.
	if( FAILED(
		pVB->Lock( 0, 0, &pVertices, 0 ) ) ){
		SAFE_RELEASE( pVB );
		return E_FAIL;
	}
	//ү���̊O�ډ~�̒��S�Ɣ��a���v�Z����.
	D3DXComputeBoundingSphere(
		static_cast<D3DXVECTOR3*>( pVertices ),
		pMesh->GetNumVertices(),	//���_�̐�.
		D3DXGetFVFVertexSize( pMesh->GetFVF() ),//���_�̏��.
		&vCenter,	//(out)���S���W.
		&m_fRadius );	//(out)���a.

	//��ۯ�.
	pVB->Unlock();
	SAFE_RELEASE( pVB );

	return S_OK;
}

bool CCollisionSphere::isCollision( CCollisionSphere* collBox )
{
	//�Q�̕��̂̒��S��(�Q�_��)�̋��������߂�.
	D3DXVECTOR3 vLength
		= collBox->GetPosition() - *m_pvPosition;
	//�����ɕϊ�����.
	float Length = D3DXVec3Length( &vLength );

	//�Q�_�Ԃ̋������A���ꂼ��̕��̂̔��a�𑫂������̂��.
	//�������Ƃ������Ƃ́A�̨����m���d�Ȃ��Ă���(�Փ˂��Ă���)�Ƃ�������.
	if( Length <=
		m_fRadius + collBox->GetRadius() ){
		return true;	//�Փ�.
	}
	return false;	//�Փ˂��Ă��Ȃ�.
}

void CCollisionSphere::InitDebug( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 )
{
}
void CCollisionSphere::DebugRender( D3DXMATRIX& mView, D3DXMATRIX& mProj )
{
}