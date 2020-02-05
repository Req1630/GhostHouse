#include "CollisionBox.h"
#include "..\..\Resource\ModelResource\DX9Mesh\CDX9Mesh.h"


CCollisionBox::CCollisionBox()
	: m_pvPosition	( nullptr )
{
}

CCollisionBox::CCollisionBox( D3DXVECTOR3* pvPos )
	: m_pvPosition	( pvPos )
{
}

CCollisionBox::~CCollisionBox()
{
	m_pvPosition	= nullptr;
}

//------------------------------.
// ���菈��.
//------------------------------.
bool CCollisionBox::isCollision( CCollisionBox* collBox )
{
	// �U������2�̒��_��p�ӂ�����������.
	D3DXVECTOR3 vecMaxAtk, vecMinAtk;	// �ő�,�ŏ����_.
	vecMaxAtk = m_vPositionMax + *m_pvPosition;
	vecMinAtk = m_vPositionMin + *m_pvPosition;

	// �ڕW����2�̒��_��p�ӂ�����������.
	D3DXVECTOR3 vecMaxTrg, vecMinTrg;	// �ő�,�ŏ����_.
	vecMaxTrg = collBox->GetPositionMax() + collBox->GetPosition();
	vecMinTrg = collBox->GetPositionMin() + collBox->GetPosition();

	// 2�̕��̂��ꂼ��̍ő�,�ŏ��ʒu�Ƃ���.
	// 4�̏�񂪂���΁A�Փ˂����o�ł���.
	if( vecMinAtk.x < vecMaxTrg.x && vecMaxAtk.x > vecMinTrg.x &&
		vecMinAtk.y < vecMaxTrg.y && vecMaxAtk.y > vecMinTrg.y &&
		vecMinAtk.z < vecMaxTrg.z && vecMaxAtk.z > vecMinTrg.z ){
		return true;
	}
	return false;
}

//------------------------------.
// �o�E���f�B���O�{�b�N�X�쐬.
//------------------------------.
HRESULT CCollisionBox::InitBBox( LPD3DXMESH pMesh )
{
	LPDIRECT3DVERTEXBUFFER9 pVB = nullptr;
	VOID* pVertices = nullptr;
	D3DXVECTOR3	Max, Min;

	if( FAILED( pMesh->GetVertexBuffer( &pVB ) ) ){
		_ASSERT_EXPR( false, L"���_�o�b�t�@�擾���s" );
		return E_FAIL;
	}
	// ���b�V���̒��_�o�b�t�@�����b�N����.
	if( FAILED( pVB->Lock( 0, 0, &pVertices, 0 ) ) ){
		_ASSERT_EXPR( false, L"���_�o�b�t�@�̃��b�N���s" );
		SAFE_RELEASE( pVB );
		return E_FAIL;
	}
	// ���b�V�����̒��_�ʒu�̍ő�ƍŏ�����������.
	D3DXComputeBoundingBox(
		static_cast<D3DXVECTOR3*>( pVertices ),
		pMesh->GetNumVertices(),
		D3DXGetFVFVertexSize(
			pMesh->GetFVF() ),
		&Min, &Max );	// (out)�ŏ�,�ő咸�_.

	pVB->Unlock();		// �A�����b�N.
	SAFE_RELEASE( pVB );	// �g�p�ςݒ��_�o�b�t�@�̉��.

	m_vPositionMin = Min;
	m_vPositionMax = Max;

	m_vLength.x = ( Max.x - Min.x ) / 2.0f;
	m_vLength.y = ( Max.y - Min.y ) / 2.0f;
	m_vLength.z = ( Max.z - Min.z ) / 2.0f;

	return S_OK;
}