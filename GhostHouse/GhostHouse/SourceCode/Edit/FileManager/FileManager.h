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

	void ImGuiRender();

	HRESULT Save( const std::vector<edit::RENDER_OBJRCT>& objInfo );
	HRESULT Load( std::vector<edit::RENDER_OBJRCT>& objInfo );
	HRESULT GetStageNameList( std::vector<std::string>& stageNameList );

	void SetTmpStage( std::vector<edit::RENDER_OBJRCT>* pobj ){ m_pTmpStage = pobj; }

private:
	void FieldRead( edit::RENDER_OBJRCT& tmpObj, const std::string& field, const int& count );
	void KeyBoardRender();

private:
	static char m_SaveFileName[128];
	static char m_LoadFileName[128];

	std::vector<std::string> m_StageNameList;
	std::vector<edit::RENDER_OBJRCT>*	m_pTmpStage;

	HRESULT	m_Save_hr;
	HRESULT	m_Load_hr;

	bool m_isFileOperations;
};

#endif	// #ifndef FILE_MANAGER_H.
