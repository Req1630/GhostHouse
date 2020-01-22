#ifndef COLLISION_LIGHT_H
#define COLLISION_LIGHT_H

#include "..\..\Main\Global.h"
#include "..\..\ObjectBase\ObjectBase.h"

class CDX9Mesh;

class CCollisionLight
{
	const float LIGHT_RADIUS	= 3.7f;		// ライトの半径.
	const float LIGHT_DISTANCE	= 7.0f;	// ライトの距離.
public:
	CCollisionLight()
		: mesh	( nullptr )
	{}
	~CCollisionLight(){}

	bool isLightHit( shared_ptr<CObjectBase> pObj, D3DXVECTOR3 vMyPos, D3DXVECTOR3 fRot_y );
	void Render( D3DXMATRIX& mView, D3DXMATRIX& mProj,
		Light& Light, stCAMERA& stCamera );
private:
	D3DXVECTOR3 D3DXVector3Destance( D3DXVECTOR3 vec, D3DXVECTOR3 vec2 );
	float D3DXVector3Size( D3DXVECTOR3 vec );

	shared_ptr<CDX9Mesh> mesh;
	D3DXVECTOR3 SayakaPos;
	D3DXVECTOR3 lightPos;
};

#endif	// #ifndef COLLISION_LIGHT_H.
