# SFML Input

Just a repository to test scancode inputs

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
