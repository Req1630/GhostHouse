#ifndef C_TUTORIAL_TEXTLOAD_H
#define C_TUTORIAL_TEXTLOAD_H

#include "..\..\SingletonBase\SingletonBase.h"
#include "..\..\Main\Global.h"

using namespace std;

class CTutorialTextLoad : public CSingletonBase<CTutorialTextLoad>
{
private:
	const char* TEXT_FILENAME = "Data\\Tutorial.txt";	// テキストパス.
public:
	CTutorialTextLoad();
	~CTutorialTextLoad();

	static void TextDataLoad();

	static int GetTextArraySize()
	{
		return GetInstance()->TextBox.size();
	}

	static int GetTextTwoArraySize( int i )
	{
		return GetInstance()->TextBox.at( i ).size();
	}

	static D3DXVECTOR2 SearchCharPosition( size_t TextBoxNum, size_t TwoArray );

	static bool SearchFiristAsterisk( int ArrayNum );
private:
	void InitHiraganaBox();
	//二次元配列.
	vector<vector<string>> TextBox;
	string HiraganaBox;
	size_t	m_HiragnaBoxSize;
	vector<bool>	m_vbAsteriskFlag;

private:
	// コピー・ムーブコンストラクタ, 代入演算子の削除.
	CTutorialTextLoad( const CTutorialTextLoad & ) = delete;
	CTutorialTextLoad& operator = ( const CTutorialTextLoad & ) = delete;
	CTutorialTextLoad( CTutorialTextLoad && ) = delete;
	CTutorialTextLoad& operator = ( CTutorialTextLoad && ) = delete;
};



#endif // !C_TUTORIAL_TEXTLOAD_H
