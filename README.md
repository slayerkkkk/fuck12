# Simple LKM Keylogger for Linux

A simple and functional Linux kernel module (LKM) that captures keyboard keystrokes, buffers them, handles backspace, and prints the complete lines to the kernel log when Enter is pressed. ;)

---

## Features

- Captures keystrokes on input devices (keyboard).
- Internal buffer stores the sequence of keystrokes.
- Backspace deletes the last character from the buffer.
- Enter sends the entire line to `dmesg` and clears the buffer.
- Automatic mapping of keys to common ASCII characters.

---

## Requirements

- Linux with kernel 6.1.x (tested on Debian GNU/Linux 12 - Bookworm).
- Tools for compiling kernel modules (`build-essential`, `linux-headers`).

---

## How to compile

Clone or download the source code and compile with:

```bash
make
```

---

## How to load the module

```bash
sudo insmod keylogger.ko
```

You will see messages in the kernel log confirming the loading and the connected devices.

---

## How to view the keystroke log

Run:

```bash
sudo dmesg -w
```

Type on the keyboard normally. The keys will be accumulated internally and, when you press Enter, the complete line will appear in `dmesg`.

---

## How to unload the module

```bash
sudo rmmod keylogger
```

---

## Example output

```
[19360.775508] FUCK12 - KEYLOGGER: Hello World
[19361.239494] FUCK12 - KEYLOGGER: Testing Backspace
```

---

## Warning

This module captures system keystrokes, use it responsibly and only in controlled and authorized environments.

---

## Project structure

* `keylogger.c` - Source code of the kernel module.

* `Makefile` - File for compiling the module.

---

## Basic Makefile

```Makefile
obj-m += keylogger.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
```
