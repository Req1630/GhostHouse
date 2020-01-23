#ifndef TITLE_SCENE_H
#define TITLE_SCENE_H

#include "..\SceneBase\SceneBase.h"
#include "..\GameScene\GameScene.h"
#include "..\StageSelect\StageSelect.h"

#include "..\..\UI\TitleUI\TitleUI.h"
#include "..\..\UI\Fade\FadeUI.h"

#include "..\..\Resource\SpriteResouce\SpriteResouce.h"
#include "..\..\Object\Stage\Stage.h"
#include "..\..\Camera\Camera.h"
#include "..\..\XInput\XInput.h"

#include "..\..\DebugMode\DebugText\DebugText.h"

#include <thread>

class CTitleScene : public CSceneBase
{
private:
	// �Q�[���p�b�h�̃X�e�B�b�N�̌X���̗V��.
	int STICK_IDLING	= 32500;

public:
	CTitleScene( shared_ptr<CSceneManager> sceneManager );
	virtual ~CTitleScene();

	void Load( HWND hWnd, ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11, LPDIRECT3DDEVICE9 pDevice9 ) override;
	void Update() override;
	void Render( D3DXMATRIX& mView, D3DXMATRIX& mProj,
		Light& stLight, stCAMERA& stCamera ) override;

private:
	// ���̃V�[���ւ̏����֐�.
	void NextScene();

	bool Release();
private:
	// �E�B���h�E�n���h��.
	HWND		m_hWnd;

	// �X�e�[�W.
	unique_ptr<CStage>		m_pSatage;
	// �J����.
	unique_ptr<CCamera>		m_pCamera;
	// �^�C�g��UI.
	unique_ptr<CTitleUI>	m_pTitleUI;

	bool m_isLoadEnd;	// �ǂݍ��݂̏I���t���O.
	bool m_isNextScene;	// ���̃V�[���ւ̃t���O.
	bool m_isEditNextScene;

	bool m_isChangeScene;	// ���̃V�[���֎��ۈړ����鋖�̃t���O.
	bool m_isEditChangeScene;

	bool m_bPushDownKey;
	bool m_bPushUpKey;

	bool	m_isEnd;			//�X���b�h�ɑ΂��ĉ�������o���t���O.
	std::thread BGMThread;		//BGM��炷�X���b�h.
	std::thread::id InThreadID;	//BGM�X���b�hID�ޔ�.

	DWORD ThreadExitCode;
	bool	m_bThreadRelease;
};

#endif // #ifndef TITLE_SCENE_H.