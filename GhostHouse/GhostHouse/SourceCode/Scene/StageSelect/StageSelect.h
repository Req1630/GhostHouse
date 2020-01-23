#ifndef STAGE_SELECT_H
#define STAGE_SELECT_H

#include "..\SceneBase\SceneBase.h"
#include "..\GameScene\GameScene.h"

#include "..\..\UI\StageSelectUI\StageSelectUI.h"
#include "..\..\UI\Fade\FadeUI.h"

#include "..\..\XInput\XInput.h"

#include <thread>

class CStageSelect : public CSceneBase
{
	const std::string BGM_NAME = "StageSelect";
public:
	CStageSelect( shared_ptr<CSceneManager> &sceneManager );
	virtual ~CStageSelect();

	void Load( HWND hWnd, ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11, LPDIRECT3DDEVICE9 pDevice9 ) override;
	void Update() override;
	void Render( D3DXMATRIX& mView, D3DXMATRIX& mProj, 
		Light& stLight, stCAMERA& stCamera ) override;

	bool Release();
private:
	bool NextScene();

	bool m_bLoadFalag;

	// �X�e�[�W�Z���N�gUI.
	unique_ptr<CStageSelectUI>	m_pStageSelectUI;

	bool m_isNextScene;
	bool m_bChangeScene;

	bool	m_isEnd;			//�X���b�h�ɑ΂��ĉ�������o���t���O.
	std::thread BGMThread;		//BGM��炷�X���b�h.
	std::thread::id InThreadID;	//BGM�X���b�hID�ޔ�.

	DWORD ThreadExitCode;
	bool	m_bThreadRelease;
};

#endif // #ifndef STAGE_SELECT_H.