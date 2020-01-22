#ifndef STAGE_SELECT_H
#define STAGE_SELECT_H

#include "..\SceneBase\SceneBase.h"
#include "..\GameScene\GameScene.h"

#include "..\..\UI\StageSelectUI\StageSelectUI.h"
#include "..\..\UI\Fade\FadeUI.h"

#include "..\..\XInput\XInput.h"

#include <thread>

class clsStageSelect : public clsSceneBase
{
	const std::string BGM_NAME = "StageSelect";
public:
	clsStageSelect( shared_ptr<clsSceneManager> &sceneManager );
	virtual ~clsStageSelect();

	void Load( HWND hWnd, ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11, LPDIRECT3DDEVICE9 pDevice9 ) override;
	void Updata() override;
	void Render( D3DXMATRIX& mView, D3DXMATRIX& mProj, 
		Light& stLight, stCAMERA& stCamera ) override;

	bool Release();
private:
	bool NextScene();

	bool m_bLoadFalag;

	// ステージセレクトUI.
	unique_ptr<CStageSelectUI>	m_pStageSelectUI;

	bool m_isNextScene;
	bool m_bChangeScene;

	bool	m_isEnd;			//スレッドに対して解放許可を出すフラグ.
	std::thread BGMThread;		//BGMを鳴らすスレッド.
	std::thread::id InThreadID;	//BGMスレッドID退避.

	DWORD ThreadExitCode;
	bool	m_bThreadRelease;
};

#endif // #ifndef STAGE_SELECT_H.