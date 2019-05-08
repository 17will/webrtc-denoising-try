
//ֻ���ڵ�ͨ��,ע���������ͣ�ֻ��8k16λ
#define WEBRTC_WIN
#define WEBRTC_AUDIO_PROCESSING_ONLY_BUILD
#include "webrtc\modules\audio_processing\ns\include\noise_suppression.h"
#define DR_WAV_IMPLEMENTATION//ʹ��drwav���붨�������
#include <vector>
#include"dr_wav.h"
#include "AudioFile.h"
#include<cstdio>
#include<cstdlib>
#include <cstdint>
int main() {
	
	AudioFile<float> audioFile, audioOut;
	audioFile.load("1.wav");
	unsigned int channels = audioFile.getNumChannels();
	unsigned int sampleRate = audioFile.getSampleRate();
	int numSamples = audioFile.getNumSamplesPerChannel();

	int frameSize = sampleRate / 100;
	int totalFrame = numSamples / frameSize;

	audioOut.setBitDepth(audioFile.getBitDepth());
	audioOut.setNumChannels(channels);
	audioOut.setSampleRate(sampleRate);

	float *in = new float[numSamples];

	for (size_t i = 0; i <numSamples; i++)
	{
		in[i] = audioFile.samples[0][i];
	}

	NsHandle *nshandle = WebRtcNs_Create();
	int status = WebRtcNs_Init(nshandle, sampleRate);
	if (status != 0) {
		printf("WebRtcNs_Init fail\n");
		return -1;
	}
	int level = 3;
	WebRtcNs_set_policy(nshandle, level);

	int len = totalFrame * frameSize;
	audioOut.setNumSamplesPerChannel(len);
	float *out = new float[len];
	float *pin = &in[0];
	float *pout = &out[0];
	//float *outout = new float[len];
	//std::vector<std::vector<float>> outout(1, std::vector<float>(len));
	for (int i = 0; i < totalFrame; i++) {
		WebRtcNs_Analyze(nshandle, pin);
		WebRtcNs_Process(nshandle, &pin, 1, &pout);

		for (int j = 0; j < frameSize; j++)
		{
			audioOut.samples[0][i*frameSize + j] = pout[j];
		}

		pin += frameSize;
		pout += frameSize;
	}

	WebRtcNs_Free(nshandle);

	if (!audioOut.save("2.wav")) {
		printf("Write wav file failed.\n");
		return -1;
	}
	else {
		printf("success!\n");
	}

	//audioOut.samples = outout;

	return 0;
	/*
	unsigned int channels = 0;
	unsigned int sampleRate = 0;
	drwav_uint64 totalSampleCount = 0;
	drwav_int16* pSampleData = drwav_open_and_read_file_s16("1.wav", &channels, &sampleRate, &totalSampleCount);//Ĭ�ϵľ���ת��16bit,����д��ʱ��ҲҪ�ĳ�16bit
	if (pSampleData == NULL) {
		printf("open failed");//Error opening and reading WAV file.
		getchar();
		return 0;
	}
	float *in = new float[totalSampleCount];

	for (size_t i = 0; i <totalSampleCount; i++)
	{
		in[i] = pSampleData[i];
	}
	int framesize = sampleRate / 100;
	drwav_int16 nframe = totalSampleCount / framesize;

	int level = 3;
	NsHandle *nshandle = WebRtcNs_Create();
	int status = WebRtcNs_Init(nshandle, sampleRate);

	if (status != 0) {
		printf("WebRtcNs_Init fail\n");
		return -1;
	}

	WebRtcNs_set_policy(nshandle, level);


	drwav_uint64 len = nframe * framesize;
	float *out = new float[len];
	float*pin = &in[0];
	float*pout = &out[0];
	drwav_int16 *outout = new drwav_int16[len];
	for (int i = 0; i <nframe; i++)
	{

		//float  *outout=new float[framesize];
		WebRtcNs_Analyze(nshandle, pin);
		WebRtcNs_Process(nshandle, &pin, 1, &pout);

		for (int j = 0; j < framesize; j++)
		{
			outout[i*framesize + j] = pout[j];
		}

		pin += framesize;
		pout += framesize;
	}



	WebRtcNs_Free(nshandle);

	drwav_data_format format;
	format.container = drwav_container_riff;     // <-- drwav_container_riff = normal WAV files, drwav_container_w64 = Sony Wave64.
	format.format = DR_WAVE_FORMAT_PCM;          // <-- Any of the DR_WAVE_FORMAT_* codes.
	format.channels = channels;
	format.sampleRate = sampleRate;
	format.bitsPerSample = 16;
	drwav* pWav = drwav_open_file_write("2.wav", &format);

	drwav_uint64 samplesWritten = drwav_write(pWav, len, outout);
	drwav_free(pSampleData);
	if (samplesWritten>0)
	{
		printf("success");
	}



	getchar();
	return 0;*/

}


