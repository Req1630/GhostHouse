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
	LPDIRECT3DVERTEXBUFFER9 pVB = nullptr;//頂点ﾊﾞｯﾌｧ.
	void* pVertices = nullptr;	//頂点.
	D3DXVECTOR3 vCenter;		//中心.

	//頂点ﾊﾞｯﾌｧを取得.
	if( FAILED(
		pMesh->GetVertexBuffer( &pVB ) ) ){
		return E_FAIL;
	}
	//ﾒｯｼｭの頂点ﾊﾞｯﾌｧをﾛｯｸする.
	if( FAILED(
		pVB->Lock( 0, 0, &pVertices, 0 ) ) ){
		SAFE_RELEASE( pVB );
		return E_FAIL;
	}
	//ﾒｯｼｭの外接円の中心と半径を計算する.
	D3DXComputeBoundingSphere(
		static_cast<D3DXVECTOR3*>( pVertices ),
		pMesh->GetNumVertices(),	//頂点の数.
		D3DXGetFVFVertexSize( pMesh->GetFVF() ),//頂点の情報.
		&vCenter,	//(out)中心座標.
		&m_fRadius );	//(out)半径.

	//ｱﾝﾛｯｸ.
	pVB->Unlock();
	SAFE_RELEASE( pVB );

	return S_OK;
}

bool CCollisionSphere::isCollision( CCollisionSphere* collBox )
{
	//２つの物体の中心間(２点間)の距離を求める.
	D3DXVECTOR3 vLength
		= collBox->GetPosition() - *m_pvPosition;
	//長さに変換する.
	float Length = D3DXVec3Length( &vLength );

	//２点間の距離が、それぞれの物体の半径を足したものより.
	//小さいということは、ｽﾌｨｱ同士が重なっている(衝突している)ということ.
	if( Length <=
		m_fRadius + collBox->GetRadius() ){
		return true;	//衝突.
	}
	return false;	//衝突していない.
}

void CCollisionSphere::InitDebug( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 )
{
}
void CCollisionSphere::DebugRender( D3DXMATRIX& mView, D3DXMATRIX& mProj )
{
}