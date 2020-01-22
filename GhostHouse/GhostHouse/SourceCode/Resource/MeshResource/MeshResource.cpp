#include "MeshResource.h"

CMeshResorce::CMeshResorce()
	: m_staticMeshList	()
	, m_skinMeshList	()
	, m_isLoadEnd		( false	)
	, m_isThreadJoined	( false )
{
}

CMeshResorce::~CMeshResorce()
{
	m_skinMeshList.clear();
	m_staticMeshList.clear();
}

CMeshResorce* CMeshResorce::GetInstance()
{
	static std::unique_ptr<CMeshResorce> pInstance =
		std::make_unique<CMeshResorce>();	// �C���X�^���X�̍쐬.
	return pInstance.get();
}

void CMeshResorce::Load(
	HWND hWnd,
	ID3D11Device* pDevice11,
	ID3D11DeviceContext* pContext11,
	LPDIRECT3DDEVICE9 pDevice9 )
{
	auto modelLoad = [&]( HWND hWnd, ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11, LPDIRECT3DDEVICE9 pDevice9 )
	{
		GetInstance()->ModelLoad( hWnd, pDevice11, pContext11, pDevice9 );
		GetInstance()->m_isLoadEnd = true;
	};
	GetInstance()->m_thread = std::thread( modelLoad, hWnd, pDevice11, pContext11, pDevice9 );
}

bool CMeshResorce::ThreadRelease()
{
	if( GetInstance()->m_isThreadJoined == true ) return true;
	if( GetInstance()->m_isLoadEnd == false ) return false;
	GetInstance()->m_thread.join();
	GetInstance()->m_isThreadJoined = true;
	return true;
}

std::shared_ptr<CDX9Mesh> CMeshResorce::GetStatic( const std::string& name )
{
	for( auto& m : GetInstance()->m_staticMeshList ){
		if( m.first == name ) return m.second;
	}

	if( GetInstance()->m_isLoadEnd == false ) return nullptr;
	_ASSERT_EXPR( false, L"�X�^�e�B�b�N���b�V����������܂���" );
	MessageBox( nullptr, name.c_str(), "�X�^�e�B�b�N���b�V����������܂���", MB_OK );
	return nullptr;
}

std::shared_ptr<CDX9SkinMesh> CMeshResorce::GetSkin( const std::string& name )
{
	for( auto& m : GetInstance()->m_skinMeshList ){
		if( m.first == name ) return m.second;
	}

	if( GetInstance()->m_isLoadEnd == false ) return nullptr;
	_ASSERT_EXPR( false, L"�X�L�����b�V����������܂���" );
	MessageBox( nullptr, name.c_str(), "�X�L�����b�V����������܂���", MB_OK );
	return nullptr;
}

bool CMeshResorce::GetStatic( std::shared_ptr<CDX9Mesh>& model, const std::string& name )
{
	model = GetStatic( name );
	if( model == nullptr ) return false; 
	return true;
}

bool CMeshResorce::GetSkin( std::shared_ptr<CDX9SkinMesh>& model, const std::string& name )
{
	model = GetSkin( name );
	if( model == nullptr ) return false;
	return true;
}

void CMeshResorce::ModelLoad(
	HWND hWnd,
	ID3D11Device* pDevice11,
	ID3D11DeviceContext* pContext11,
	LPDIRECT3DDEVICE9 pDevice9 )
{
	auto eachLoad = [&]( const fs::directory_entry& entry )
	{
		const std::string exe		= entry.path().extension().string();	// �g���q.
		const std::string filePath	= entry.path().string();				// �t�@�C���p�X.
		const std::string fileName	= entry.path().stem().string();			// �t�@�C����.

		if( exe != ".x" && exe != ".X" ) return;	// �g���q�� "X�t�@�C��" ����Ȃ���ΏI��.

		// "_s" �̓X�L�����b�V��.
		if( fileName.find("_s") == std::string::npos ){
			// "_s" ��������Ȃ��̂� �X�^�e�B�b�N���b�V��.
			m_staticMeshList[fileName] = 
				std::make_shared<CDX9Mesh>( hWnd, pDevice11, pContext11, pDevice9, filePath.c_str() );
		} else {
			// �X�L�����b�V��.
			m_skinMeshList[fileName] = 
				std::make_shared<CDX9SkinMesh>( hWnd, pDevice11, pContext11, pDevice9, filePath.c_str() );
		}
	};

	try {
		fs::recursive_directory_iterator dir_itr( FILE_PATH ), end_itr;
		std::for_each( dir_itr, end_itr, eachLoad );

	} catch( const fs::filesystem_error& e ){
		const char* errorMessage = e.path1().string().c_str();
		_ASSERT_EXPR( false, L"���b�V���t�@�C���Ǎ����s" );
		MessageBox( nullptr, errorMessage, "���b�V���t�@�C���Ǎ����s", MB_OK );
	}
}