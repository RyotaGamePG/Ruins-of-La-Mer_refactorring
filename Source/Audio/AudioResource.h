#pragma once

#include <vector>
#include <Windows.h>

// �I�[�f�B�I���\�[�X
class AudioResource
{
public:
	AudioResource(const char* filename);
	~AudioResource();

	// �f�[�^�擾
	UINT8* GetAudioData() { return data.data(); }

	// �f�[�^�T�C�Y�擾
	UINT32 GetAudioBytes() const { return static_cast<UINT32>(data.size()); }

	// WAVE�t�H�[�}�b�g�擾
	const WAVEFORMATEX& GetWaveFormat() const { return wfx; }

private:
	// RIFF�w�b�_
	struct Riff
	{
		UINT32				tag;			// RIFF�`���̎��ʎq 'RIFF'
		UINT32				size;			// ����ȍ~�̃t�@�C���T�C�Y(�t�@�C���T�C�Y - 8)
		UINT32				type;			// RIFF�̎�ނ�\�����ʎq 'WAVE'
	};

	// �`�����N
	struct Chunk
	{
		UINT32				tag;			// �`�����N�`���̎��ʎq 'fmt ' 'data'
		UINT32				size;			// �f�[�^�T�C�Y('fmt '���j�APCM�Ȃ��16 'data'�g�`�f�[�^�T�C�Y)
	};

	// fmt �`�����N
	struct Fmt
	{
		UINT16				fmtId;			// �t�H�[�}�b�gID(���j�APCM�Ȃ��1)
		UINT16				channel;		// �`�����l����(���m����:1 �X�e���I:2)
		UINT32				sampleRate;		// �T���v�����O���[�g(44.1kHz�Ȃ�44100)
		UINT32				transRate;		// �f�[�^���x(Byte/sec) [ 44.1kHz 16bit �X�e���I 44100�~2�~2 ]
		UINT16				blockSize;		// �u���b�N�T�C�Y(Byte/sample�~�`�����l����)
		UINT16				quantumBits;	// �T���v��������̃r�b�g��(bit/sample)
	};

	Riff					riff;
	Fmt						fmt;
	std::vector<UINT8>		data;
	WAVEFORMATEX			wfx;

};
