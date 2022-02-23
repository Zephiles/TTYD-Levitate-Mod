## Overview
This is a levitate mod that I have made for Paper Mario: The Thousand-Year Door. It is effectively the same as the **Super Jump** Action Replay code that Datel made, which can be found on [this page](http://us.codejunkies.com/search/codes/paper-mario--the-thousand-year-door_gamecube_7340790-7___.aspx).

## Download
The latest release can be found on the [releases page](https://github.com/Zephiles/TTYD-Levitate-Mod/releases).

## Usage
This mod is loaded the same way as the Practice Codes project, and instructions for doing so can be found [here](https://github.com/Zephiles/TTYD-Practice-Codes/blob/master/USER_MANUAL.md#setup). Once loaded, simply press/hold a combination of the **L** and **A** buttons in-game.

## Prerequisites
To build this, you need to have devkitPPC installed. Instructions for doing so can be found [here](https://devkitpro.org/wiki/Getting_Started). You also need to place **elf2rel.exe** in the **bin** folder. This file can either be compiled manually, or downloaded from [here](https://github.com/PistonMiner/ttyd-tools/releases/download/v2.7/elf2rel.exe).

## Building
To build, you must first add **DEVKITPPC** and **TTYDTOOLS** to your environment. **DEVKITPPC** should be set to the `devkitPPC` folder, and **TTYDTOOLS** should be set to the `ttyd-tools` folder. Then, navigate to the root directory of the repository (the folder with the makefile in it) and run `make`. Any combination of rules can be applied to this. You should also run `make clean` after making any changes, as files built with a previous build could cause issues.  

Examples of `make` being used:  
`make us`  
`make jp eu`  
`make us eu`  
