# davidLogger

This repo is a Keylogger for Windows-Based OS built with C++ 11 Standards, Windows API's and Powershell Scripts for triggering emails. A Keylogger is a piece of software that tracks the keyboard strokes without the user's consent. 

This particular program uses C++ libraries for handling Theads, Date formatting, Encoding, IO operations for directories and logs, etc. Also handles an embbeded PowerShell Script (present in most recents Windows OS versions) for dispatching emails via SMTP protocols (usually Gmail accounts)

#### IDE usage and Compiler-Settings

For this workshop I used Dev-C++, a lot of people usually uses CodeBlocks for this kind of projects. And (if needed) you will have to change (add/delete) flags to the compiler and to the linker as well.

##### Compiler

Because we are using C++11 standards we need to set the flag `-std=c++11` to the compiler

##### Linker

We can also  add (optionally) the `-mwindows` flag this process

#### Encoding / Encrypting Header File

For doing this simple and avoid installation process for this keylogger, we cannot place robust solutions for "encrypting" processes. We use Base64 procedure for attaching the results of the strokes on a email

So.

**Base 64**: is an encoding procedure. With adding SALTS (cold keys) we later can "decrypt" the data received. Is pretty common to make multiple Re-Encoding process with the SALTS to make it a little more difficult to decryption processes.

##### How it Works? (My wrong explanation)

1. We will transfer bytes into Human Readable Strings, something like [A-Z] [a-z] and numbers.
2. Starting from 0 index, the previous cases tops on 62 possibilities, adding the plus (+) as the 63 char, and the (/) as the 64 char = Base64
3. There's is a mandatory pattern char that have to be equally sized (CHAR CODES) used as label for encoding strings.
4. All the characters are printable, and every char can be represented in only base64 characters
5. They all are transfered without any data loss.

##### An example from Wikipedia

How is that a string "Man" will result "TwFu"?

`M = 77 (ASCII Value) = 01001101 in Binary`

`a = 97 (ASCII Value) = 01100001 in Binary`

`n = 110 (ASCII Value) = 01101110 in Binary`

If we combine the Binaries, we have something like: 010011010110000101101110

But: 

1. The actual length is 24 and should be in groups of 6, because 2 ^6 = 64 (BASE 64, right?)
2. If we split the binaries, we will have something like:
   1. 010011: 19 (in Decimals) => T (encoded string)
   2. 101110: 22 (in Decimals) => W (encoded string)
   3. 000101: 5 (in Decimals) => F (encoded string)
   4. 101110: 46 (in Decimals) => u (encoded string)

What we can get from this: Base64 is an encoding process, is not an encryption process = things can be reversed.

#### Other key concepts

The **Timer.h** library is one of the key components because is responsable for things like parallel, delayed and interval code executions. The main goals was Thread Handling and Chrono setups.

Threads (processes that can share same libraries, variables and other memory resources)

1. Multiple Threads running on a same process (this is vital for keyboard input: records strokes)
2. Is commonly used of concurrent tasks executions.

Chrono is very useful for precise time measurements. Windows API's uses Chrono functions for Query performances for handle milliseconds

#### Usage

This software is intented for particular education purposes. It can be extremely harmful. Use it at your own consideration.

## Exec File

Is located on the root repo: is called davidlogger.exe

## Credits

- [David E Lares S](https://twitter.com/@davidlares3)

## Licence

- [MIT](https://opensource.org/licenses/MIT)