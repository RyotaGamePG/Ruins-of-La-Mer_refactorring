#include "Misc.h"
#include "Audio/AudioSource.h"

// �R���X�g���N�^
AudioSource::AudioSource(IXAudio2* xaudio, std::shared_ptr<AudioResource>& resource, bool loop)
	: resource(resource)
{
	HRESULT hr;

	// �\�[�X�{�C�X�𐶐�
	hr = xaudio->CreateSourceVoice(&sourceVoice, &resource->GetWaveFormat());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	// �\�[�X�{�C�X�Ƀf�[�^�𑗐M
	XAUDIO2_BUFFER buffer = { 0 };
	buffer.AudioBytes = resource->GetAudioBytes();
	buffer.pAudioData = resource->GetAudioData();
	buffer.LoopCount = loop ? XAUDIO2_LOOP_INFINITE : 0;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	
	sourceVoice->SubmitSourceBuffer(&buffer);
}

// �f�X�g���N�^
AudioSource::~AudioSource()
{
	if (sourceVoice != nullptr)
	{
		sourceVoice->DestroyVoice();
		sourceVoice = nullptr;
	}
}

// �Đ�
void AudioSource::Play()
{
	HRESULT hr = sourceVoice->Start();
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	sourceVoice->SetVolume(volume);
}

// ��~
void AudioSource::Stop()
{
	sourceVoice->Stop();
}


//���ʒ���
void AudioSource::SetVolume(float volume)
{
	this->volume = volume;
}
