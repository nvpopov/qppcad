# ![](/data/images/icon_big.png)
[![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.3257167.svg)](https://doi.org/10.5281/zenodo.3257167)
[![Build Status](https://travis-ci.org/nvpopov/qppcad.svg?branch=master)](https://travis-ci.org/nvpopov/qppcad)
[![Documentation Status](https://readthedocs.org/projects/qppcad-doc/badge/?version=latest)](https://qppcad-doc.readthedocs.io/en/latest/?badge=latest)
![qppcad_collage](/docs/images/qppcad_coolage.jpg)
### current status - under construction
qpp::cad - molecule and crystal editor written in c++

# Installation

### Init submodules in one command
```bash
git clone https://github.com/nvpopov/qppcad.git --recurse-submodules
```

### or in two!
```bash
git clone https://github.com/nvpopov/qppcad.git
git submodule update --init --recursive
```

### Init && compile
```bash
git clone https://github.com/nvpopov/qppcad.git --recurse-submodules
cd qppcad
mkdir build
cd build
cmake ..
make
```

### Update
```bash
git pull --recurse-submodules
```

## Debian - based distro dependencies :
```bash
apt install libqt5opengl5-dev libqt5charts5-dev libqt5svg5-dev libqt5core5a libqt5widgets5 libqt5gui5 libpython3-dev
```

# Dependencies and resources
1. Qt5 - https://www.qt.io
2. Eigen3 - http://eigen.tuxfamily.org
3. Python3 - https://www.python.org/
4. Pybind11 - https://github.com/pybind/pybind11
5. JSON for Modern C++ - https://nlohmann.github.io/json/
6. {fmt} -  A modern formatting library https://fmt.dev
7. Pugixml - https://pugixml.org/
8. CppOptimizationLibrary - https://github.com/PatWie/CppNumericalSolvers
9. qpp - https://github.com/nvpopov/qpp
10. icons - https://material.io/resources/icons/
11. font - OpenSans