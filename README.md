# Auris: HRTF Auralization Engine

An hrtf auralization engine built as the final project of my Engineering degree.
It has it's own tiny FPS like user interface, and it was built using Juce and several open source projects (Look to my thesis for more details). It was tested on OsX 10.11.6 (Xcode) and on Windows 10(Visual Studio 2015), may work in Linux since all libraries are cross-platform.

![screenshot](https://raw.githubusercontent.com/Bk8/Auris/master/Resources/doxyicon.png)

![screenshot](https://raw.githubusercontent.com/Bk8/Auris/master/ScreenShot.png)

# Compilation
First get Juce and Projucer from this link:

https://www.juce.com/get-juce

To compile use Projucer, set Juce module paths and generate a target for Visual Studio or Xcode. Then open the generated project on the IDE and compile it. All necessary code is included except Juce libraries.
