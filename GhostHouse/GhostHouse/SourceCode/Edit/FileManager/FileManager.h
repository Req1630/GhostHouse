#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "..\EditHeader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <direct.h>
#include <filesystem>

namespace fs = std::filesystem;

class CFileManager
{
public:
	CFileManager();
	~CFileManager();

	// Imgui�̕`��֐�.
	void ImGuiRender();
	// �}�b�v�̕ۑ�.
	HRESULT Save( const std::vector<edit::RENDER_OBJRCT>& objInfo );
	// �}�b�v�̓ǂݍ���.
	HRESULT Load( std::vector<edit::RENDER_OBJRCT>& objInfo );
	// �X�e�[�W���̎擾�֐�.
	HRESULT GetStageNameList( std::vector<std::string>& stageNameList );
	// ���X�e�[�W�̐ݒ�֐�.
	void SetTmpStage( std::vector<edit::RENDER_OBJRCT>* pobj ){ m_pTmpStage = pobj; }

private:
	// �t�@�C���̍s�̊e�l�̎擾.
	void FieldRead( edit::RENDER_OBJRCT& tmpObj, const std::string& field, const int& count );
	// ���z�L�[�{�[�h�̕\��.
	void KeyBoardRender();

private:
	static char m_SaveFileName[128];	// �Z�[�u���̃t�@�C����.
	static char m_LoadFileName[128];	// ���[�h���̃t�@�C����.

	std::vector<std::string> m_StageNameList;		// �X�e�[�W���̃��X�g.
	std::vector<edit::RENDER_OBJRCT>*	m_pTmpStage;// ���X�e�[�W.

	HRESULT	m_Save_hr;	// �Z�[�u���̃n���h��.
	HRESULT	m_Load_hr;	// ���[�h���̃n���h��.

	bool m_isFileOperations; // ���z�L�[�̕`��t���O.
};

#endif	// #ifndef FILE_MANAGER_H.
