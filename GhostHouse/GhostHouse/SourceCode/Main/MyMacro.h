#pragma once

//===========================================================
//	ϸ�.
//===========================================================

//���.
#define SAFE_RELEASE(p)	if(p!=nullptr){(p)->Release();(p)=nullptr;}
//�j��.
#define SAFE_DELETE(p) if(p!=nullptr){delete (p);(p)=nullptr;}

#define SAFE_DELETE_ARY(p) if(p!=nullptr){delete[] (p);(p)=nullptr;}

#define ALIGN16	_declspec(align(16))