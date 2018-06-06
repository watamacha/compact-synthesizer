# compact-synthesizer

This repository contains files associated with my synthesizer project. The goal was to create a cheap, reprogrammable device to serve as a self contained synthesizer, audio processor, and midi mangler using off the shelf components. The synthesizer accepts user input via a series of 3 adafruit trellis buttonpads which communicate with an arduino uno over an i2c bus. The arduino then sends UART serial through a 5v to 3.3v level converter to an axoloti core, which performs all the DSP.

In the future I will upload the case stl for 3d printing. In the patches directory I will be putting pairs of .axp and .ino scripts which define the behavior of the axoloti and the arduino under specific configurations. The configuration presently provided is a hammond organ emulator for use in Reggae, complete with drawbars, reverb, percussion controls, tremolo, and leslie speaker emulation. The patch is designed to utilize the onboard buttons for parameter control and a midi keyboard over either USB or 5-pin DIN for note entry.