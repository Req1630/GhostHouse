#ifndef COLLISION_LIGHT_H
#define COLLISION_LIGHT_H

#include "..\..\Main\Global.h"
#include "..\..\ObjectBase\ObjectBase.h"

class CDX9Mesh;

class CCollisionLight
{
	const float LIGHT_RADIUS	= 3.7f;	// ���C�g�̔��a.
	const float LIGHT_DISTANCE	= 7.0f;	// ���C�g�̋���.
public:
	CCollisionLight(){}
	~CCollisionLight(){}

	// ���C�g���菈��.
	bool isLightHit( shared_ptr<CObjectBase> pObj, D3DXVECTOR3 vMyPos, D3DXVECTOR3 fRot_y );

private:
	// ��̋������v�Z.
	D3DXVECTOR3 D3DXVector3Destance( D3DXVECTOR3 vec, D3DXVECTOR3 vec2 );
	// �x�N�^�[�̃T�C�Y���擾.
	float D3DXVector3Size( D3DXVECTOR3 vec );
};

#endif	// #ifndef COLLISION_LIGHT_H.
