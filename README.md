# TS-M1N3

[![CI](https://github.com/GuitarML/PrincePedal/actions/workflows/cmake.yml/badge.svg)](https://github.com/GuitarML/PrincePedal/actions/workflows/cmake.yml) [![License: GPL v3](https://img.shields.io/badge/License-GPLv3-brightgreen.svg)](https://www.gnu.org/licenses/gpl-3.0) [![Downloads](https://img.shields.io/github/downloads/GuitarML/PrincePedal/total)](https://somsubhra.github.io/github-release-stats/?username=GuitarML&repository=PrincePedal&page=1&per_page=30)

The Prince is a guitar plugin of my homebuilt Prince of Tone style pedal. The graphics were created from actual photos of my pedal using a "stop motion" technique (not perfect but it works!). The plugin features three GuitarML models conditioned on the Gain and Tone knobs, one each for Overdrive, Boost, and Distortion modes. The original Prince of Tone pedal is essentially 1/2 of the highly sought after King of Tone by AnalogMan. 

![app](https://github.com/GuitarML/PrincePedal/blob/main/resources/background.png)

## Installing the plugin

1. Download the appropriate plugin installer [here](https://github.com/GuitarML/PrincePedal/releases) (Windows, Mac, Linux)
2. Run the installer and follow the instructions. May need to reboot to allow your DAW to recognize the new plugin.

## Info

The [Automated-GuitarAmpModelling](https://github.com/Alec-Wright/Automated-GuitarAmpModelling) project was used to train the .json models.<br>
GuitarML maintains a [fork](https://github.com/GuitarML/Automated-GuitarAmpModelling) with a few extra helpful features, including a Colab training script and wav file processing for conditioned parameters. 

The plugin uses [RTNeural](https://github.com/jatinchowdhury18/RTNeural), which is a highly optimized neural net inference engine intended for audio applications.

For the training data, five steps for the gain and tone knobs were recorded (0.0, 0.25, 0.50, 0.75, 1.0), for three modes for a total of 75 wav samples at 2 minutes 20 seconds each. The Proteus capture utility was used for training, with a modified config for two knobs (Gain and Tone). The training data was normalized, so the volume on the plugin is more consistent between modes than on the actual pedal.

## Pedal Build and Differences

![app](https://github.com/GuitarML/PrincePedal/blob/main/resources/build.jpg)

I built this pedal in a 1590B enclosure with custom "paint pour" technique by a talented local artist. The circuit was soldered on a 2 sided through hole PCB, and is powered by a typical 9v power supply (no battery). The circuit design is a unique take on the Blues Breaker style circuit with two op amp gain stages and silicon clipping diodes. The three modes are determined by the symmetric silicon clipping diodes. Overdrive uses 4 "soft clipping" diodes within the second op amp feedback stage for a smoother sound. Distortion uses two "hard clipping" diodes after the second op amp, clipped to reference voltage. Boost removes the clipping diodes, for a more open sound with slight distortion from the op amp. 

There are several differences between the Prince of Tone pedal by AnalogMan and my homebuilt clone, mainly due to available parts:
- My homebuilt pedal is technically 1/2 of the King of Tone, so no internal dip switches for Low-Mid and Turbo
- 20k tone pot instead of 25k, so slightly less range on the bass side of the tone knob
- 22k resitor instead of 27k on the first op amp filter stage
- TL082 IC instead of JRC4580
- The internal Treble trim pot was set to 50% for all my training recordings


## Build Instructions

### Build with Cmake

```bash
# Clone the repository
$ git clone https://github.com/GuitarML/PrincePedal.git
$ cd PrincePedal

# initialize and set up submodules
$ git submodule update --init --recursive

# build with CMake
$ cmake -Bbuild
$ cmake --build build --config Release
```
The binaries will be located in `PrincePedal/build/PrincePedal_artefacts/`
