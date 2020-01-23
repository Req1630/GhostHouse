#include "GameEndScene.h"
#include "..\TItleScene\TitleScene.h"

#include "..\..\XAudio2\SoundManager.h"
#include "..\..\XAudio2\PlaySEThread.h"

#include "..\..\Command\Command.h"

#define _CRT_SECURE_NO_WARNINGS

CGameEndScene::CGameEndScene( shared_ptr<CSceneManager> sceneManager )
	: CSceneBase( sceneManager )

	, m_pGameEndImage(nullptr)
	, m_isLoadEnd( false )
	, m_isNextScene( false )
	, m_isChangeScene( false )
	, m_bPushDownKey( false )
	, m_bPushUpKey( false )
	, m_isEnd( false )
	, ThreadExitCode( -1 )
	, m_bThreadRelease( false )
{

	CFadeUI::Init();
	CSoundManager::InitALL();
	CPlaySEThread::SetUpThread();
	CSoundManager::StopSEByName( "OK" );
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

CGameEndScene::~CGameEndScene()
{
	m_isEnd = true;
	while( Release() == false ){
	}
}

void CGameEndScene::Load( HWND hWnd, ID3D11Device* pDevice11,
	ID3D11DeviceContext* pContext11, LPDIRECT3DDEVICE9 pDevice9 )
{
	if( m_isLoadEnd == true )	return;
	m_hWnd = hWnd;	// �E�B���h�E�n���h�����󂯎��.
	
	if( m_pGameEndImage == nullptr ){
		m_pGameEndImage = CSpriteResource::GetSprite("GameEnding");
		if( m_pGameEndImage != nullptr ){
			CFadeUI::FadeINStart();
		}
	}
}

void CGameEndScene::Update()
{
	if( CSoundManager::GetBGMVolme( "Title" ) != -1 ){
		if( CSoundManager::GetBGMVolme( "Title" ) != 1.0f && m_isNextScene == false ){
			CSoundManager::FadeInBGMByName( "Title" );
		}
		if( CSoundManager::GetBGMVolme( "Title" ) == 0.0f && m_isNextScene == true ){
			m_isChangeScene = true;
		}
	}
	NextScene();
	if( m_isChangeScene == true ){
		Sleep( 10 );
		m_pSceneManager->Swap( make_shared<CTitleScene>( m_pSceneManager ) );
	}
	CPlaySEThread::Updata();

	if( CFadeUI::isActive() == true ) return;


	// R�{�^������.
	if( ( CXInput::IsPress( XINPUT_GAMEPAD_B ) ) ||
		( GetAsyncKeyState( VK_RETURN ) & 0x0001 ) ){
		CPlaySEThread::SetSEName( "OK" );
		CSoundManager::FadeOutBGMByName( "Title" );
		CFadeUI::FadeOUTStart();
		m_isNextScene = true;
	}
	if( CXInput::GetLThumbY() < STICK_IDLING )	m_bPushUpKey = false;
	if( CXInput::GetLThumbY() > -STICK_IDLING )	m_bPushDownKey = false;
}

void CGameEndScene::Render( D3DXMATRIX& mView, D3DXMATRIX& mProj,
	Light& stLight, stCAMERA& stCamera )
{
	if( m_pGameEndImage == nullptr ) return;

	m_pGameEndImage->Render();
}

void CGameEndScene::NextScene()
{
	if( CFadeUI::isEnd() == false ) return;
	if( m_isNextScene == false ) return;

	CFadeUI::Init();
	m_isEnd = true;
	while( Release() == false ){
	}
	
}

bool CGameEndScene::Release()
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