#VSTDev

A design document for the Audio Apron granulator. 

The goal of this project is to make granular synthesis a fun and intuitive process for any level of sound designer. 

We will achieve this by providing easy to understand visual UI controls and a product that creates interesting sounds no matter what the parameters are set to.

# 🔄 `CircularBuffer`
The `CircularBuffer` class is minimal and designed to keep track of past audio samples.

The `write()` function is called for every sample of it's respective channel so that an accurate source of audio history is maintained.

As additional functionality specific to this project, each `CircularBuffer` has a vector of `GrainWindow`

## • Functions
---
```cpp
CircularBuffer()
```
- Initializes the `CircularBuffer` object with default values.
---
```cpp
void write(double sample)
```
- Called every sample for its respective channel
---
```cpp
double read(double index)
```
- Returns the current read index and wraps it to the `userSize`
- Linear interpolates to account for decimal value read indexes
---
```cpp
void setSize(double size)
```
- Sets the **true size** of the vector using it's `resize()` function
- Should **NOT** be used for changing the user size, as this is a slow operation
---
```cpp
int getSize()
```
- Gets the **true size** of the vector using its `size()` function
- Parameter is given in seconds
---
```cpp
void setUserSize(double size)
```
- Sets the size defined by the user from the UI
- Parameter is given in seconds
- This is simply an index that tells the write function when to loop back to the start
---
```cpp
double getUserSizeInSeconds()
```
---
```cpp
double getProgress()
```
- Gets a value from 0.0 to 1.0 representing the normalized position of the `writeIndex`
---
```cpp
void clear()
```
- Clears the buffer within it's `userSize`
---
## • Member Variables
---
### Public
---
```cpp
std::vector<GrainWindow> grains
```
- Vector of `GrainWindow`s for this circular buffer channel.
- This is additional functionality for the circular buffer class and has nothing to do with the actual circular buffer
---
```cpp
bool writePause
```
- Whether to increment the `writeIndex` or not
---
```cpp
int userNumGrains
```
- Amount of `GrainWindow`s defined by the user

---
### Private
---
```cpp
std::vector<double> buffer
```
- The input audio is stored in this vector
- Acts as a source of "truth" of what happened in the past
---
```cpp
int writeIndex
```
- The current write position of the circular buffer
---
```cpp
int userSize
```
- The size defined by the user from the UI
---

# 🌾 `GrainWindow`
The `GrainWindow` class defines parameters such as the start index, read index, size, attack, decay, playback speed, and more, to control the behavior of audio grains.

## • Functions
---
```cpp
GrainWindow()
```
- Initializes the `GrainWindow` object with default values.
---
```cpp
void setReadIndex(double index)
```
- Should only be called in rare circumstances.
- Can cause "clicking" if used incorrectly.
---
```cpp
double getReadIndex()
```
- Returns the current read index.
- Index is adjusted with `randomStartOffset` and `readOffset`
- Will return a value outside the bounds of the `userSize`, so it needs to be wrapped after the call.
---
```cpp
void incrementReadIndex(double historyBufferUserSize)
```
- Increments the read index based on `playbackSpeed` and `randomPitch`
- Recalculates the `progress`
- Resets `randomPitch` and `randomStartOffset` when the progress is complete
---
```cpp
void setReadOffset(double offset)
```
- Sets the `readOffset`
- Parameter is given in seconds
---
```cpp
void setRandomStartOffset(double offset)
```
- Sets the `randomStartOffset`
- Parameter is given in secondss
---
```cpp
void setStartIndex(double start)
```

- Sets the `startIndex`
- Parameter is given in seconds
---
```cpp
void setGrainPitch(double speed)
```
- Sets the or `playbackSpeed` based on semitone mode.
---
```cpp
double getGainFactor()
```
- Returns the gain factor based on the `progress`, `attack`, and `decay` parameters.
- Example output is a value from 0.0 to 1.0
---
## • Member Variables
---
### Public
---
```cpp
double size
```
- Should always be less than the `userSize`
---
```cpp
double attack
```
- A value from 0.0 - 1.0 representing the time at which to stop affecting the gain factor
---
```cpp
double decay
```
- A value from 0.0 - 1.0 representing the time at which to start affecting the gain factor
---
```cpp
double progress
```
- A value from 0.0-1.0 representing how far the read index is in the grain size
---
```cpp
bool reverse
```
- Whether the `playbackSpeed` should be positive or negative
---
```cpp
bool semitoneMode
```
- Whether to use semitones for the `playbackSpeed` or to use raw rate 
- 0.0 being completely stopped and 1.0 being normal speed
---
```cpp
bool grainHasReset
```
- A state variable to tell us if the progress reset or not
---
```cpp
double randomPitch
```
- The random pitch value to add to the `playbackSpeed`
---
```cpp
double randomStartOffsetMax
```
- Used as the upper value when determining the `randomStartOffset`
---
```cpp
juce::Random pitchRandomizer
```
- Random object which is synced with it's respective channel using the same seed.
---
```cpp
juce::Random startOffsetRandomizer
```
- Random object which is synced with it's respective channel using the same seed.
---

### Private
---
```cpp
double startIndex
```
- Index which can be set outside `userSize` if not careful
---
```cpp
double readIndex
```
- Should auto update by calling `incrementReadIndex()`
---
```cpp
double readOffset
```
- Applied when getting the read index
---
```cpp
double randomStartOffset
```
- How much to offset the start index by
---
```cpp
double playbackSpeed
```
- Used as the incrementor for the read index
- This is a double because of having variable pitches
---






# ⚓ Utilities
This project uses a set of custom utility functions for remapping, lerping, wrapping, etc. 

These functions are usually called within the class rather than outside; however, they are not limited to that use case.

## • Functions
---
```cpp
double secondsToSamples(double seconds)
```
- Utilizes the plugin sample rate
---
```cpp
double samplesToSeconds(double samples)
```
- Utilizes the plugin sample rate
---
```cpp
double linearInterpolation(double y1, double y2, double x)
```
- Simple lerp function
---
```cpp
double remapValue(double originalValue, double originalMin, double originalMax, double targetMin, double targetMax)
```
- Remaps a value with a range to a new range.
- This could just use `juce::Remap()` instead
---
```cpp
double semitonesToPlaybackRate(double semitones)
```
- Converts a number of semitones to a normal playback rate
- 0.0 semitones would be a playback rate of 1.0
---
```cpp
double playbackRateToSemitones(double playbackRate) 
```
- Given a playback rate, this will return a value relative to C
- For example, a playback speed of 1.0 should return 0.0 semitones, which represents a C note.
- Any return value mod 12 is a C note
---
```cpp
double wrapReadIndexToBuffer(double readIndex, double bufferSize)
```
- Basically does `readIndex % bufferSize` but works for doubles and negative read indexes
---
```cpp
double getRandomDouble(double min, double max)
```
- Returns a random double within the range
---
# Misc. Todo 
- colors should evenly distribute as adding more grains. lightest color is first grain darkest is last
- changing grain size affects offset somehow and causes clicks
- offset system is completely broken
- grain start and grain size will not have knobs
- attack and decay will not have knobs either

# ⏩ Future Dev
## • Generator
Create second version where you can drag a sample into the sampler and play it like an instrument (rather than an effect plugin).

Sell both versions together as one package

---
## • Switch from OOP
Redesign plugin to be less object oriented internally.

Make heavy use of utility functions and prioritize performance.

This will easily allow "grain windows" to be aware of each other, making the process of creating other effects much simpler.

---
## • Frontend Framework
Integrate some type of frontend framework like JIVE or use a webview to design the user interface.

---