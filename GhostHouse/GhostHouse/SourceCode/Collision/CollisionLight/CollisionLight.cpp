#include "CollisionLight.h"
#include "..\..\Resource\MeshResource\MeshResource.h"

//--------------------------.
// ���C�g���菈��.
//--------------------------.
bool CCollisionLight::isLightHit( shared_ptr<CObjectBase> pObj, D3DXVECTOR3 vMyPos, D3DXVECTOR3 fRot_y )
{
	float r = LIGHT_RADIUS;						// ���C�g�̔��a.

	D3DXVECTOR3 sayakaPos = vMyPos;
	sayakaPos.y += 1.0f;
	D3DXVECTOR3 lightPos = sayakaPos;		// ���C�g�̐�̍��W�p.
	D3DXMATRIX mAttackRot;	// ��]�s��.

	// ���C�g�̍��W�ɉ�]���W����������.
	lightPos += fRot_y * LIGHT_DISTANCE;

	D3DXVECTOR3 vTarPosToMyPos		= D3DXVector3Destance( pObj->GetPosition(), sayakaPos );
	D3DXVECTOR3 vLightPosToMyPos	= D3DXVector3Destance( lightPos, sayakaPos );

	float h = fabsf( D3DXVector3Size( sayakaPos ) - D3DXVector3Size( lightPos ) );
	float vDot = D3DXVec3Dot( &vTarPosToMyPos, &vLightPosToMyPos );

	D3DXVECTOR3 vCloss;
	D3DXVec3Cross( &vCloss, &vTarPosToMyPos, &vLightPosToMyPos );

	if( ( 0.0f <= vDot && vDot <= h*h ) &&
		( fabsf( D3DXVector3Size(vCloss) ) <= fabsf( D3DXVector3Size(vTarPosToMyPos) ) * r*h / sqrtf(r*r + h*h) ) ){
		return true;
	}
	return false;
	
}

//--------------------------.
// ��̋������v�Z.
//--------------------------.
D3DXVECTOR3 CCollisionLight::D3DXVector3Destance( D3DXVECTOR3 vec, D3DXVECTOR3 vec2 )
{
	D3DXVECTOR3 c;

	c.y = vec.y - vec2.y;
	c.z = vec.z - vec2.z;
	c.x = vec.x - vec2.x;

	return c;
}

//--------------------------.
// �x�N�^�[�̃T�C�Y���擾.
//--------------------------.
float CCollisionLight::D3DXVector3Size( D3DXVECTOR3 vec )
{
	return sqrtf( powf( vec.x, 2 ) + powf( vec.y, 2 ) + powf( vec.z, 2 ) );
}