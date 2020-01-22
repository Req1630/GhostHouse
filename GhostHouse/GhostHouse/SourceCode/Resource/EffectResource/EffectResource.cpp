#include "EffectResource.h"

CEffectResource::CEffectResource()
	: m_EffectList	()
{
}

CEffectResource::~CEffectResource()
{
	m_EffectList.clear();
}

CEffectResource* CEffectResource::GetInstance()
{
	static std::unique_ptr<CEffectResource> pInstance =
		std::make_unique<CEffectResource>();	// �C���X�^���X�̍쐬.
	return pInstance.get();
}


void CEffectResource::Load( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 )
{
	auto effectLoad = [&]( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 )
	{
		GetInstance()->EffectLoad( pDevice11, pContext11 );
		GetInstance()->m_isLoadEnd = true;
	};
	GetInstance()->m_thread = std::thread( effectLoad, pDevice11, pContext11 );
}

bool CEffectResource::ThreadRelease()
{
	if( GetInstance()->m_isThreadJoined == true ) return true;
	if( GetInstance()->m_isLoadEnd == false ) return false;
	GetInstance()->m_thread.join();
	GetInstance()->m_isThreadJoined = true;
	return true;
}

std::shared_ptr<CEffect> CEffectResource::GetEffect( const std::string& fileName )
{
	for( auto& m : GetInstance()->m_EffectList ){
		if( m.first == fileName ) return m.second;
	}

	if( GetInstance()->m_isLoadEnd == false ) return nullptr;
	_ASSERT_EXPR( false, L"�X�^�e�B�b�N���b�V����������܂���" );
	MessageBox( nullptr, fileName.c_str(), "�X�^�e�B�b�N���b�V����������܂���", MB_OK );
	return nullptr;
}

void CEffectResource::EffectLoad(
	ID3D11Device* pDevice11,
	ID3D11DeviceContext* pContext11 )
{
	auto eachLoad = [&]( const fs::directory_entry& entry )
	{
		const std::string exe		= entry.path().extension().string();	// �g���q.
		const std::string filePath	= entry.path().string();				// �t�@�C���p�X.
		const std::string fileName	= entry.path().stem().string();			// �t�@�C����.

		if( exe != ".efk" ) return;	// �g���q�� "X�t�@�C��" ����Ȃ���ΏI��.

		m_EffectList[fileName] = 
			std::make_shared<CEffect>( pDevice11, pContext11, filePath );
	};

	try {
		fs::recursive_directory_iterator dir_itr(FILE_PATH), end_itr;
		std::for_each( dir_itr, end_itr, eachLoad );
	} catch( const fs::filesystem_error& e ){
		const char* errorMessage = e.path1().string().c_str();
		MessageBox( nullptr, errorMessage, "�G�t�F�N�g�ǂݍ��ݎ��s", MB_OK );
	}
}