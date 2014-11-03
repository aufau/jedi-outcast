#ifndef TR_CAPTURE_H
#define TR_CAPTURE_H

#define MAX_BLUR_FRAMES	64
#define MAX_PACK_LEN 16

typedef struct {
	double		blurQuality;
	float		mult[MAX_BLUR_FRAMES];
	float		*accum;
	int		frame;
	int		blurFrames;
	int		blurOverlap;
	int		width;
	int		height;
	int		accumFrame;
	GLenum		glMode;
	qboolean	alpha;
	qboolean	swap;
	qboolean	recording;
} shotData_t;

extern shotData_t	shot;

qboolean	RE_StartVideoRecording(void);
void		RE_StopVideoRecording(void);

void		R_AccumLoad(const byte *captureBuffer, size_t memcount, float *accumBuffer, float multiplier);
void		R_Accum(const byte *captureBuffer, size_t memcount, float *accumBuffer, float multiplier);
void		R_AccumReturn(const float *accumBuffer, size_t memcount, byte *returnBuffer);

#endif // TR_CAPTURE_H
