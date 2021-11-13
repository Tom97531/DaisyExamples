# V.oscillator

The principle behind this oscillator is to divide a single wave cycle in 8 micro oscillator. Each of this micro osc plays its part of the cycle. At init, it outputs a sine wave. The subtleties appear when you change a parameter of a single micro-osc (rattio, fine tune, pan, ...). If you double the ratio of `micro-osc 1` then 1/8 of the cycle is going to be at twice the frequency of the main wave cycle.

## Author

Original Idea and max4live patch v.MICROSYNTH from [V.ictor](https://v-ictor.ca/d-evices-m4l/). Ported on daisy patch by Thomas Piquet.

## Description
It is meant to be used as a usual oscillator module, meaning you have a pitch and a fine pitch control and it outputs a sound at that pitch, timbre modification are mostly done via menus. 

On top of that, an amplitude envelope is added for the sake of not buying new modules. You can turn this feature on through the menu `Envelope`. By default the envelope type is `OFF`. If you use the `AD` envelope type, you can trigger an attack decay cycle using `GATE IN 1`. Attack and Decay time are set using `CTRL 1` and `CTRL 2` respectively. For `ADSR` mode, the extra `Sustain` and `Release` are avaible in the menu.

Currently all of the micro-osc settings are available as menus.

### I/O
---
- CTRL 1: Pitch v/oct
- CTRL 2: Main frequency Fine tune
- CTRL 3: Attack (if enabled, for AD and ADSR envelope type)
- CTRL 4: Decay (if enabled, for AD and ADSR envelope type)
---
- GATE IN 1: amp envelope gate input (if enable)
---
- OUT 1: Left audio output
- OUT 2: Right audio output
---

### Menu
Navigating through the menu is done with the `ENC 1` knob. Turning it, changes the menu selection, clicking it, will either enter a sub-menu, toggle a switch or move to the value to change. Turn the knob to change the value. Click to go back to menu navigation. When in a sub-menu a `back` menu selection is available.

Menus can be sub-divised in 2 categories, one for the micro-osc settings, and the other for global oscillator settings. Regarding micro-osc setting, they are all made the same way, ie. 8 micro-osc changable values plus a `back` menu selection. Altering one of this parameter will result in a change of 1/8 of the main wave cycle (Pitch determined by `CTRL 1`).

Micro-osc menu:
- Ratio: change the ratio with the main oscillator.
- Fine Tune: change the fine tuning compared to the main osc.
- Pan: pan left or right.
- Volume: change volume. 

General menu:
- Hard sync: Toggle hard reset of micro-osc at the end of the cycle. Change the tone of the oscillator. Stops the phasing effect.
- Envelope:
    - Env type: OFF / AD / ADSR
    - Sustain: amp envelope sustain level (0 to 1). Only for ADSR type.
    - Release: amp envelope release time in seconds. Only for ADSR type.