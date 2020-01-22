#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "..\SceneBase\SceneBase.h"

using namespace std;

struct Light;
struct stCAMERA;

class clsSceneBase;

class clsSceneManager
{
public:
	clsSceneManager();
	~clsSceneManager();

	void Load( HWND hWnd, ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11, LPDIRECT3DDEVICE9 pDevice9 );
	void LoadRender( D3DXMATRIX& mView, D3DXMATRIX& mProj,
		Light& stLight, stCAMERA& stCamera );
	void Update();
	void Render( D3DXMATRIX& mView, D3DXMATRIX& mProj,
		Light& stLight, stCAMERA& stCamera );

	void Push( shared_ptr<clsSceneBase> pScenebase );
	void Pop();
	void TwoPop();
	void Change( shared_ptr<clsSceneBase> pScenebase );
	void Swap( shared_ptr<clsSceneBase> pScenebase );

	void StackRelease();

private:
	stack<shared_ptr<clsSceneBase>> m_pStackScene;
};

#endif // #ifndef SCENE_MANAGER_H.