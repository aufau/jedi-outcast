#include "tr_local.h"

shotData_t	shot;

/*
**  Calculate the normalized error function with sigma parameter
*/
double myerf( double x, double sigma )
{
	double f = sqrt(2) * sigma;
	return 0.5 * erff(x / f);
}

qboolean initBlurPattern( void )
{
	int	i;
	int	cutoff;

	if (!Q_stricmp("best", r_blurQuality->string)) {
		shot.blurQuality = 1.0;
	} else if (!Q_stricmp("high", r_blurQuality->string)) {
		shot.blurQuality = 0.95;
	} else if (!Q_stricmp("medium", r_blurQuality->string)) {
		shot.blurQuality = 0.86;
	} else if (!Q_stricmp("low", r_blurQuality->string)) {
		shot.blurQuality = 0.68;
	} else if (r_blurQuality->integer <= 0 || r_blurQuality->integer > 100) {
		ri.Printf(PRINT_WARNING,
			  "r_blurQuality can be set to low, medium, high, best or a number in the 1..100 range.\n");
		return qfalse;
	} else {
		shot.blurQuality = r_blurQuality->integer * 0.01;
	}

	// Both gaussian and triangular blur types are approximating
	// the continuous "reality".
	if (!Q_stricmp("gaussian", r_blurType->string)) {
		double	sigma;
		double	mu;

		// Use the 3 sigma rule
		sigma = shot.blurFrames / 6.0;
		// These steps are just approximations.
		// We'd need an inverse error function for arbitrary blurQuality.
		if (shot.blurQuality >= 0.99)		// 3 sigma
			cutoff = 0;
		else if (shot.blurQuality >= 0.95)	// 2 sigma
			cutoff = shot.blurFrames / 3;
		else if (shot.blurQuality >= 0.86)	// 1.5 sigma
			cutoff = shot.blurFrames / 2;
		else					// 1 sigma
			cutoff = shot.blurFrames * 2 / 3;

		shot.blurFrames -= cutoff;
		shot.blurOverlap -= cutoff;
		shot.blurQuality = 2.0 * myerf(shot.blurFrames * 0.5, sigma);


		mu = shot.blurFrames / 2.0;
		for (i = 0; i < shot.blurFrames; i++) {
			shot.mult[i] = (myerf(i + 1 - mu, sigma) - myerf(i - mu, sigma))
				/ shot.blurQuality;
		}
	} else if (!Q_stricmp("triangular", r_blurType->string)) {
		double invArea;
		// Think that frames are in the middle of their respective unit intervals
		// and the "real" right triangle starts at -0.5 and ends at blurFrames + 0.5
		// with blurOverlap = 0 they form a perfect saw, see?

		// Guarantee quality greater or equal to shot.blurQuality
		cutoff = floor(shot.blurFrames * sqrt(1.0 - shot.blurQuality));
		shot.blurQuality = (double) cutoff / shot.blurFrames;
		shot.blurQuality = 1.0 - shot.blurQuality * shot.blurQuality;
		// Reciprocal of the blended area
		invArea = 4.0 / (shot.blurFrames * shot.blurFrames * shot.blurQuality);

		shot.blurFrames -= cutoff;
		shot.blurOverlap -= cutoff;

		for (i = 0; i < shot.blurFrames; i++) {
			double x = 0.5 * cutoff + i + 0.5;
			if (i < shot.blurFrames - i - 1)
				shot.mult[i] = x * invArea;
			else if (i == shot.blurFrames - i - 1)
				shot.mult[i] = (x - 0.25) * invArea;
			else
				shot.mult[i] = (shot.blurFrames + cutoff - x) * invArea;
		}
	} else {
		if (Q_stricmp("uniform", r_blurType->string))
			ri.Printf(PRINT_ALL, "Unrecognised r_blurType value. Using uniform blur.");

		for (i = 0; i < shot.blurFrames; i++) {
			shot.mult[i] = 1.0f / shot.blurFrames;
		}
	}

	if (shot.blurOverlap > 0) {
		ri.Printf(PRINT_WARNING, "With current settings r_blurOverlap must be less or equal to %d",
			  r_blurOverlap->integer - shot.blurOverlap);
		return qfalse;
	}
	return qtrue;
}

qboolean RE_StartVideoRecording( void )
{
	int	bufferSize;

	shot.width = glConfig.vidWidth;
	shot.height = glConfig.vidHeight;
	shot.frame = 1;
	shot.blurOverlap = r_blurOverlap->integer;
	shot.blurFrames = r_blurFrames->integer;
	if (shot.blurFrames < 0 || shot.blurFrames > MAX_BLUR_FRAMES) {
		ri.Printf(PRINT_WARNING, "r_blurFrames must be in the 0..%d range.\n", MAX_BLUR_FRAMES);
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

	if (!initBlurPattern())
		return qfalse;

	return qtrue;
}

void RE_StopVideoRecording( void )
{
	ri.Free(shot.accum);
	shot.frame = 1;
	shot.recording = qfalse;
}

// Make sure that these auto-vectorize nicely with GCC
void R_AccumLoad( const byte *captureBuffer, size_t memcount, float *accumBuffer, float multiplier )
{
	int i;
	shot.accumFrame = 1;
	multiplier /= 255.0f;
	for(i = 0; i < memcount; i++) {
		accumBuffer[i] = multiplier * captureBuffer[i];
	}
}

void R_Accum( const byte *captureBuffer, size_t memcount, float *accumBuffer, float multiplier )
{
	int	i;
	shot.accumFrame++;
	multiplier /= 255.0f;
	for(i = 0; i < memcount; i++) {
		accumBuffer[i] += multiplier * captureBuffer[i];
	}
}

void R_AccumReturn( const float *accumBuffer, size_t memcount, byte *returnBuffer )
{
	int i;
	shot.accumFrame = 0;
	for(i = 0; i < memcount; i++) {
		returnBuffer[i] = (byte) (accumBuffer[i] * 255.0f);
	}
}
