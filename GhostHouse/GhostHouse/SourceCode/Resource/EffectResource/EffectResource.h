#ifndef EFFECT_RESOURCE_H
#define EFFECT_RESOURCE_H

#include "Effect/CEffect.h"

#include <unordered_map>
#include <string>
#include <memory>
#include <thread>
#include <filesystem>

namespace fs = std::filesystem;

class CEffectResource
{
	const char* FILE_PATH = "Data\\Effect";
	typedef std::unordered_map<std::string, std::shared_ptr<CEffect>> effect_map;
public:
	CEffectResource();
	~CEffectResource();
	// �C���X�^���X�̎擾.
	static CEffectResource* GetInstance();

	// �ǂݍ���.
	static void Load( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 );
	// �X���b�h�̉��.	
	static bool ThreadRelease();
	// �G�t�F�N�g�擾�֐�.
	static std::shared_ptr<CEffect> GetEffect( const std::string& fileName );

private:
	void EffectLoad( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 );

private:
	effect_map m_EffectList;	// �G�t�F�N�g���X�g.

	std::thread m_thread;		// �X���b�h.

	bool m_isLoadEnd;			// ���[�h���I��������.
	bool m_isThreadJoined;		// �X���b�h��������ꂽ��.

private:
	// �R�s�[�E���[�u�R���X�g���N�^, ������Z�q�̍폜.
	CEffectResource( const CEffectResource & )					= delete;
	CEffectResource& operator = ( const CEffectResource & )		= delete;
	CEffectResource( CEffectResource && )						= delete;
	CEffectResource& operator = ( CEffectResource && )			= delete;
};

#endif	// #ifndef EFFECT_RESOURCE_H.