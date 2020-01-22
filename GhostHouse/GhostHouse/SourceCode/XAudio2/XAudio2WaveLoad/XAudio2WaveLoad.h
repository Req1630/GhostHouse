#ifndef C_XAUDIO2_WAVELOAD_H
#define C_XAUDIO2_WAVELOAD_H

#include "..\XAudio2.h"
#include <string>
class clsXAudio2WaveLoad
{
public:
	clsXAudio2WaveLoad();
	~clsXAudio2WaveLoad();

	//// Wav�t�@�C���̓ǂݍ���.
	//bool LoadFromFile(const char* FileName);

	// WAVE �t�H�[�}�b�g���擾
	const WAVEFORMATEX&     GetWaveFmtEx()const;

	//// �t�@�C�����̎擾
	//const char*             GetFileName()const;

	//// Wave �f�[�^���擾
	//const BYTE*             GetWaveData()const;

	//// Wave �f�[�^�̃T�C�Y���擾
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
	//HMMIO               m_handle;			// MMIO�n���h��.
	//WAVEFORMATEX		m_WaveForMatex;		// �ǂݍ���Wave�f�[�^������\����.
	//BYTE*	            m_waveData;         // Sound Data	
	//size_t              m_waveDataSize;		// Data�`�����N�𒤂荞��.
	//char                m_filename[512];	// �ǂݍ���Wave�̃t�@�C���̖��O.

	//bool Open(const char* filename);		// WAVE �t�@�C���I�[�v��
	//bool Close();                           // WAVE �t�@�C���N���[�Y

	//bool ReadToWaveFmtEx(size_t szFmtChunk);
	//bool ReadToWaveData(size_t szDataSize);

	const char* Tmp;

	// �t�@�C���n���h��
	FILE* m_pFile;
	// �t�H�[�}�b�g�����擾�ς݂�
	bool m_hasGotWaveFormat;
	// �t�H�[�}�b�g���
	WAVEFORMATEX m_WaveFormat;
	// data �`�����N���擪�T���v���ւ̃I�t�Z�b�g
	long m_firstSampleOffset;
	// data �`�����N�T�C�Y
	size_t m_dataChunkSize;
	// data �`�����N���T���v����
	size_t m_dataChunkSamples;


	

};




#endif //#ifndef C_XAUDIO2_WAVELOAD_H
