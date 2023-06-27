# SFML Input

Visualize / audiolize keyboard inputs, especially useful to check keyboard layouts and test SFML's scan code implementation

![screenshot](https://github.com/eXpl0it3r/SFML-Input/assets/920861/5d5cbf37-c718-4cd6-8a53-705dced6f4a7)

## Simulate Keys with AutoHotkey (Windows)

- Install AutoHotkey: `choco install autohotkey`
- Create a file with the file extension `.ahk`
- Use the following lines `ControlSend, , {vkXX}, SFML Input Test`
- Replace `XX` with any known hex [Virtual Key Code](https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes)

**See also** the provided TestAllKeys.ahk file

## Retrieve Key Information (Linux)

- Install evtest: `sudo apt install evtest`
- Run evtest: `evtest /dev/input/event5` (or similar)
- Press keys and check the output in the terminal

## License

This software is available under 2 licenses -- choose whichever you prefer.

- Public Domain
- MIT
