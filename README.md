# csc3022h_assignment_5
You are required to write a C++ application which can process audio sounds clips. Using
your application, it should be possible to perform simple editing operations on sound clips –
such as cut and paste – as well as transforming the sound clips. Examples of the latter
include fade in/out and normalisation. The sound clips will be 1-channel (mono) or 2-channel
(stereo) and will be provided as simple raw byte data which you need to interpret correctly.
Programmatically, a raw sound file/clip is a sequence of samples (usually, 8, 16 or 24-bits)
of an audio signal that can be sent to a speaker to produce sound. The sound clip also has
an associated sample rate – for example, 44.1 kHz (ie. 44100 samples per second). The
higher the sample rate, the better, usually, the quality of the sound produced. The number
of bits per sample also has a profound effect on audio quality: generally, 8-bits per sample
produces really poor sound. Of course, high sampling rates result in very large sound files,
which is why compression (such as MP3) is usually used – we will not expect you to
manipulate compressed formats. Simple raw (byte stream) audio will be used throughout.
