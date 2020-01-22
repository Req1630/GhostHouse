#ifndef GAME_END_SCENE_H
#define GAME_END_SCENE_H

#include "..\SceneBase\SceneBase.h"
#include "..\..\Resource\SpriteResouce\SpriteResouce.h"
#include <thread>

class clsGameEndScene : public clsSceneBase
{
private:
	// �Q�[���p�b�h�̃X�e�B�b�N�̌X���̗V��.
	int STICK_IDLING = 32500;

public:
	clsGameEndScene( shared_ptr<clsSceneManager> sceneManager );
	virtual ~clsGameEndScene();

	void Load( HWND hWnd, ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11, LPDIRECT3DDEVICE9 pDevice9 ) override;
	void Updata() override;
	void Render( D3DXMATRIX& mView, D3DXMATRIX& mProj,
		Light& stLight, stCAMERA& stCamera ) override;

private:
	// ���̃V�[���ւ̏����֐�.
	void NextScene();

	bool Release();
private:
	// �E�B���h�E�n���h��.
	HWND		m_hWnd;
	std::shared_ptr<CSpriteUI> m_pGameEndImage;

	bool m_isLoadEnd;	// �ǂݍ��݂̏I���t���O.
	bool m_isNextScene;	// ���̃V�[���ւ̃t���O.

	bool m_isChangeScene;	// ���̃V�[���֎��ۈړ����鋖�̃t���O.

	bool m_bPushDownKey;
	bool m_bPushUpKey;

	bool	m_isEnd;			//�X���b�h�ɑ΂��ĉ�������o���t���O.
	std::thread BGMThread;		//BGM��炷�X���b�h.
	std::thread::id InThreadID;	//BGM�X���b�hID�ޔ�.

	DWORD ThreadExitCode;
	bool	m_bThreadRelease;
};

#endif	// #ifndef GAME_END_SCENE_H.