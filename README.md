xvConvert
=========

xvConvert is a tool to convert xv video format by xunlei company to other format

xvConvert is licensed  under the terms of the GNU General Public License 3 as published by
the Free Software Foundation


How to?
-----

### Compile it

Build script currently generates only solution _(*.sln)_ files for Microsoft Visual Studio IDE

#### 1. Requirements

- Windows with [MS Visual Studio](https://www.visualstudio.com/products/visual-studio-express-vs) 2012+ *(2010 not tested but should work too)*

#### 2. Steps

1. [Download snapshot of this repository][repo-dl], unzip and open it
2. Run `premake.bat` or `premake5.bat`
3. The script should generate a solution file (_.sln_) in the project root dir, open it in VS as usual and upgrade the solution when needed, eg. if you have VS2013 and the script created VS2012 solution (automatic prompt or `Project -> Upgrade Solution`)
4. `Build -> Configuration Manager` and choose configurations and platforms you want to build
5. And here we go `Build -> Build Solution`, which should produce binaries in the `bin` dir

**Following these steps and building all binaries in their _Release_ versions took me ~15 second on my workstation.**

