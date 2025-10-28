## Logger

This repo is a Keylogger for Windows-based OS built with C++ 11 Standards, Windows API's and PowerShell scripts for triggering emails. A Keylogger is a piece of software that tracks keyboard strokes without the user's consent. 

This particular program uses C++ libraries for handling threads, Date formatting, Encoding, IO operations for directories and logs, etc. Also handles an embedded PowerShell Script (present in most recent Windows OS versions) for dispatching emails via SMTP protocols (usually Gmail accounts)

#### IDE usage and Compiler Settings

For this workshop, I used Dev-C++; a lot of people usually use CodeBlocks for this kind of project. And (if needed) you will have to change (add/delete) flags to the compiler and to the linker as well.

##### Compiler

Because we are using C++11 standards, we need to set the flag `-std=c++11` to the compiler

##### Linker

We can also  add (optionally) the `-mwindows` flag to this process

#### Encoding / Encrypting Header File

To do this simply and avoid the installation process for this keylogger, we cannot place robust solutions for "encrypting" processes. We use the Base64 procedure for attaching the results of the strokes to an email

So.

**Base 64** is an encoding procedure. By adding SALTS (cold keys), we can later "decrypt" the data received. It is pretty common to make multiple Re-Encoding processes with the SALTS to make it a little more difficult to decryption processes.

##### How it Works? (My wrong explanation)

1. We will transfer bytes into Human Readable Strings, something like [A-Z] [a-z] and numbers.
2. Starting from 0 index, the previous cases top 62 possibilities, adding the plus (+) as the 63 character, and the (/) as the 64 character = Base64
3. There's a mandatory pattern character that has to be equally sized (CHAR CODES) used as a label for encoding strings.
4. All the characters are printable, and every character can be represented in only base64 characters
5. They are all transferred without any data loss.

##### An example from Wikipedia

How is that a string "Man" will result in "TwFu"?

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

What we can get from this: Base64 is an encoding process, not an encryption process = so things can be reversed.

#### Other key concepts

The **Timer.h** library is one of the key components because it is responsible for things like parallel, delayed, and interval code executions. The main goals were Thread Handling and Chrono setups.

Threads (processes that can share the same libraries, variables, and other memory resources)

1. Multiple Threads running in the same process (this is vital for keyboard input: records strokes)
2. It is commonly used for concurrent task executions.

Chrono is very useful for precise time measurements. Windows API uses Chrono functions for querying performance for handling milliseconds

#### Usage

This software is intended for particular educational purposes. It can be extremely harmful. Use it at your discretion.

## Exec File

It is located in the root repo: is called davidlogger.exe

## Credits
[David Lares S](https://davidlares.com)

## License
[MIT](https://opensource.org/licenses/MIT)
