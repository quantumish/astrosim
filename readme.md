# AstroSim

## About
AstroSim is a work-in-progress astronomy simulation program written in C++ with a focus on a variety of potential applications: astrodynamics, rocket science, stellar dynamics, and simulation of formation of celestial objects. AstroSim also doubles as a visualization of said physical processes with 2d graphics using the SFML graphics library. This was created as an exercise in learning C++, using multi-file C++ projects and headers, using Xcode, and learning some new math and science. 

## Usage
### The Easy Way
Head to the releases tab and download the release for your operating system to view the demo in the easiest way possible.

### Building from Source

#### Command-Line
Building from the commandline is a little more involved. Clone the repot, use Xcode Commandline Tools to build, then navigate to the executable and open it. Sometimes you will need to hunt for the executable due to Xcode's inconsistencies.
```
git clone https://github.com/richardfeynmanrocks/astrosim.git
xcodebuild --configuration Release
open Release/AstroSim.app
```
#### Using Xcode App
1. Clone the repository.
2. Open the project in Xcode.
3. Build it inside the app.
<!-- 4. Right click the "Products" folder in the app, click on the "Show in Finder" item in the menu -->
