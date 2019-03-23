# qpp::cad
![qppcad_collage](/docs/images/qppcad_coolage.jpg)
### current status - under construction
 molecule and crystal editor written in c++
### How to obtain the code

#### Init submodules in one command
```bash
git clone https://github.com/nvpopov/qppcad.git --recurse-submodules
```

#### or in two!
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

### Debian - based distro dependencies :
#### Qt5:
```bash
apt install libqt5opengl5-dev libqt5charts5-dev libqt5svg5-dev libqt5core5a libqt5widgets5 libqt5gui5
```
#### Python3:
```bash
apt install libpython3-dev
```

### CI Status
1. Travis CI [![Build Status](https://travis-ci.org/nvpopov/qppcad.svg?branch=master)](https://travis-ci.org/nvpopov/qppcad)

### Current implemented features
  [Go to implemented features gallery](docs/features-milestone.md)

