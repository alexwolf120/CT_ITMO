#include "myffmpeg.h"

#include "myfftw.h"
#include "return_codes.h"
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>

static AudioFile open_audio_file(const char *filename, AVFormatContext **format_ctx, AVCodecContext **codec_ctx)
{
	const AVCodec *codec = NULL;
	int32_t audio_stream_index = -1;
	AudioFile audio_file;
	if (avformat_open_input(format_ctx, filename, NULL, NULL) < 0)
	{
		audio_file.str_error = "Cannot open file";
		audio_file.err = ERROR_CANNOT_OPEN_FILE;
		return audio_file;
	}
	if (avformat_find_stream_info(*format_ctx, NULL) < 0)
	{
		audio_file.str_error = "Cannot find stream info";
		audio_file.err = ERROR_DATA_INVALID;
		return audio_file;
	}
	for (size_t i = 0; i < (*format_ctx)->nb_streams; i++)
	{
		if ((*format_ctx)->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
		{
			audio_stream_index = i;
			break;
		}
	}
	if (audio_stream_index == -1)
	{
		audio_file.str_error = "Unsupported stream index";
		audio_file.err = ERROR_UNSUPPORTED;
		return audio_file;
	}
	enum AVCodecID codec_id = (*format_ctx)->streams[audio_stream_index]->codecpar->codec_id;
	switch (codec_id)
	{
	case AV_CODEC_ID_FLAC:
	case AV_CODEC_ID_MP2:
	case AV_CODEC_ID_MP3:
	case AV_CODEC_ID_OPUS:
	case AV_CODEC_ID_AAC:
		break;
	default:
		audio_file.str_error = "Unsupported audio format";
		audio_file.err = ERROR_FORMAT_INVALID;
		return audio_file;
	}
	codec = avcodec_find_decoder((*format_ctx)->streams[audio_stream_index]->codecpar->codec_id);
	if (codec == NULL)
	{
		audio_file.str_error = "Codec not found";
		audio_file.err = ERROR_UNSUPPORTED;
		return audio_file;
	}
	*codec_ctx = avcodec_alloc_context3(codec);
	if (*codec_ctx == NULL)
	{
		audio_file.str_error = "Memory not allocated.";
		audio_file.err = ERROR_NOTENOUGH_MEMORY;
		return audio_file;
	}
	if (avcodec_parameters_to_context(*codec_ctx, (*format_ctx)->streams[audio_stream_index]->codecpar) < 0)
	{
		audio_file.str_error = "Error setting codec context";
		audio_file.err = ERROR_DATA_INVALID;
		return audio_file;
	}
	if (avcodec_open2(*codec_ctx, codec, NULL) < 0)
	{
		audio_file.str_error = "Error openning codec context";
		audio_file.err = ERROR_UNSUPPORTED;
		return audio_file;
	}
	audio_file.err = SUCCESS;
	return audio_file;
}
static size_t check_send_packet(size_t codes)
{
	switch (-codes)
	{
	case AVERROR(ENOMEM):
		fputs("Failed to add packet to internal queue, or similar\n", stderr);
		return ERROR_NOTENOUGH_MEMORY;
	case AVERROR(EINVAL):
		fputs("Codec not opened, it is an encoder, or requires flush\n", stderr);
		return ERROR_DATA_INVALID;
	case AVERROR_EOF:
		fputs("The decoder has been flushed, and no new packets can be sent to it\n", stderr);
		return ERROR_UNSUPPORTED;
	default:
		fputs("Memory not allocated.\n", stderr);
		return ERROR_UNKNOWN;
	}
}
static Buffer
	getsamplebuf(AVFormatContext *format_ctx, AVPacket *packet, AVCodecContext *codec_ctx, AVFrame *frame, double *buf, size_t chan_ind, size_t buf_ind)
{
	Buffer res;
	while (av_read_frame(format_ctx, packet) >= 0)
	{
		size_t cod_sp = avcodec_send_packet(codec_ctx, packet);
		if (cod_sp < 0)
		{
			res.err = check_send_packet(cod_sp);
			if (buf != NULL)
			{
				free(buf);
			}
			return res;
		}
		avcodec_receive_frame(codec_ctx, frame);
		buf = realloc(buf, (buf_ind + frame->nb_samples) * sizeof(double));
		if (buf == NULL)
		{
			fputs("Memory not allocated.\n", stderr);
			res.err = ERROR_NOTENOUGH_MEMORY;
			return res;
		}
		for (size_t j = 0; j < frame->nb_samples; j++)
		{
			buf[buf_ind] = frame->data[chan_ind][j];
			buf_ind++;
		}
		av_packet_unref(packet);
	}
	res.array = buf;
	res.size = buf_ind;
	res.err = SUCCESS;
	return res;
}
static Cross_samp init(
	int argc,
	char *argv[],
	AVFormatContext **format_ctx1,
	AVFormatContext **format_ctx2,
	AVCodecContext **codec_ctx1,
	AVCodecContext **codec_ctx2,
	AudioFile *audio1,
	AudioFile *audio2,
	int *chan_ind_second)
{
	Cross_samp samp;

	av_log_set_level(AV_LOG_QUIET);

	*audio1 = open_audio_file(argv[1], format_ctx1, codec_ctx1);
	if (audio1->err != SUCCESS)
	{
		samp.err = audio1->err;
		fprintf(stderr, "%s\n", audio1->str_error);
		return samp;
	}

	if (argc == 2)
	{
		*audio2 = open_audio_file(argv[1], format_ctx2, codec_ctx2);
		*chan_ind_second = 1;
	}
	else
	{
		*audio2 = open_audio_file(argv[2], format_ctx2, codec_ctx2);
	}

	if (audio2->err != SUCCESS)
	{
		samp.err = audio2->err;
		fprintf(stderr, "%s\n", audio2->str_error);
		return samp;
	}

	samp.err = SUCCESS;
	return samp;
}
static Cross_samp
	open_audio(AVFormatContext *format_ctx1, AVFormatContext *format_ctx2, AVCodecContext *codec_ctx1, AVCodecContext *codec_ctx2, int chan_ind_second, Cross_samp samp)
{
	AVPacket *packet1 = NULL;
	AVPacket *packet2 = NULL;
	AVFrame *frame1 = NULL;
	AVFrame *frame2 = NULL;
	Buffer res1, res2;
	Over over_sam;
	Fast fast_sam;
	double *buf1 = NULL;
	double *buf2 = NULL;
	int sample_rate1 = codec_ctx1->sample_rate;
	int sample_rate2 = codec_ctx2->sample_rate;
	size_t size1 = 0;
	size_t size2 = 0;

	frame1 = av_frame_alloc();
	frame2 = av_frame_alloc();
	if (!frame2 || !frame1)
	{
		fputs("Error allocating frame2\n", stderr);
		samp.err = ERROR_UNSUPPORTED;
		goto free_array;
	}

	packet1 = av_packet_alloc();
	packet2 = av_packet_alloc();
	if (!packet2 || !packet1)
	{
		fputs("Error allocating packet2\n", stderr);
		samp.err = ERROR_UNSUPPORTED;
		goto free_array;
	}

	buf1 = malloc(size1 * sizeof(double));
	buf2 = malloc(size2 * sizeof(double));
	if (buf1 == NULL || buf2 == NULL)
	{
		fputs("Memory not allocated.\n", stderr);
		samp.err = ERROR_NOTENOUGH_MEMORY;
		goto free_array;
	}

	res1 = getsamplebuf(format_ctx1, packet1, codec_ctx1, frame1, buf1, 0, size1);
	if (res1.err != SUCCESS)
	{
		samp.err = res1.err;
		goto free_array;
	}
	buf1 = res1.array;
	size1 = res1.size;

	if (chan_ind_second == 1 && frame1->ch_layout.nb_channels != 2)
	{
		fputs("Notenought channels\n", stderr);
		samp.err = ERROR_FORMAT_INVALID;
		goto free_array;
	}

	res2 = getsamplebuf(format_ctx2, packet2, codec_ctx2, frame2, buf2, chan_ind_second, size2);
	if (res2.err != SUCCESS)
	{
		samp.err = res2.err;
		goto free_array;
	}
	buf2 = res2.array;
	size2 = res2.size;

	size_t dif_sampl = 0;
	size_t old_size = 0;
	if (sample_rate1 != sample_rate2)
	{
		if (sample_rate1 > sample_rate2)
		{
			old_size = size2;
			size2 = (size2 * sample_rate1) / sample_rate2;
			sample_rate2 = sample_rate1;
			dif_sampl = 2;
		}
		else
		{
			old_size = size1;
			size1 = (size1 * sample_rate2) / sample_rate1;
			sample_rate1 = sample_rate2;
			dif_sampl = 1;
		}
	}

	size_t sizeres = size1 + size2 - 1;
	if (dif_sampl == 1)
	{
		over_sam = oversampling(old_size, size1, buf1);
		buf1 = over_sam.arr;
		if (over_sam.err != SUCCESS)
		{
			samp.err = over_sam.err;
			goto free_array;
		}
	}
	else if (dif_sampl == 2)
	{
		over_sam = oversampling(old_size, size2, buf2);
		buf2 = over_sam.arr;
		if (over_sam.err != SUCCESS)
		{
			samp.err = over_sam.err;
			goto free_array;
		}
	}

	double *new_buf1;
	double *new_buf2;
	new_buf1 = realloc(buf1, sizeres * sizeof(double));
	new_buf2 = realloc(buf2, sizeres * sizeof(double));
	if (new_buf1 == NULL || new_buf2 == NULL)
	{
		fputs("Memory not allocated.\n", stderr);
		samp.err = ERROR_NOTENOUGH_MEMORY;
		goto free_array;
	}
	buf1 = new_buf1;
	buf2 = new_buf2;

	memset(&buf1[size1], 0, sizeof(double) * (sizeres - size1));
	memset(&buf2[size2], 0, sizeof(double) * (sizeres - size2));

	fast_sam = fast_cross_correlation(buf1, buf2, sizeres);
	if (fast_sam.err != SUCCESS)
	{
		samp.err = fast_sam.err;
		goto free_array;
	}

	samp.samples = fast_sam.index;
	samp.sample_rate = sample_rate1;
	samp.time = (int)((double)samp.samples * 1000.0 / samp.sample_rate);
	samp.err = SUCCESS;

free_array:
	if (buf1 != NULL)
	{
		free(buf1);
	}
	if (buf2 != NULL)
	{
		free(buf2);
	}
	if (frame1 != NULL)
	{
		av_frame_free(&frame1);
	}
	if (frame2 != NULL)
	{
		av_frame_free(&frame2);
	}
	if (packet1 != NULL)
	{
		av_packet_free(&packet1);
	}
	if (packet2 != NULL)
	{
		av_packet_free(&packet2);
	}

	return samp;
}
void closeup(AVFormatContext **format_ctx1, AVFormatContext **format_ctx2, AVCodecContext **codec_ctx1, AVCodecContext **codec_ctx2)
{
	if (*format_ctx1 != NULL)
	{
		avformat_close_input(format_ctx1);
	}
	if (*format_ctx2 != NULL)
	{
		avformat_close_input(format_ctx2);
	}
	if (*codec_ctx1 != NULL)
	{
		avcodec_free_context(codec_ctx1);
	}
	if (*codec_ctx2 != NULL)
	{
		avcodec_free_context(codec_ctx2);
	}
}
Cross_samp cross(int argc, char *argv[])
{
	AVFormatContext *format_ctx1 = NULL;
	AVFormatContext *format_ctx2 = NULL;
	AVCodecContext *codec_ctx1 = NULL;
	AVCodecContext *codec_ctx2 = NULL;
	AudioFile audio1;
	AudioFile audio2;
	int chan_ind_second = 0;
	Cross_samp samp;

	samp = init(argc, argv, &format_ctx1, &format_ctx2, &codec_ctx1, &codec_ctx2, &audio1, &audio2, &chan_ind_second);
	if (samp.err != SUCCESS)
	{
		closeup(&format_ctx1, &format_ctx2, &codec_ctx1, &codec_ctx2);
		return samp;
	}

	samp = open_audio(format_ctx1, format_ctx2, codec_ctx1, codec_ctx2, chan_ind_second, samp);
	closeup(&format_ctx1, &format_ctx2, &codec_ctx1, &codec_ctx2);

	return samp;
}
