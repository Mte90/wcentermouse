# WCenterMouse
[![License](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](http://www.gnu.org/licenses/gpl-3.0)   

Utility to center the mouse pointer on the screens, tested in Wayland.
Based on ChatGPT and Co-Pilot code and improved manually.

As Wayland doesn't support aboslute position for mouse and in a multi monitor environment is not good, we set a big negative position initially to set a different later.
The program require the size of the various monitor configured in the source code.

Note: xdotool is only for Xorg and ydotool not works well for the same reason with multi monitors environment.

## Compile

```
g++ main.cpp -o wcentermouse
```

## Use it

as root:

```
visudo wcentermouse
```

With this automatically sudo will execute it without asking the password and also sharing the XDG runtime variable.

```
ALL ALL=(root) NOPASSWD: /path/wcentermouse
Defaults    env_keep += "XDG_RUNTIME_DIR"
```

Run

```
sudo ./wcentermouse 1/2/3/etc
```
