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


//--------------------.
// ���菈��.
//--------------------.
bool CCollisionSphere::isCollision( CCollisionSphere* collBox )
{
	//�Q�̕��̂̒��S��(�Q�_��)�̋��������߂�.
	D3DXVECTOR3 vLength
		= collBox->GetPosition() - *m_pvPosition;
	// �����ɕϊ�����.
	float Length = D3DXVec3Length( &vLength );

	// �Q�_�Ԃ̋������A���ꂼ��̕��̂̔��a�𑫂������̂��.
	// �������Ƃ������Ƃ́A�X�t�B�A���m���d�Ȃ��Ă���(�Փ˂��Ă���)�Ƃ�������.
	if( Length <= m_fRadius + collBox->GetRadius() ){
		return true;	// �Փ�.
	}
	return false;	// �Փ˂��Ă��Ȃ�.
}

//--------------------.
// �X�t�B�A������.
//--------------------.
HRESULT CCollisionSphere::InitSphere( LPD3DXMESH pMesh )
{
	LPDIRECT3DVERTEXBUFFER9 pVB = nullptr; // ���_�o�b�t�@.
	void* pVertices = nullptr;	// ���_.
	D3DXVECTOR3 vCenter;		// ���S.

	// ���_�o�b�t�@���擾.
	if( FAILED(
		pMesh->GetVertexBuffer( &pVB ) ) ){
		return E_FAIL;
	}
	// ���b�V���̒��_�o�b�t�@�����b�N����.
	if( FAILED(
		pVB->Lock( 0, 0, &pVertices, 0 ) ) ){
		SAFE_RELEASE( pVB );
		return E_FAIL;
	}
	// ���b�V���̊O�ډ~�̒��S�Ɣ��a���v�Z����.
	D3DXComputeBoundingSphere(
		static_cast<D3DXVECTOR3*>( pVertices ),
		pMesh->GetNumVertices(),				// ���_�̐�.
		D3DXGetFVFVertexSize( pMesh->GetFVF() ),// ���_�̏��.
		&vCenter,		// (out)���S���W.
		&m_fRadius );	// (out)���a.

	// �A�����b�N.
	pVB->Unlock();
	SAFE_RELEASE( pVB );

	return S_OK;
}
