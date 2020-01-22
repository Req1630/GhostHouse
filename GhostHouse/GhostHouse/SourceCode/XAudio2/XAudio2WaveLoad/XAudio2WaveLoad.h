#ifndef C_XAUDIO2_WAVELOAD_H
#define C_XAUDIO2_WAVELOAD_H

#include "..\XAudio2.h"
#include <string>
class clsXAudio2WaveLoad
{
public:
	clsXAudio2WaveLoad();
	~clsXAudio2WaveLoad();

	//// Wavファイルの読み込み.
	//bool LoadFromFile(const char* FileName);

	// WAVE フォーマットを取得
	const WAVEFORMATEX&     GetWaveFmtEx()const;

	//// ファイル名の取得
	//const char*             GetFileName()const;

	//// Wave データを取得
	//const BYTE*             GetWaveData()const;

	//// Wave データのサイズを取得
	//const size_t            GetWaveSize()const;

	bool Open(const char * filename);

	bool GetWaveFormat(const char* filename);

	size_t GetSamples(const char* filename);

	size_t ReadRaw(const std::size_t start, const std::size_t samples, void * buffer, const char* filename);

	size_t ReadNormalized(const std::size_t start, const std::size_t samples, float * left, float * right, const char* filename);

	const char* GetFileName() {
		return Tmp;
	}
	bool Close();
private:
	//HMMIO               m_handle;			// MMIOハンドル.
	//WAVEFORMATEX		m_WaveForMatex;		// 読み込んだWaveデータを入れる構造体.
	//BYTE*	            m_waveData;         // Sound Data	
	//size_t              m_waveDataSize;		// Dataチャンクを彫り込む.
	//char                m_filename[512];	// 読み込むWaveのファイルの名前.

	//bool Open(const char* filename);		// WAVE ファイルオープン
	//bool Close();                           // WAVE ファイルクローズ

	//bool ReadToWaveFmtEx(size_t szFmtChunk);
	//bool ReadToWaveData(size_t szDataSize);

	const char* Tmp;

	// ファイルハンドル
	FILE* m_pFile;
	// フォーマット情報を取得済みか
	bool m_hasGotWaveFormat;
	// フォーマット情報
	WAVEFORMATEX m_WaveFormat;
	// data チャンク内先頭サンプルへのオフセット
	long m_firstSampleOffset;
	// data チャンクサイズ
	size_t m_dataChunkSize;
	// data チャンク内サンプル数
	size_t m_dataChunkSamples;


	

};




#endif //#ifndef C_XAUDIO2_WAVELOAD_H
