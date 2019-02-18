# ofOSCDebugger
## Description
ofOSCDebugger is command line tool to monitor and send OSC messages.

## Installation
### OSX
On OSX you can install it via brew.

```
brew tap thomasgeissl/tools
brew install ofoscdebugger
# brew upgrade ofoscdebugger
```
This will download the most recent version of the OSC debugger and add it to your search path.

### Building
And as always you can clone it to your apps directory and build it by running make.

## Usage
* listen to incommig OSC messages: `ofOSCDebugger -p 8000`
* send an OSC message: `ofOSCDebugger -h localhost -p 8000 -m "/address 0 0.0 zero"`
* add -i to send an OSC message and then go into the interactive mooe to be able to enter a new message and send it to the specified receiver: `ofOSCDebugger -h localhost -p 8000 -m "/address 0 0.0 zero" -i`

## License
This software is distributed under the [MIT License](https://en.wikipedia.org/wiki/MIT_License), please note that dependencies might be released differently.

Copyright (c) 2016 Thomas Geissl

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
