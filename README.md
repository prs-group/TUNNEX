# TUNNEX
Tunneling in experiments (TUNNEX) is a free open-source program with an easy-to use graphical user interface (GUI) to simplify the process of WKB computations.

## How to use

TUNNEX can be directly downloaded at [Releases](https://github.com/prs-group/TUNNEX/releases).

After the generation of the input files for TUNNEX with [make_projections](https://github.com/prs-group/make_projections). You are ask to input the IRC and projected IRC into the gui. The import options can be found under File. TUNNEX automatically computes the tunneling half-lives.

## Compilation

### prerequisite

- Qt Library 5.6 or greater. 
- ALGLIB (included in source)
- QtCustomPlot (included in source)

### Build System

TUNNEX relies on the **cmake** > 2.9 build system. It is recommended to do the following:

```
git clone https://github.com/prs-group/TUNNEX TUNNEX
cd TUNNEX
mkdir build
cd build
cmake .. -DCMAKE_PREFIX_PATH=[QtInstallPath]
make 
```

For more information see: [cmake: Useful Variables](https://gitlab.kitware.com/cmake/community/wikis/doc/cmake/Useful-Variables).

The compilation is tested on the following operating systems:

- MacOS 10.13 with XCode and makefiles
- Windows with Visual Studio 17 as generator and ninja makefiles
- Linux with Makefiles

## How to install

After compilation just enter:

```
make install
```
