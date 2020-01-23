#include "TitleScene.h"
#include "..\EditScene\EditScene.h"

#include "..\..\XAudio2\SoundManager.h"
#include "..\..\XAudio2\PlaySEThread.h"

#include "..\..\Command\Command.h"

#define _CRT_SECURE_NO_WARNINGS

CTitleScene::CTitleScene( shared_ptr<CSceneManager> sceneManager )
	: CSceneBase		( sceneManager )
	, m_pSatage			( make_unique<CStage>() )
	, m_pTitleUI		( make_unique<CTitleUI>() )
	, m_isLoadEnd		( false )
	, m_isNextScene		( false )
	, m_isEditNextScene	( false )
	, m_isChangeScene	( false )
	, m_isEditChangeScene( false )
	, m_bPushDownKey	( false )
	, m_bPushUpKey		( false )
	, m_isEnd			( false )
	, ThreadExitCode	( -1 )
	, m_bThreadRelease	( false )
{

	CFadeUI::Init();
	CSoundManager::InitALL();
	CPlaySEThread::SetUpThread();
	CSoundManager::StopSEByName("OK");
	auto BGM = [&]()
	{
		CSoundManager::PlayBGMByName( "Title", m_isEnd, true );
	};
	BGMThread = std::thread( BGM );
	InThreadID = BGMThread.get_id();
	//BGM���t�F�[�h�C��������ׂɁA�{�����[����0��.
	CSoundManager::SetBGMVolme( "Title", 0.2f );
	CCommand::Initialize();
}

CTitleScene::~CTitleScene()
{
	m_isEnd = true;
	while( Release() == false ){}
}

void CTitleScene::Load( HWND hWnd, ID3D11Device* pDevice11,
	ID3D11DeviceContext* pContext11, LPDIRECT3DDEVICE9 pDevice9 )
{
	if( m_isLoadEnd == true )	return;
	m_hWnd = hWnd;	// �E�B���h�E�n���h�����󂯎��.
	// �X�e�[�W�̓ǂݍ���.
	m_pSatage->Load( pDevice11, pContext11 );
	if( m_pTitleUI->Load() == true  && m_pSatage->isRender() == true ){
		CFadeUI::FadeINStart();
		m_isLoadEnd = true;	// ���[�h�̏I���t���O�𗧂Ă�.
	}
}

void CTitleScene::Update()
{
	if (CSoundManager::GetBGMVolme("Title") != -1) {
		if (CSoundManager::GetBGMVolme("Title") != 1.0f && m_isNextScene == false) {
			CSoundManager::FadeInBGMByName("Title");
		}
		if (CSoundManager::GetBGMVolme("Title") == 0.0f && m_isNextScene == true) {
			m_isChangeScene = true;
		}
		if( CSoundManager::GetBGMVolme( "Title" ) == 0.0f && m_isEditNextScene == true ){
			m_isEditChangeScene = true;
		}
	}
	NextScene();
	if (m_isChangeScene == true) {
		Sleep(10);
		m_pSceneManager->Push( make_shared<CStageSelect>(m_pSceneManager) );
	}
	if( m_isEditChangeScene == true ){
		Sleep(10);
		m_pSceneManager->Change( make_shared<CEditScene>(m_pSceneManager) );
	}
	CPlaySEThread::Updata();

	if( CFadeUI::isActive() == true ) return;

	m_pTitleUI->Update();
	//// �X�e�B�b�N�����.
	//if( CXInput::GetLThumbY() > STICK_IDLING ||
	//	( GetAsyncKeyState( VK_UP ) & 0x8000 ) ){
	//	if( m_bPushUpKey == false ){
	//		m_bPushUpKey = true;
	//		CPlaySEThread::SetSEName( "Select" );
	//	}
	//}
	//// �X�e�B�b�N������.
	//if( CXInput::GetLThumbY() < -STICK_IDLING ||
	//	( GetAsyncKeyState( VK_DOWN ) & 0x8000 ) ){
	//	if( m_bPushDownKey == false ){
	//		m_bPushDownKey = true;
	//		CPlaySEThread::SetSEName( "Select" );
	//	}
	//}

	// R�{�^������.
	if(( CXInput::IsPress( XINPUT_GAMEPAD_B )) ||
		( GetAsyncKeyState( VK_RETURN ) & 0x0001 ) ){
		CPlaySEThread::SetSEName( "OK" );
		CSoundManager::FadeOutBGMByName( "Title" );
		CFadeUI::FadeOUTStart();
		m_isNextScene = true;
	}
	if( CXInput::GetLThumbY() < STICK_IDLING )	m_bPushUpKey = false;
	if( CXInput::GetLThumbY() > -STICK_IDLING )	m_bPushDownKey = false;

	if( CCommand::GetSuccessCommand() ){
		CPlaySEThread::SetSEName( "OK" );
		CSoundManager::FadeOutBGMByName( "Title" );
		CFadeUI::FadeOUTStart();
		m_isEditNextScene = true;
	}
}

void CTitleScene::Render( D3DXMATRIX& mView, D3DXMATRIX& mProj,
	Light& stLight, stCAMERA& stCamera )
{
	m_pSatage->SetCameraOFF();

	// �X�e�[�W�̕\��.
	m_pSatage->Render( mView, mProj, stLight, stCamera );

	// �^�C�g��UI�̕\��.
	m_pTitleUI->Render();
}

void CTitleScene::NextScene()
{
	if( CFadeUI::isEnd() == false ) return;
	if( m_isNextScene == false &&
		m_isEditNextScene == false ) return;

	switch( m_pTitleUI->GetNowMenuState()  )
	{
	case CTitleUI::Start:
		CFadeUI::Init();
		m_isEnd = true;
		while( Release() == false ){}

		break;
	case CTitleUI::End:
		m_isEnd = true;
		while( Release() == false ){}

		// �E�B���h�E�����.
		PostMessage( m_hWnd, WM_CLOSE, 0, 0 );

		break;

	case CTitleUI::Edit:
		CFadeUI::Init();
		m_isEnd = true;
		while( Release() == false ){}
		break;
	default:
		break;
	}
}


bool CTitleScene::Release()
{
	GetExitCodeThread( BGMThread.native_handle(), &ThreadExitCode );

	//ThreadExitCode = 1234567890;
	//ThreadExitCode = 1234567890;
	if( ThreadExitCode == 0 ){
		if( InThreadID == BGMThread.get_id() ){
			m_isEnd = true;				//�Ȃ��X�g�b�v�����ꍇ�A�X�g�b�v�𖽗߂����R�[�h�̒���
			//CSoundManager::CleanBGM("Title");
			//false�̊ԃ��[�v�ő҂����Ă���̂ŁASoundManager��������Ă���
			//true�ŃX���b�h����while���[�v����ł�B
			BGMThread.detach();
			m_bThreadRelease = true;
			return true;
		}
		if( m_bThreadRelease == true ){
			return true;
		}
		return false;
	}


	return false;
}