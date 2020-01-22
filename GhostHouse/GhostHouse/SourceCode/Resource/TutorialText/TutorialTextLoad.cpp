#include "TutorialTextLoad.h"
#include <fstream>
CTutorialTextLoad::CTutorialTextLoad()
{
	InitHiraganaBox();
}
CTutorialTextLoad::~CTutorialTextLoad()
{
}

void CTutorialTextLoad::TextDataLoad()
{
	// �t�@�C�����J��.
	fstream filestream( GetInstance()->TEXT_FILENAME );
	if( !filestream.is_open() ) return;

	int i = 0;
	// �t�@�C����ǂݍ���.
	while( !filestream.eof() ){
		string buffer;
		//Text�t�@�C������ǂݎ����1�s��string�ϐ��ubuffer]
		filestream >> buffer;
		//�v�f�𑝂₵�ď�����.
		GetInstance()->TextBox.emplace_back();
		GetInstance()->m_vbAsteriskFlag.emplace_back( false );
		//���{���2�o�C�g�Ȃ̂ŁAsubstr��
		//(�J�n�ʒu�u�����������v�Łu���v�Ȃ�4, �o�C�g��(���{��Ȃ�2)���w�肵�Ď��o��.
		for( size_t StartNum = 0; StartNum < buffer.length(); StartNum += 2 ){
			GetInstance()->TextBox[i].emplace_back( buffer.substr( StartNum, 2 ) );
		}
		i++;
	}

	GetInstance()->m_HiragnaBoxSize = GetInstance()->HiraganaBox.size();

}

D3DXVECTOR2 CTutorialTextLoad::SearchCharPosition( size_t TextBoxNum, size_t TwoArray )
{
	//TextBox.at(3).size()�Ł@TextBox[3][]�̉E���̗v�f�����B���.
	if( TextBoxNum > GetInstance()->TextBox.size() )return D3DXVECTOR2( 7, 3 );;
	if( TwoArray > GetInstance()->TextBox.at( TextBoxNum ).size() )return D3DXVECTOR2( 7, 3 );

	string Tmp = GetInstance()->TextBox[TextBoxNum][TwoArray];
	int X = 0;
	int Y = 0;
	int NegativNumX = 1;
	int NegativNumY = 5;
	int Count = 0;
	if( Tmp == " " || Tmp == "�@" ){
		return D3DXVECTOR2( 0, 3 );
	}
	// X = �Z�Z �̐��l�͍ő剡��.W 1600 �Ȃ� 16.
	for( size_t i = 0; i < GetInstance()->m_HiragnaBoxSize - 1; i += 2 ){
		if( Tmp.c_str() == GetInstance()->HiraganaBox.substr( i, 2 ) ){
			X = 16 - NegativNumX;
			Y = 5 - NegativNumY;
			return D3DXVECTOR2( (FLOAT)X, (FLOAT)Y );
		} else{
			Count++;
			// �u���v�̉��ɂ͉����Ȃ��̂ł�����Ƃ��}�b�N�X�ɂ���.
			if( GetInstance()->HiraganaBox.substr( i, 2 ) == "��" ){
				Count = 5;
			}
			NegativNumY -= 1;
		}
		if( Count == 5 ){
			NegativNumX += 1;
			NegativNumY = 5;
			Count = 0;
		}
	}
	return D3DXVECTOR2( 7, 3 );


}

bool CTutorialTextLoad::SearchFiristAsterisk( int ArrayNum )
{
	if( ArrayNum < 0 ){
		return false;
	}
	if( GetInstance()->m_vbAsteriskFlag[ArrayNum] == true ){
		return true;
	}
	if( GetInstance()->TextBox[ArrayNum][0] == "��" ){
		//�擪��"��"���폜.
		GetInstance()->TextBox[ArrayNum].erase( GetInstance()->TextBox[ArrayNum].begin() );
		GetInstance()->m_vbAsteriskFlag[ArrayNum] = true;
		return true;
	}
	return false;
}

void CTutorialTextLoad::InitHiraganaBox()
{
	HiraganaBox = "����������"
		"����������"
		"����������"
		"�����Ă�"
		"�Ȃɂʂ˂�"
		"�͂Ђӂւ�"
		"�܂݂ނ߂�"
		"����H�I"
		"������"
		"�����B�A"
		"����������"
		"����������"
		"�����Âł�"
		"�΂тԂׂ�"
		"�ς҂Ղ؂�"
		"�[��";
}

