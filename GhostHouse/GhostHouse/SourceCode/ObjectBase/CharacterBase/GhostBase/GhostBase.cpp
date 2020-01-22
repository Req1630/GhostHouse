#include "GhostBase.h"
#include "..\..\Resource\MeshResource\MeshResource.h"

CGhostBase::CGhostBase()
	: m_bMoveFlag		( false )
	, m_fWaitDistance	( 0.0f )
	, m_bAlphaFlag		( false )
	, m_fCount			( 0 )
	, m_pSearchRange	( nullptr )
	, m_iAngle			( 180 )
{
}

// �ҋ@�֐�.
void CGhostBase::IdleState()
{
	// sin�g�ŏ㉺�Ɉړ�����.
	m_vPosition.y += sinf( D3DXToRadian( m_iAngle ) ) * RADIUS * m_fWaitDistance;
	// �p�x��360�ȏ�ɂȂ�����0�ɖ߂�.
	if( m_iAngle >= 360 )	m_iAngle = 0;
	m_iAngle++;	// �p�x�𑝂₷.
}

// ���ݓ���֐�.
float CGhostBase::FadeChange()
{
	if( m_bAlphaFlag == true || 
		GetBitFlag( BitFlag::isHit ) == true ){
		m_fCount += 2;
		if( m_fCount >= 360 ){
			m_bAlphaFlag = false;
			BitFlagOFF( BitFlag::isHit );
			m_fCount = 0;
		}
		return fabsf(cosf( m_fCount * 0.03f ))+0.5f;
	} else {
		return 1.0f;
	}
}

// ���G�͈͂̓����蔻�菈��.
bool CGhostBase::SearchCollision( shared_ptr<CObjectBase> pObj )
{
	if( m_pSearchRange->isCollision( pObj->GetSphere() ) == true ){
		m_bMoveFlag = true;
		return true;
	} else{
		m_bMoveFlag = false;
		return false;
	}
	return false;
}

// �I�u�W�F�N�g�Ƃ̓����蔻��֐�.
bool CGhostBase::ObjSphereCollision( shared_ptr<CObjectBase> pObj )
{
	if( pObj->GetBitFlag( BitFlag::isHit ) == true ) return false ;
	if( pObj->GetBitFlag( BitFlag::isInvincible ) == true ) return false;
	if( pObj->GetBitFlag( BitFlag::isDead ) == true ) return false;
	if( GetBitFlag( BitFlag::isHit ) == true ) return false;
	if( GetBitFlag( BitFlag::isInvincible ) == true ) return false;

	// �����蔻��.
	if( m_pCollision->isSphereCollision( pObj->GetSphere() ) == true ){
		pObj->BitFlagON( BitFlag::isHit );
		return true;
	} else {
		pObj->BitFlagOFF( BitFlag::isHit );
		return false;
	}
	return false;
}

// 2�_�̋����𑪂�֐�.
float CGhostBase::GetTwoDistance( const D3DXVECTOR3& vMyPos, const D3DXVECTOR3& vtargetPos )
{
	float distance = sqrtf( ( vtargetPos.x - vMyPos.x ) * ( vtargetPos.x - vMyPos.x ) 
		+ ( vtargetPos.z - vMyPos.z ) * ( vtargetPos.z - vMyPos.z ) );

	return distance;
}

// 2�_�̊p�x�𑪂�֐�.
float CGhostBase::GetTwoRadian( const D3DXVECTOR3& vMyPos, const D3DXVECTOR3& vtargetPos )
{
	float radian = atan2f( vtargetPos.x - vMyPos.x, vtargetPos.z - vMyPos.z );

	return radian;
}

D3DXVECTOR3 CGhostBase::AxisZProc( float y )
{
	// ���x�N�g����p��.
	D3DXVECTOR3 vecAxisZ( 0.0f, 0.0f, 1.0f );	// �ϊ��O�x�N�g��.

	D3DXMATRIX mYaw;	// Y����]�s��.

	// Y����].
	D3DXMatrixRotationY( &mYaw, y );

	// Z���x�N�g�����̂��̂���]��Ԃɂ��ϊ�����.
	D3DXVec3TransformCoord( &vecAxisZ,	// (out)�ԊҌ�x�N�g��.
		&vecAxisZ, &mYaw );

	return vecAxisZ;
}