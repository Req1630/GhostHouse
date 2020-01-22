#include "MapLoad.h"

// �}�b�v�I�u�W�F�N�g.
#include "..\..\Object\Map\Event.h"
#include "..\..\Object\Map\Goal.h"
#include "..\..\Object\Map\TextEvent.h"

// �v���C���[.
#include "..\..\Object\Character\Player\Sayaka\Sayaka.h"
#include "..\..\Object\Character\Player\Towa\Towa.h"

// �S�[�X�g.
#include "..\..\Object\Character\Ghost\GhostA\GhostA.h"
#include "..\..\Object\Character\Ghost\GhostB\GhostB.h"
#include "..\..\Object\Character\Ghost\BigGhost\BigGhost.h"

// �M�~�b�N.
#include "..\..\Object\Gimmick\BookShelf\BookShelf.h"
#include "..\..\Object\Gimmick\InvisibleFloor\InvisibleFloor.h"
#include "..\..\Object\Gimmick\InvisibleFloor\SlipThroughFloor.h"
#include "..\..\Object\Gimmick\InvisibleFloor\InvisibleEndFloor.h"
#include "..\..\Object\Gimmick\InvisibleFloor\InvisibleStartFloor.h"
#include "..\..\Object\Gimmick\Signboard\Signboard.h"
#include "..\..\Object\Gimmick\Key\Key.h"
#include "..\..\Object\Gimmick\KeyDoor\KeyDoor.h"
#include "..\..\Object\Gimmick\Pitfall\PitfallLeft\PitfallLeft.h"
#include "..\..\Object\Gimmick\Pitfall\PitfallRight\PitfallRight.h"
#include "..\..\Object\Gimmick\InvisibleFloor\CameraUpCollision.h"
#include "..\..\Object\Gimmick\Switch\Switch.h"
#include "..\..\Object\Gimmick\SwitchDoor\SwitchDoor.h"
#include "..\..\Object\Gimmick\SwitchRed\SwitchRed.h"
#include "..\..\Object\Gimmick\SwitchDoorRed\SwitchDoorRed.h"
#include "..\..\Object\Gimmick\UpDownFloor\UpDownFloor.h"
#include "..\..\Object\Map\InvisibleWall.h"

#include "..\..\Object\Map\MapObject\MapObject.h"
#include "..\..\Object\Map\MapObject\MapObjectObstacle.h"

// �e���v���[�g�Ŏw�肵���^���쐬����.
template<class T>
shared_ptr<CObjectBase> InitFunc( const stObjectInfo& ObjInfo )
{
	return make_shared<T>( ObjInfo );
}


CMapLoad::CMapLoad()
	: m_NowStageNum		( 0 )
	, m_isLoadEnd		( false )
	, m_isThreadJoined	( false )
{
	unordered_map<string, Init_func> objList =
	{
		{ "Ground",					InitFunc<CMapObject> },
		{ "Ground_I",				InitFunc<CMapObject> },
		{ "Ground_L",				InitFunc<CMapObject> },
		{ "Flor",					InitFunc<CMapObject> },
		{ "Flor_I_one",				InitFunc<CMapObject> },
		{ "Flor_I",					InitFunc<CMapObject> },
		{ "Flor_L_one",				InitFunc<CMapObject> },
		{ "Flor_L",					InitFunc<CMapObject> },
		{ "NarrowRoad1",			InitFunc<CMapObject> },
		{ "NarrowRoad2",			InitFunc<CMapObject> },
		{ "NarrowRoad3",			InitFunc<CMapObject> },
		{ "NarrowRoad4",			InitFunc<CMapObject> },
		{ "Wall",					InitFunc<CMapObjectObstacle> },
		{ "Wall_out",				InitFunc<CMapObjectObstacle> },
		{ "Entrance",				InitFunc<CMapObjectObstacle> },
		{ "Entrance_close",			InitFunc<CMapObjectObstacle> },
		{ "OpenDoor",				InitFunc<CMapObjectObstacle> },
		{ "Tree",					InitFunc<CMapObjectObstacle> },
		{ "Roof",					InitFunc<CMapObjectObstacle> },
		{ "Roof_L",					InitFunc<CMapObjectObstacle> },
		{ "Roof2_I",				InitFunc<CMapObjectObstacle> },
		{ "Roof2_L",				InitFunc<CMapObjectObstacle> },
		{ "Pillar",					InitFunc<CMapObjectObstacle> },

		{ "Sayaka_s",				InitFunc<CSayaka> },
		{ "Towa_s",					InitFunc<CTowa> },

		{ "GhostA",					InitFunc<CGhostA> },
		{ "GhostB_s",				InitFunc<CGhostB> },
		{ "BigGhost_s",				InitFunc<CBigGhost> },

		{ "BookShelf",				InitFunc<CBookShelf> },
		{ "InvisibleFloor",			InitFunc<CInvisibleFloor> },
		{ "InvisibleStartFloor",	InitFunc<CInvisibleStartFloor> },
		{ "InvisibleEndFloor",		InitFunc<CInvisibleEndFloor> },
		{ "SlipThroughFloor",		InitFunc<CSlipThroughFloor> },
		{ "Signboard",				InitFunc<CSignboard> },
		{ "Key",					InitFunc<CKey> },
		{ "KeyDoor",				InitFunc<CKeyDoor> },
//		{ "Pitfall",				InitFunc<CPitfall> },
		{ "LongCollision",			InitFunc<CCameraUpCollision> },
		{ "Switch",					InitFunc<CSwitch> },
		{ "SwitchDoor",				InitFunc<CSwitchDoor> },
		{ "Switch_Red",				InitFunc<CSwitchRed> },
		{ "SwitchDoor_Red",			InitFunc<CSwitchDoorRed> },
		{ "UpDownFloor",			InitFunc<CUpDownFloor> },

		{ "Event",					InitFunc<CEvent> },
		{ "Goal",					InitFunc<CGoal> },
		{ "TextEvent",				InitFunc<CTextEvent> },
		{ "InvisibleWall",			InitFunc<CInvisibleWall> },
		{ "InvisibleBox",			InitFunc<CInvisibleWall> },
	};
	objList.swap( CreateObjList );
}

CMapLoad::~CMapLoad()
{
}

// �t�@�C���̓ǂݍ��� �}�b�v�T�C�Y�擾.
void CMapLoad::CreateStage( vector<shared_ptr<CObjectBase>> &pvObj )
{
	pvObj.clear();
	string fileName = GetInstance()->m_sFileName;
	string filePath = "Data\\MapData\\" + fileName + "\\" + fileName + ".csv";
	ifstream CSVData( filePath );
	string line;		// ��ł̕����擾�p.
	string field;		// �s�ł̕����擾�p.
	int fieldnum = 0;

	if( !CSVData ){
		// �t�@�C�����������ǂݍ��߂Ă��Ȃ���� �G���[ ���f�o�b�O���̂�.
		_ASSERT_EXPR( CSVData, L"�}�b�v�f�[�^�̓ǂݍ��ݎ��s" );
		return;
	}

	int Index = 0;
	enLine LineNum = enLine::Index;
	while( getline( CSVData, line ) ){
		istringstream stream( line );
		if( fieldnum == 0 ){// �ŏ��̈�s���X�L�b�v.
			fieldnum++;
			continue;
		}
		stObjectInfo stobjInfo;
		while( getline( stream, field, ',' ) ){
			switch( LineNum ){
				case enLine::Index:	// �v�f��.
					LineNum = enLine::xPosition;
					break;

				case enLine::xPosition:	// x���W.
					stobjInfo.vPos.x = stof( field );
					LineNum = enLine::yPosition;
					break;
				case enLine::yPosition:	// y���W.
					stobjInfo.vPos.y = stof( field );
					LineNum = enLine::zPosition;
					break;
				case enLine::zPosition:	// z���W.
					stobjInfo.vPos.z = stof( field );
					LineNum = enLine::xRotation;
					break;

				case enLine::xRotation:	// x���W.
					stobjInfo.vRot.x = stof( field );
					LineNum = enLine::yRotation;
					break;
				case enLine::yRotation:	// y���W.
					stobjInfo.vRot.y = stof( field );
					LineNum = enLine::zRotation;
					break;
				case enLine::zRotation:	// z���W.
					stobjInfo.vRot.z = stof( field );
					LineNum = enLine::TipNo;
					break;

				case enLine::TipNo:	// �`�b�v�i���o�[.
					stobjInfo.ObjName = field;
					LineNum = enLine::Index;
					break;
			}
		}
		if( stobjInfo.enObjNo == enObjectNo::None ) continue;

		try {
			bool is_notFind = true;
			for( auto & m : GetInstance()->CreateObjList ){
				if( m.first == stobjInfo.ObjName ){
					pvObj.emplace_back( m.second(stobjInfo) );
					is_notFind = false;
					break;
				}
			}
			if( is_notFind == true ){
				throw "�I�u�W�F�N�g��������܂���";
			}
		} catch ( char* e ){
			string message = e;
			_ASSERT_EXPR( false, L"�I�u�W�F�N�g��������܂���" );
		}

		Index++;
	}
	CSVData.close();
}

void CMapLoad::FileRead( vector<shared_ptr<CObjectBase>> &pvObj, string fileName )
{
	GetInstance()->m_sFileName = fileName;
	GetInstance()->CreateStage( pvObj );
}

void CMapLoad::StageNameListLoad( ID3D11Device* pDevice11,
	ID3D11DeviceContext* pContext11 )
{
	auto stageNameListSearch = [&]( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 )
	{
		GetInstance()->StageNameListSearch( pDevice11, pContext11 );
		GetInstance()->m_isLoadEnd = true;
	};
	GetInstance()->m_thread = std::thread( stageNameListSearch, pDevice11, pContext11 );
}

void CMapLoad::StageNameListSearch(  ID3D11Device* pDevice11,
	ID3D11DeviceContext* pContext11 )
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	string filePath = "Data\\MapData\\*";
	vector<string> tmpStageNameList;

	hFind = FindFirstFile( filePath.c_str(), &FindFileData );
	if( hFind != FreeLibrary ){
		do{
			tmpStageNameList.emplace_back( FindFileData.cFileName );
		} while( FindNextFile( hFind, &FindFileData ) );
		// m_vsStageList : [0]"." [1]".." �̍폜.
		tmpStageNameList.erase( tmpStageNameList.begin(), tmpStageNameList.begin()+2 );
		FindClose( hFind );
	}
	// �X�e�[�W���X�g��ۑ�.
	m_vsStageNameList = tmpStageNameList;
	m_sFileName = tmpStageNameList[0];
	CSpriteUI::SPRITE_STATE ss;
	ss.Base.w = 800;
	ss.Base.h = 500;
	ss.Disp.w = ss.Base.w / 1.5f;
	ss.Disp.h = ss.Base.h / 1.5f;
	ss.Stride.w = ss.Base.w;
	ss.Stride.h = ss.Base.h;
	ss.vPos = D3DXVECTOR3( 1000.0f,400.0f, 0.6f );
	for( int i = 0; i < static_cast<int>(tmpStageNameList.size()); i++ ){
		// �X�e�[�W�摜�̃p�X�ݒ�.
		string imagePath = "Data\\MapData\\" + tmpStageNameList[i] + "\\" + tmpStageNameList[i] + ".png";
		m_mStageInfoDataList[i] = make_pair( tmpStageNameList[i], 
			make_shared<CSpriteUI>( pDevice11, pContext11, imagePath.c_str(), ss ) );
	}
}

bool CMapLoad::ThreadRelease()
{
	if( GetInstance()->m_isThreadJoined == true ) return true;
	if( GetInstance()->m_isLoadEnd == false ) return false;
	GetInstance()->m_thread.join();
	GetInstance()->m_isThreadJoined = true;
	return true;
}
