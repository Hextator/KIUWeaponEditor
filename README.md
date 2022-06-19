# KIUWeaponEditor
In-game weapon editor for Kid Icarus: Uprising compatible with the Rosalina sysmodule trainer.
Consider [becoming my patron on ![P](https://i.imgur.com/5URQWRR.png)atreon](https://www.patreon.com/hextator) if you like this software; I really need the help.

# Installation
Copy the appropriate `[Title ID].txt` from the `Codes` directory into the folder (probably `./cheats`), of your SD card, that your Rosalina sysmodule (of the Luma3DS custom firmware) uses for loading cheats.
If the codes are installed properly, you can enable them from the Rosalina menu as usual. The included `Input Scan` cheat must also be enabled, as the weapon editor uses it to...well, you can probably guess.

# Features
The weapon editor adds support for new button functions within the weapon fusion menu (the "fusion grid"). The code is only executed there, so the extra button functions should not be active outside of that menu.

The weapon editor operates on the selected "fodder A" (the top weapon of the 2 selected for fusion).

The editor is designed with the intent that the player will be holding L with no "pop up" menus overlaying the fusion grid when other buttons are pressed. If holding L causes such a dialog to appear on the screen, selecting the dialog's cancel option, then holding L again should result in the desired state of L being held with default state of the fusion grid menu intact (other than the selected grid index).

These are the button functions added:
1. L - Enable editing
	* Any actions that would alter the currently selected weapon won't work if the weapon is favorited; things like changing which slot is selected for editing or duplicating the current weapon still work
2. R - "Alternate mode"
3. D Pad Up - Select slot above (or else with "alternate mode": select slot 0)
4. D Pad Down - Select slot below (or else with "alternate mode": select last slot (the last affix slot)). Slots are like:
	* Weapon type
	* Weapon model
	* Ranged stars
	* Melee stars
	* Mod slots 1-6
5. D Pad Left - Minus 1 weapon type/1 weapon model/0.5 stars/1 mod ID (2/3/2.0/16 with "alternate mode")
6. D Pad Right - Plus 1 weapon type/1 weapon model/0.5 stars/1 mod ID (2/3/2.0/16 with "alternate mode")
7. B - Reset current slot (3.0 for stars, Mod ID = the maximum mod ID divided by 2 for mods). The resetting of the current slot does not occur with "alternate mode" enabled; instead:
	* Having the type or model slot selected duplicates the current weapon
	* Having the ranged or melee star slots selected resets the current weapon to v100
	* Having an affix slot selected should sort the weapon's modifiers (not yet implemented)

# License
1. You may use the parts of this software shared in this repository, but you must be aware of how you can donate to my well being, and that I am not very healthy, and very much in need of financial support. To that end, you may: [become my patron on ![P](https://i.imgur.com/5URQWRR.png)atreon](https://www.patreon.com/hextator)
	* You use my software at your own peril. No warranty is provided and I disclaim all liability.
	* You may only share this software by linking to this repository, when this repository contains this license.
2. You may copy or modify the parts of this software shared in this repository with direct permission from me, Hextator. I can be reached via Discord; my handle is `Hextator#6552`.
	* Any use of my source code, in this repository or otherwise, must be attributed to me (Hextator).
	* You may not use my software for financial gain without cutting me in on the deal, and the terms must be decided entirely by me, subject to change on a whim.
	* You may not use my software to intentionally harm others.

# Building
Simply put, you don't.

This project uses GNU make scripts that are organized in a tree suitable to my personal needs, the applications of which are far beyond the scope of this project. Managing them has become a project unto itself, and the licensing for, and resources of, that project are thus separate.

This project also uses a combination of C/++ header files and Python scripts that are part of a system I've developed called "Adapt" (stylized "adapt"). That is also a project unto itself, and so licensing and resources related to it do not belong here.

If you are contacting me already regarding your own usage of this repository's source code, you may ask for assistance with filling in the missing pieces at that time, but I am not obligated to assist.
