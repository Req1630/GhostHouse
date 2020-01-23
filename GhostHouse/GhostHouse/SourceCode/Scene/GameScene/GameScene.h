#ifndef GAME_SCENE_H
#define GAME_SCENE_H

// �V�[���N���X.
#include "..\SceneBase\SceneBase.h"			// �V�[���x�[�X.
#include "..\TItleScene\TitleScene.h"

// �X�e�[�W�N���X.
#include "..\..\Object\Stage\Stage.h"

#include "..\..\UI\PauseUI\PauseUI.h"
#include "..\..\UI\GameOverUI\GameOverUI.h"
#include "..\..\UI\Iiris\Iris.h"
#include "..\..\UI\Tutorial\TutorialText.h"

#include <thread>

enum class enSceneState
{
	None,

	Game,		// �Q�[��,
	Pause,		// �|�[�Y.
	GameOver,	// �Q�[���I�[�o�[.

	Max,
};

class CGameScene : public CSceneBase
{
	// �Q�[���p�b�h�̃X�e�B�b�N�̌X���̗V��.
	const int STICK_IDLING = 30000;
	const std::string MAIN_BGM_NAME = "GameMain";
	const std::string OVER_BGM_NAME = "GameOver";
public:
	CGameScene( shared_ptr<CSceneManager> &sceneManager );
	virtual ~CGameScene();

	void Load( HWND hWnd, ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11, LPDIRECT3DDEVICE9 pDevice9 ) override;
	void Update() override;
	void Render( D3DXMATRIX& mView, D3DXMATRIX& mProj,
		Light& stLight, stCAMERA& stCamera ) override;

private:
	void GameUpdate();
	void GameRender( D3DXMATRIX& mView, D3DXMATRIX& mProj,
		Light& stLight, stCAMERA& stCamera );

	void PauseUpdate();
	void PauseRender( D3DXMATRIX& mView, D3DXMATRIX& mProj,
		Light& stLight, stCAMERA& stCamera );

	void GameOverUpdate();
	void GameOverRender( D3DXMATRIX& mView, D3DXMATRIX& mProj,
		Light& stLight, stCAMERA& stCamera );

	// ���̃V�[���ւ̏����֐�.
	void PauseNextScene();
	void GameOverNextScene();

	bool Release();
private:

	// �X�e�[�W.
	unique_ptr<CStage>			m_pSatage;
	unique_ptr<CWipeEffectUI>	m_pWipeEffectUI;
	unique_ptr<CPauseUI>		m_pPauseUI;
	unique_ptr<CGameOverUI>		m_pGameOverUI;

	float	m_fAlpha;
	bool	m_bisGoal;

	enSceneState m_enNowSceneState;

	bool m_bPushDownKey;
	bool m_bPushUpKey;

	bool	m_isEnd;			//�X���b�h�ɑ΂��ĉ�������o���t���O.

	std::thread BGMThread;		//BGM��炷�X���b�h.
	std::thread::id InThreadID;	//BGM�X���b�hID�ޔ�.

	DWORD ThreadExitCode;
	bool	m_bThreadRelease;

	int		m_TextArrayNum;
};

#endif // #ifndef GAME_SCENE_H.