# OpenGL Scene Editor

This project involves adding functionalty to a simple scene editor program that allows users to create and light elements in a number of fun ways. This document provides details on the necessary information to install and run the scene editor. Details on how certain features were implemented can be found in the Project_Report.pdf file.

# Install & Run

Step 1. Clone the repo to your local device (skip to step 3 if you already have the repo locally).

```
git clone https://github.com/TylerIllman/OpenGL-Scene-Editor
```

Step 2. `cd` into the newly cloned directory.

```
cd OpenGL-Scene-Editor
```

Step 3. Generate the build files.

```
cmake -S . -B cmake-build-release -DCMAKE_BUILD_TYPE=Release
```

Step 4. Run the build command. This can take some time.

```
cmake --build cmake-build-release
```

Step 5. Once the project has been built run the bellow command to open the scene editor

```
./cits3003_project
```

# OS Specifications

This project was built on hardware using Apple M1 Silicon chips and running Mac OS. It should be able to run on most operating systems.


