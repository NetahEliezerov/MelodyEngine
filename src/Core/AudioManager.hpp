#include <AL/al.h>
#include <sndfile.h>
#include <AL/alc.h>
#include <vector>
#include <string>
#include "./Renderer.h"
#include "./Input.h"
class AudioManager
{
public:
    AudioManager()
    {
        // Initialize OpenAL
        device = alcOpenDevice(nullptr);
        context = alcCreateContext(device, nullptr);
        alcMakeContextCurrent(context);
    }

    ~AudioManager()
    {
        // Clean up OpenAL resources
        alcMakeContextCurrent(nullptr);
        alcDestroyContext(context);
        alcCloseDevice(device);
    }

    void LoadSound(const std::string& filename)
    {
        ALuint buffer;
        alGenBuffers(1, &buffer);

        // Load audio file using libsndfile
        SNDFILE* file = sf_open(filename.c_str(), SFM_READ, &sfInfo);
        if (file)
        {
            std::vector<short> data(sfInfo.frames * sfInfo.channels);
            sf_read_short(file, data.data(), data.size());
            sf_close(file);

            ALenum format = (sfInfo.channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
            alBufferData(buffer, format, data.data(), data.size() * sizeof(short), sfInfo.samplerate);

            buffers.push_back(buffer);
        }
        else
        {
            // Failed to load the audio file
            alDeleteBuffers(1, &buffer);
        }
    }

    void PlaySound(int index)
    {
        if (index >= 0 && index < buffers.size())
        {
            ALuint source;
            alGenSources(1, &source);
            alSourcei(source, AL_BUFFER, buffers[index]);
            alSourcePlay(source);

            // Store the source for later cleanup
            sources.push_back(source);
        }
    }

    void Update()
    {
        // Clean up finished sound sources
        for (auto it = sources.begin(); it != sources.end();)
        {
            ALuint source = *it;
            ALint state;
            alGetSourcei(source, AL_SOURCE_STATE, &state);

            if (state == AL_STOPPED)
            {
                alDeleteSources(1, &source);
                it = sources.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

private:
    ALCdevice* device;
    ALCcontext* context;
    std::vector<ALuint> buffers;
    std::vector<ALuint> sources;
    SF_INFO sfInfo;
};