#include "CollisionRay.h"

#include "..\..\ObjectBase\ObjectBase.h"

CCollisionRay::CCollisionRay( float* pfYaw )
	: m_pfYaw	( pfYaw )
{
}

CCollisionRay::~CCollisionRay()
{
	m_pfYaw = nullptr;
}


// ���C�ƃ��b�V���̓����蔻��.
bool CCollisionRay::Intersect( shared_ptr<CObjectBase> pTargetObj,
	float* pfDistance,			// (out)����.
	D3DXVECTOR3* pvIntersect, 
	D3DXVECTOR3 endpos )	// (out)�������W.
{
	D3DXMATRIXA16 mAttackRot;	// ��]�s��.

	// ��]�s����v�Z.
	D3DXMatrixRotationY( &mAttackRot, *m_pfYaw );

	// ���x�N�g����p��.
	D3DXVECTOR3 vecAxisZ;
	// Z�x�N�g�����̂��̂����݂̉�]���ɂ��ϊ�����.
	D3DXVec3TransformCoord( &vecAxisZ, &m_vAxis, &mAttackRot );

	// ���C�̈ʒu.
	D3DXVECTOR3 vecStart, vecEnd;
	vecStart = vecEnd = m_vRay;

	// ���L�����̍��W�ɉ�]���W����������.
	if( endpos == D3DXVECTOR3(0.0f, 0.0f, 0.0f) ){
		vecEnd += vecAxisZ * 1.0f;
	} else {
		vecEnd = endpos;
	}

	// �Ώۂ������Ă��镨�̂ł��A�Ώۂ̃��[���h�s���.
	// �t�s���p����ΐ��������C��������.
	// �Ώۂ�. ܰ��ލs��A���ٍs��A��]�s��A���s�ړ��s��.
	D3DXMATRIX mWorld, mScale, mRotYPR, mTran;

	//�g��k���s��쐬.
	D3DXMatrixScaling(
		&mScale,	//(out)�v�Z����.
		pTargetObj->GetScale(),
		pTargetObj->GetScale(),
		pTargetObj->GetScale() );	//x,y,z���ꂼ��̊g�k�l.

	D3DXMATRIX mYaw, mPitch, mRoll;
	//Y����]�s��쐬.
	D3DXMatrixRotationY( &mYaw, pTargetObj->GetRotation().y );
	//X����]�s��쐬.
	D3DXMatrixRotationX( &mPitch, pTargetObj->GetRotation().x );
	//Z����]�s��쐬.
	D3DXMatrixRotationZ( &mRoll, pTargetObj->GetRotation().z );
	//��]�s����쐬.
	mRotYPR = mYaw * mPitch * mRoll;

	//���s�ړ��s��쐬.
	D3DXMatrixTranslation(
		&mTran,	//(out)�v�Z����.
		pTargetObj->GetPosition().x, pTargetObj->GetPosition().y, pTargetObj->GetPosition().z );	//x,y,z���W.


	//ܰ��ލs��쐬.
	mWorld = mScale * mRotYPR * mTran;
	D3DXMATRIX mBackWorld;
	// �t�s������߂�.
	D3DXMatrixInverse( &mBackWorld, nullptr, &mWorld );
	D3DXVec3TransformCoord( &vecStart, &vecStart, &mBackWorld );
	D3DXVec3TransformCoord( &vecEnd, &vecEnd, &mBackWorld );

	// ����.
	D3DXVECTOR3 vecDistance;
	vecDistance = vecEnd - vecStart;	// ���������߂�.

	BOOL bHit = FALSE;	// �����t���O.

	DWORD dwIndex = 0;	// �C���f�b�N�X�ԍ�.
	D3DXVECTOR3 vVertex[3];	// ���_���W.
	FLOAT U = 0.0F, V = 0.0F;	// (out)�d�S�q�b�g���W.

	// ���C�ƃ��b�V���̌����𒲂ׂ�.
	D3DXIntersect(
		pTargetObj->GetMeshData(),	// �Ώۃ��b�V��,
		&vecStart,				// �J�n�ʒu.
		&vecDistance,			// ���C�̋���.
		&bHit,					// (out)���茋��.
		&dwIndex,				// (out)bHIt��TRUE����.
								// ���C�̎n�_�ɍł��߂��̖ʂ̃C���f�b�N�X�l�ւ̃|�C���^.
		&U, &V,					// (out)�d�S�q�b�g���W.
		pfDistance,				// (out)�^�[�Q�b�g�Ƃ̋���.
		nullptr, nullptr );

	if( bHit == TRUE ){
		// ����������.
		FindVerticesOnPoly(
			pTargetObj->GetMeshData(), dwIndex, vVertex );
		// �d�S���W�����_���Z�o.
		// ���[�J����_�́A v0 + U * (v1-v0) + V * (v2-v0)�ŋ��܂�.
		*pvIntersect =
			vVertex[0]
			+ U * ( vVertex[1] - vVertex[0] )
			+ V * ( vVertex[2] - vVertex[0] );

		D3DXVec3TransformCoord( pvIntersect, pvIntersect, &mWorld );

		return true;
	}
	return false;
}

// �����ʒu�̃|���S���̒��_��������.
HRESULT CCollisionRay::FindVerticesOnPoly(
	LPD3DXMESH pMeshForRay, DWORD dwPolyIndex,
	D3DXVECTOR3* pVecVertices )	// (out)���_���W(���[�J�����W),
{
	// ���_���Ƃ̃o�C�g�����擾.
	DWORD dwStrid = pMeshForRay->GetNumBytesPerVertex();
	// ���_�����擾.
	DWORD dwVertexAmt = pMeshForRay->GetNumVertices();
	// �ʐ����擾.
	DWORD dwPolyAmt = pMeshForRay->GetNumFaces();

	WORD* pwPoly = nullptr;

	// �C���f�b�N�X�o�b�t�@�����b�N(�ǂݍ��݃��[�h).
	pMeshForRay->LockIndexBuffer(
		D3DLOCK_READONLY, (VOID**)&pwPoly );
	BYTE* pbVertices = nullptr;	// ���_(�o�C�g��).
	FLOAT* pfVerTices = nullptr;	// ���_(FLOAT�^).
	LPDIRECT3DVERTEXBUFFER9 VB = nullptr;	// ���_�o�b�t�@.
	pMeshForRay->GetVertexBuffer( &VB );	// ���_���̎擾.

	// ���_�o�b�t�@�̃��b�N.
	if( SUCCEEDED(
		VB->Lock( 0, 0, (VOID**)&pbVertices, 0 ) ) ){
		for( int i = 0; i < 3; i++ ){
			// �|���S���̒��_��3�ڂ��擾.
			pfVerTices
				= (FLOAT*)( &pbVertices[dwStrid*pwPoly[dwPolyIndex * 3 + i]] );
			pVecVertices[i].x = pfVerTices[0];
			pVecVertices[i].y = pfVerTices[1];
			pVecVertices[i].z = pfVerTices[2];
		}

		pMeshForRay->UnlockIndexBuffer();
		VB->Unlock();
	}
	SAFE_RELEASE( VB );

	return S_OK;
}