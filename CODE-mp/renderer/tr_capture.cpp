#include "tr_local.h"

shotData_t	shot;

qboolean RE_StartVideoRecording(void)
{
	int	i, bufferSize;
	double	mu, sigma, f;

	shot.width = glConfig.vidWidth;
	shot.height = glConfig.vidHeight;
	shot.frame = 0;
	shot.blurFrames = r_blurFrames->integer;
	if (shot.blurFrames < 0 || shot.blurFrames > MAX_BLUR_FRAMES) {
		ri.Printf(PRINT_WARNING, "r_blurFrames must be in the 0..%d range.\n", MAX_BLUR_FRAMES);
		return qfalse;
	}
	shot.blurSkipFrames = r_blurSkipFrames->integer;
	if (shot.blurSkipFrames >= shot.blurFrames && shot.blurFrames) {
		ri.Printf(PRINT_WARNING, "r_blurSkipFrames must be less than r_blurFrames\n");
		return qfalse;
	}

	if (!Q_stricmp("GL_RGBA", r_aviFetchMode->string)) {
		shot.alpha = qtrue;
		shot.swap = qtrue;
		shot.glMode = GL_RGBA;
	} else if (!Q_stricmp("GL_RGB", r_aviFetchMode->string)) {
		shot.alpha = qfalse;
		shot.swap = qtrue;
		shot.glMode = GL_RGB;
	} else if (!Q_stricmp("GL_BGR", r_aviFetchMode->string)) {
		shot.alpha = qfalse;
		shot.swap = qfalse;
		shot.glMode = GL_BGR;
	} else if (!Q_stricmp("GL_BGRA", r_aviFetchMode->string)) {
		shot.alpha = qtrue;
		shot.swap = qfalse;
		shot.glMode = GL_BGRA;
	} else {
		ri.Printf(PRINT_ALL, "Unknown r_aviFetchMode value. Using GL_BGRA\n");
		shot.alpha = qfalse;
		shot.swap = qtrue;
		shot.glMode = GL_BGRA;
	}

	bufferSize = (shot.width * 4 * sizeof(float) + MAX_PACK_LEN - 1)
		* shot.height + MAX_PACK_LEN - 1;
	shot.accum = (float *)ri.Malloc(bufferSize, TAG_GENERAL, qtrue);
	if (!shot.accum) {
		ri.Printf(PRINT_WARNING,
			  "Failed to allocate %f megabytes for the accumulation buffer\n",
			  bufferSize / (1024.0 * 1024.0));
		return qfalse;
	}

	if (!Q_stricmp("gaussian", r_blurType->string)) {
		mu = shot.blurFrames / 2.0;
		// Use the 3 sigma rule (where 3 = 2)
		// This covers 0.9545 of the area under our normalized gaussian function
		sigma = shot.blurFrames / 4.0;
		f = sqrt(2) * sigma;
		for (i = 0; i < shot.blurFrames; i++) {
			shot.mult[i] = 0.5*(erf((i+1-mu)/f) - erf((i-mu)/f))
				/ 0.9545;
		}
	} else if (!Q_stricmp("triangular", r_blurType->string)) {
		sigma = (floor(shot.blurFrames / 2.0) + 1.0) * ceil(shot.blurFrames / 2.0);
		for (i = 0; i < shot.blurFrames; i++) {
			shot.mult[i] = min(i+1, shot.blurFrames - i) / sigma;
		}
	} else {
		if (Q_stricmp("uniform", r_blurType->string))
			ri.Printf(PRINT_ALL, "Unrecognised r_blurType value. Using uniform blur.");

		for (i = 0; i < shot.blurFrames; i++) {
			shot.mult[i] = 1.0f / shot.blurFrames;
		}
	}

	return qtrue;
}

void RE_StopVideoRecording(void)
{
	ri.Free(shot.accum);
	shot.frame = 0;
	shot.recording = qfalse;
}

// Make sure that these auto-vectorize nicely with GCC
void R_AccumLoad( const byte *captureBuffer, size_t memcount, float *accumBuffer, float multiplier )
{
	int i;
	multiplier /= 255.0f;
	for(i = 0; i < memcount; i++) {
		accumBuffer[i] = multiplier * captureBuffer[i];
	}
}

void R_Accum( const byte *captureBuffer, size_t memcount, float *accumBuffer, float multiplier )
{
	int	i;
	multiplier /= 255.0f;
	for(i = 0; i < memcount; i++) {
		accumBuffer[i] += multiplier * captureBuffer[i];
	}
}

void R_AccumReturn( const float *accumBuffer, size_t memcount, byte *returnBuffer )
{
	int i;
	for(i = 0; i < memcount; i++) {
		returnBuffer[i] = (byte) (accumBuffer[i] * 255.0f);
	}
}
