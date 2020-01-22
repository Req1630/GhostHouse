#ifndef SPRITE_RESOURCE_H
#define SPRITE_RESOURCE_H

#include "..\..\SingletonBase\SingletonBase.h"
#include "Sprite/CSpriteUI.h"

#include <thread>

class CSpriteResource : public CSingletonBase<CSpriteResource>
{
	const char* SPRITE_LIST_TEXT_PAHT	= "Data\\PathText\\SpriteList.csv";	// ���f�����X�g�e�L�X�g�̃t�@�C���p�X.

public:
	CSpriteResource()
		: m_isLoadEnd		( false )
		, m_isThreadJoined	( false )
	{}
	virtual ~CSpriteResource(){}

	static void Load( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 );
	static shared_ptr<CSpriteUI> GetSprite( const string& modelName );
	static bool ThreadRelease();

private:
	void SpriteLoad( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 );
	void TextDataLoad( vector<string> &Modelpath, vector<string> &ModelName, vector<string> &SpriteTextoath );
	string TextPathSearch( string buf );
	string NameSearch( string buf );
	void Init( CSpriteUI::SPRITE_STATE* pss, string buf );

private:
	map<string, shared_ptr<CSpriteUI>>	m_pSpriteList;			// �X�v���C�g�N���X.

	std::thread		m_thread;	// �X���b�h.

	bool	m_isLoadEnd;		// ���[�h���I��������.
	bool	m_isThreadJoined;	// �X���b�h��������ꂽ��.

private:
	// �R�s�[�E���[�u�R���X�g���N�^, ������Z�q�̍폜.
	CSpriteResource( const CSpriteResource & ) = delete;
	CSpriteResource& operator = ( const CSpriteResource & ) = delete;
	CSpriteResource( CSpriteResource && ) = delete;
	CSpriteResource& operator = ( CSpriteResource && ) = delete;
};

#endif	// #ifndef SPRITE_RESOURCE_H.