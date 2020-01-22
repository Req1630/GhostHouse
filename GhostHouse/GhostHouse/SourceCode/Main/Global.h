#ifndef GLOBAL_H
#define GLOBAL_H

// �x���ɂ��ẴR�[�h���͂𖳌��ɂ��� 4005:�Ē�`.
#pragma warning( disable:4005 )

#include <Windows.h>
#include <memory>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#include "MyMacro.h"

#include <d3dx9.h>
#include <D3DX11.h>
#include <D3D11.h>
#include <D3DX10.h>	// �uD3DX�`�v�̒�`�g�p�ɕK�v.
#include <D3D10.h>

#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "d3dx9.lib" )
#pragma comment( lib, "d3dx11.lib" )
#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "d3dx10.lib" )

using namespace std;

//=================================================
//	�萔.
//=================================================
//#define WND_W 1280	//���萔�錾��#define�͎g��Ȃ�.
const int WND_W = 1280;	//����޳�̕�.
const int WND_H = 720;	//����޳�̍���.

struct stCAMERA
{
	D3DXVECTOR3 Pos;	// �ʒu.
	D3DXVECTOR3 vLook;	// �����ʒu.
	D3DXVECTOR3 vRot;	// ��].
	float		fDegree;// �p�x.
	float		fYaw;	// Y����].
	float		fLength;//�����W(����).
	D3DXMATRIX	mRot;	// ��]�s��.
	D3DXVECTOR3 InvisibleCenter;	//�����Ȃ����̒��S.
	D3DXVECTOR3 InvisibleCollisionPos;	//�����Ȃ����M�~�b�N�쓮�̂��߂̓����蔻��̈ʒu.
};

struct Light
{
	Light()
	{
		ZeroMemory( this, sizeof( Light ) );
	}
	D3DXVECTOR3 pos;
	float range;
	D3DXVECTOR3 dir;
	float cone;
	D3DXVECTOR3 att;
	D3DXVECTOR4 ambient;
	D3DXVECTOR4 diffuse;
	D3DXMATRIX Rot;
	D3DXVECTOR4 Color;
};

#endif	// #ifndef GLOBAL_H.