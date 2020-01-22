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
	// ファイルを開く.
	fstream filestream( GetInstance()->TEXT_FILENAME );
	if( !filestream.is_open() ) return;

	int i = 0;
	// ファイルを読み込む.
	while( !filestream.eof() ){
		string buffer;
		//Textファイルから読み取った1行をstring変数「buffer]
		filestream >> buffer;
		//要素を増やして初期化.
		GetInstance()->TextBox.emplace_back();
		GetInstance()->m_vbAsteriskFlag.emplace_back( false );
		//日本語は2バイトなので、substrで
		//(開始位置「あいうえお」で「う」なら4, バイト数(日本語なら2)を指定して取り出す.
		for( size_t StartNum = 0; StartNum < buffer.length(); StartNum += 2 ){
			GetInstance()->TextBox[i].emplace_back( buffer.substr( StartNum, 2 ) );
		}
		i++;
	}

	GetInstance()->m_HiragnaBoxSize = GetInstance()->HiraganaBox.size();

}

D3DXVECTOR2 CTutorialTextLoad::SearchCharPosition( size_t TextBoxNum, size_t TwoArray )
{
	//TextBox.at(3).size()で　TextBox[3][]の右側の要素数が撮れる.
	if( TextBoxNum > GetInstance()->TextBox.size() )return D3DXVECTOR2( 7, 3 );;
	if( TwoArray > GetInstance()->TextBox.at( TextBoxNum ).size() )return D3DXVECTOR2( 7, 3 );

	string Tmp = GetInstance()->TextBox[TextBoxNum][TwoArray];
	int X = 0;
	int Y = 0;
	int NegativNumX = 1;
	int NegativNumY = 5;
	int Count = 0;
	if( Tmp == " " || Tmp == "　" ){
		return D3DXVECTOR2( 0, 3 );
	}
	// X = 〇〇 の数値は最大横幅.W 1600 なら 16.
	for( size_t i = 0; i < GetInstance()->m_HiragnaBoxSize - 1; i += 2 ){
		if( Tmp.c_str() == GetInstance()->HiraganaBox.substr( i, 2 ) ){
			X = 16 - NegativNumX;
			Y = 5 - NegativNumY;
			return D3DXVECTOR2( (FLOAT)X, (FLOAT)Y );
		} else{
			Count++;
			// 「っ」の下には何もないのでかうんとをマックスにする.
			if( GetInstance()->HiraganaBox.substr( i, 2 ) == "っ" ){
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
	if( GetInstance()->TextBox[ArrayNum][0] == "＊" ){
		//先頭の"＊"を削除.
		GetInstance()->TextBox[ArrayNum].erase( GetInstance()->TextBox[ArrayNum].begin() );
		GetInstance()->m_vbAsteriskFlag[ArrayNum] = true;
		return true;
	}
	return false;
}

void CTutorialTextLoad::InitHiraganaBox()
{
	HiraganaBox = "あいうえお"
		"かきくけこ"
		"さしすせそ"
		"たちつてと"
		"なにぬねの"
		"はひふへほ"
		"まみむめも"
		"やゆよ？！"
		"らりるれろ"
		"わをん。、"
		"がぎぐげご"
		"ざじずぜぞ"
		"だぢづでど"
		"ばびぶべぼ"
		"ぱぴぷぺぽ"
		"ーっ";
}

