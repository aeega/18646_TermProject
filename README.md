# 18646 Term Project

## Why Wilkes?
18646 is the postal zip code for Wilkes-Barre, Pennsylvania. 
Also, it's a word that's unlikely to appear whole or as a substring anywhere
in the project, so it can be wholesale text-replaced to a more relevant
name down the road.

## How to Build with Cmake & Conan
The original target system is CentOS 6.8, with cmake 3.13 and gcc 4.6.2.
This is an extremely old (Past EOL in 2018) distro, and thus many of the
build features may not work. Ideally an updated environment (potentially
via docker) will make things here work.

### Python3 & Pip3 
Make sure you've got python3 and pip3 installed. They may be called python3.6
and pip3.6, or something similar - look in /usr/bin or /usr/local/bin to find
them.

Verify that you've got the sqlite3 python module. If this isn't installed
(it should be), conan won't work.

The steps in this python/pip/conan section can be done from any directory

```python
>python
Python 3.10.4 (tags/v3.10.4:9d38120, Mar 23 2022, 23:13:41) [MSC v.1929 64 bit (AMD64)] on win32
Type "help", "copyright", "credits" or "license" for more information.
>>> import sqlite3
>>> exit()
```

Update pip3.

```bash
pip3 install --upgrade pip
```

Install conan. 
```bash
pip3 install conan
```

Configure conan, and install a test package
```bash
conan profile new default --detect;
conan profile update settings.compiler.libcxx=libstdc++11 default;
conan config set general.revisions_enabled=1;
conan install fmt/8.0.1@ --build=missing
```

If this all works, then your system can build this cmake/conan repo.

### Out-of-source builds
The .gitignore in this project will automatically ignore the 'build' subdirectory
and anything inside it. CMake (and build systems in general) encourage 
'out-of-source builds', which means that the build setup and products do not 
live in the same directories as the source code. This has a number of important
reasons - the most important is that it allows you to simultaneously have 
multiple build modes, like Debug or Release. Additionaly, it's not good to 
put build artifacts (object files, executables) in git repos - they're large
and not semantically mergeable, so they just clutter up the git repo.

Building a release build from scratch. Starting at the root of the repo,

```bash
mkdir -p build/Release;
cd build/Release;
cmake -DCMAKE_BUILD_TYPE=Release -G "Unix Makefiles" ../..;
make
```

And the same for Debug:

```bash
mkdir -p build/Debug;
cd build/Debug;
cmake -DCMAKE_BUILD_TYPE=Debug -G "Unix Makefiles" ../..;
make
```

For subsequent builds, even if you add or remove subdirectories, change
the conan dependencies, add or remove source files, you just type `make`
from the `build/Release` or `build/Debug` directories.

To clean,
```bash
make clean
```

## Running
Because the builds are now out-of-source, and these programs read and write
explicitly named files from the current working directory, the execution is
a little different. The image and csv files have been moved into a `data` 
subdirectory in each module, because it is good to keep source files and 
data files separated. Let's execute the `opencv_seq` executable on the 
`aeega_ced.jpeg` image.

From the root of the repo, assuming you've completed a release build,
```bash
cd wilkes/opencv_seq_implementation/data;
../../../build/Release/bin/opencv_seq ./aeega_ced.jpeg
```

## Ninja
If you're able to get ninja installed, it's much better than gnu make. You'll have to delete
the build/Release and build/Debug directories, and regenerate them with Ninja instead of 
Unix Makefiles. The Release build process would then look like this:

```bash
mkdir -p build/Release;
cd build/Release;
cmake -DCMAKE_BUILD_TYPE=Release -G Ninja ../..;
ninja
```

