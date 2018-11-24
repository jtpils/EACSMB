#ifndef __EACSMB_sound_sound_h__
#define __EACSMB_sound_sound_h__


#include <alsa/asoundlib.h>

#include "common_math.h"
#include "ds.h"

#include "c_json/json.h"




typedef struct SoundClip {
	
	int channels;
	float* data;
	
	int numSamples;
	int sampleRate;
	
	float length; // in seconds
	
} SoundClip;


enum SoundFlag {
	SOUNDFLAG_POSITIONAL = (1<<0), // position the sound in 3d space relative to the camera
	SOUNDFLAG_LOOP = (1<<1), // loop forever
	
};


// TODO: handle suspended playback after a pause
typedef struct SoundInstance {
	SoundClip* clip;
	
	enum SoundFlag flags;
	
	float volume;
	Vector* worldPos;
	
	double globalStartTime; // when to start playing, in global game time
	int lastSampleWritten;
	
} SoundInstance;



typedef struct SoundManager {
	
	int status;
	
	VEC(SoundClip*) clips;
	VEC(SoundInstance*) instances;
	
	// software mixer
	// channels are interleaved
	int channels;
	int sampleRate;
	
	float* buffer;
	int bufferLen;
	int bufferWriteHead, bufferReadTail;
	double lastMixTime;
	
	
	float globalVolume;
	
	
	// temp alsa-specific info
	snd_pcm_t* playback_handle;
	int16_t* alsabuf; 
	char* deviceName;
	
	
	
	
	
} SoundManager;


SoundManager* SoundManager_alloc();
int SoundManager_reallocBuffer(SoundManager* sm, int newLen);
void SoundManager_start(SoundManager* sm);
void SoundManager_shutdown(SoundManager* sm);
void SoundManager_addClip(SoundManager* sm, SoundClip* sc, char* name);
void SoundManager_addInstance(SoundManager* sm, SoundInstance* si);


void SoundManager_readConfigFile(SoundManager* sm, char* path);
void SoundManager_readConfigJSON(SoundManager* sm, json_value_t* jo);


void SoundManager_tick(SoundManager* sm, double newGlobalTime);
void SoundManager_outputTick(SoundManager* sm);


SoundClip* SoundClip_fromWAV(char* path);
void SoundClip_resample(SoundClip* sc, int newRate);


#endif // __EACSMB_sound_sound_h__