#ifndef EDIT_SCENE_H
#define EDIT_SCENE_H

#include "..\SceneBase\SceneBase.h"
#include "..\..\Edit\Edit.h"

class CEditScene : public CSceneBase
{
public:
	CEditScene( shared_ptr<CSceneManager> sceneManager );
	~CEditScene();

	void Load( 
		HWND hWnd, 
		ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11,
		LPDIRECT3DDEVICE9 pDevice9 ) override;
	void Update() override;
	void Render( D3DXMATRIX& mView, D3DXMATRIX& mProj,
		Light& stLight, stCAMERA& stCamera ) override;

private:
	unique_ptr<CEdit> m_pEdit;
	bool m_isImGuiCreate;

	bool m_isEnd;			//�X���b�h�ɑ΂��ĉ�������o���t���O.
	std::thread BGMThread;		//BGM��炷�X���b�h.
	std::thread::id InThreadID;	//BGM�X���b�hID�ޔ�.

	DWORD ThreadExitCode;
	bool	m_bThreadRelease;
	bool	m_isLightSet;
};

#endif // #ifndef EDIT_SCENE_H.