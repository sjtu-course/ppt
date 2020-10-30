#include <stdio.h>
#include <iostream>

#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")

#include "opus/opus.h"
#include "wav_head_writer.h"

#pragma warning(disable:4996)
#define FRAME_LEN 20 //encoder frame length: ms
#define INPUT_SAMPLERATE 48000
#define INPUT_CHANNEL 2
#define INPUT_FRAME_SIZE  (INPUT_SAMPLERATE * INPUT_CHANNEL * FRAME_LEN)/1000
int main() {
	FILE *origin_file_id;
	FILE *opus_file_id;
	FILE *wav_file_id;
	int error = OPUS_OK;
	OpusEncoder* opus_encoder_handle = NULL;
	OpusDecoder* opus_decoder_handle = NULL;
	int encode_sample_rate, decode_sample_rate, encode_channel, decode_channel;
	int encode_bitrate;
	short input_buffer[INPUT_FRAME_SIZE];
	uint8_t encoded_buffer[1024];
	short decoded_buffer[INPUT_FRAME_SIZE];
	//creat opus encoder
	encode_sample_rate = INPUT_SAMPLERATE;
	encode_channel = INPUT_CHANNEL;
	encode_bitrate = 128000;
	uint8_t header[kWavHeaderSize];

	opus_encoder_handle = opus_encoder_create(encode_sample_rate, encode_channel, OPUS_APPLICATION_AUDIO, &error);
	if (OPUS_OK != error) {
		std::cout << "open opus encoder error, error code: " << error << std::endl;
		getchar();
		return -1;
	}
	printf("Finish creating opus encoder.\n");
	//set bitrate
	opus_encoder_ctl(opus_encoder_handle, OPUS_SET_BITRATE(encode_bitrate));

	//creat opus decoder
	decode_sample_rate = 48000;
	decode_channel = 2;
	opus_decoder_handle = opus_decoder_create(decode_sample_rate, decode_channel, &error);
	if (OPUS_OK != error) {
		std::cout << "open opus decoder error, error code: " << error << std::endl;
		getchar();
		return -1;
	}
	printf("Finish creating opus decoder.\n");

	//open file
	//origin.pcm foramt: 48k/stereo/16bit
	origin_file_id = fopen("../resource/origin.pcm", "rb");
	opus_file_id = fopen("../resource/out/encoded.opus", "wb");
	wav_file_id = fopen("../resource/out/decoded.wav", "wb");

	int process_cnt = 0;
	int res = 0;
	bool wave_head_has_written = false;
	size_t read_samples = 0;
	while (1) {
		int read_size = fread(input_buffer, sizeof(short), INPUT_FRAME_SIZE, origin_file_id);
		if (read_size != INPUT_FRAME_SIZE) {
			break;
		}
		process_cnt++;
		read_samples += read_size;
		if (process_cnt % 1000 == 0) {
			std::cout << "processing...., frame count:" << process_cnt << std::endl;
		}
		//encode process
		res = opus_encode(opus_encoder_handle, input_buffer, INPUT_FRAME_SIZE / 2, encoded_buffer, 1024);
		if (res > 0) {
			fwrite(encoded_buffer, 1, res, opus_file_id);
		}

		//decode process
		res = opus_decode(opus_decoder_handle, encoded_buffer, res, decoded_buffer, INPUT_FRAME_SIZE, 0);
		if (!wave_head_has_written) {
			wave_head_has_written = true;
			WriteWavHeader(header, decode_channel, decode_sample_rate, kWavFormatPcm, sizeof(short), 0);
			fwrite(header, kWavHeaderSize, 1, wav_file_id);
		}
		if (res > 0) {
			fwrite(decoded_buffer, 2, res * decode_channel, wav_file_id);
		}

		//break;
	}

	printf("\nAdding wav header for the decoded pcm data.\n");

	fseek(wav_file_id, 0, SEEK_SET);
	WriteWavHeader(header, decode_channel, decode_sample_rate, kWavFormatPcm, sizeof(short), read_samples);
	fwrite(header, kWavHeaderSize, 1, wav_file_id);
	std::cout << "process over, total process frame count:" << process_cnt << ". press any key to play the decoded wav file." << std::endl;

	fclose(origin_file_id);
	fclose(opus_file_id);
	fclose(wav_file_id);
	getchar();

	PlaySound(TEXT("..\\resource\\out\\decoded.wav"), NULL, SND_FILENAME | SND_ASYNC);

	//destroy resources.
	if (opus_encoder_handle) {
		opus_encoder_destroy(opus_encoder_handle);
		printf("Finish destroying opus encoder.\n");
	}
	if (opus_decoder_handle) {
		opus_decoder_destroy(opus_decoder_handle);
		printf("Finish destroying opus decoder.\n");
	}
	printf("press any key to exit.");
	getchar();

	return 0;
}