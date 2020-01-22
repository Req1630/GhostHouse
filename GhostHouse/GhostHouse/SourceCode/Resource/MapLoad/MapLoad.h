#ifndef MAP_LOAD_H
#define MAP_LOAD_H

#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <thread>
#include <unordered_map>

#include "..\..\SingletonBase\SingletonBase.h"
#include "..\..\Resource\SpriteResouce\Sprite\CSpriteUI.h"
#include "..\..\ObjectBase\ObjectBase.h"

using namespace std;

class CMapLoad : public CSingletonBase<CMapLoad>
{
	typedef function<shared_ptr<CObjectBase>(const stObjectInfo&)> Init_func;
private:
	enum class enLine : int
	{
		None,

		Index,		// �C���f�b�N�X.
		xPosition,	// x ���W.
		yPosition,	// y ���W.
		zPosition,	// z ���W.

		xRotation,	// x ���W.
		yRotation,	// y ���W.
		zRotation,	// z ���W.

		TipNo,		// �`�b�v�i���o�[.

		Max,
	};

public:
	CMapLoad();
	virtual ~CMapLoad();

	// �t�@�C���̓ǂݍ���.
	static void CreateStage( vector<shared_ptr<CObjectBase>> &pvObj );
 	static void FileRead( vector<shared_ptr<CObjectBase>> &pvObj, string fileName );

	// �e�X�e�[�W���̓ǂݍ���.
	static void StageNameListLoad( ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11 );
	// �X���b�h�̉��.
	static bool ThreadRelease();

	// �X�e�[�W���擾�֐�.
	static string GetStageName( int i ){ return GetInstance()->m_mStageInfoDataList.at(i).first; }
	static shared_ptr<CSpriteUI> GetStageImage( int i ){ return GetInstance()->m_mStageInfoDataList.at(i).second; }
	static pair<string, shared_ptr<CSpriteUI>> GetStageInfoData( int i ){ return GetInstance()->m_mStageInfoDataList.at(i); }
	static void SetStageNum( int i ){ GetInstance()->m_NowStageNum = i; }
	static int GetStageNum(){ return GetInstance()->m_NowStageNum; }
	// �X�e�[�W�̍ő吔�擾�֐�.
	static int GetMAXStageNumber(){ return GetInstance()->m_mStageInfoDataList.size(); }

	static void SetStageName( string sFileName ){ GetInstance()->m_sFileName = sFileName; }

private:
	// �X�e�[�W�������֐�.
	void StageNameListSearch( ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11 );

private:
	// �X�e�[�W��.
	string			m_sFileName;
	// �X�e�[�W���̃��X�g.
	vector<string>	m_vsStageNameList;
	// �X�e�[�W�́@���O�Ɖ摜���擾.
	map<int, pair<string, shared_ptr<CSpriteUI>>>	m_mStageInfoDataList;

	unordered_map<string, Init_func> CreateObjList;

	int m_NowStageNum;

	std::thread		m_thread;	// �X���b�h.

	bool	m_isLoadEnd;		// ���[�h���I��������.
	bool	m_isThreadJoined;	// �X���b�h��������ꂽ��.
};

#endif	// #ifndef MAP_LOAD_H.