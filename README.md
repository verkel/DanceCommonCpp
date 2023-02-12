# Dance Common (C++ version)

A parser for dance game stepcharts in [Stepmania](https://www.stepmania.com/) format and an AI that figures out the optimal way to play through a stepchart.

See detailed description of the AI in my [Aalto University Master's Thesis](https://github.com/verkel/thesis)

## Project organization

There is no standalone executable program produced by the solution. It consists of:

* DanceCommon - stepchart parser and AI library
* DanceCommonTests - [Google Tests](https://github.com/google/googletest) for the library

## Usage

To run project unit tests:

* Clone the repository and open DanceCommonCpp.sln in Visual Studio 2022.
  * This is a hard requirement because the project uses C++20 features like modules. Also you will see various IntelliSense errors because the Modules support seems still somewhat buggy (but there should not be any compile errors).
* Open Test Explorer (View->Test Explorer)
* Click Run All Tests In View
  * All tests should pass
  * TestRobotPlaysDT compares the solution of the AI to ones generated with the original Java version of the AI. There are 308 tests like this (one for each chart), so running them will take the majority of the test execution time.
  * All tests take around 6s in Release mode and 40s in Debug mode in my workstation (i7 2600k)
