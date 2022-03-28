[![Issues](https://img.shields.io/github/issues/Airblader/unclutter-xfixes.svg)](https://github.com/Airblader/unclutter-xfixes/issues)
[![Forks](https://img.shields.io/github/forks/Airblader/unclutter-xfixes.svg)](https://github.com/Airblader/unclutter-xfixes/network)
[![Stars](https://img.shields.io/github/stars/Airblader/unclutter-xfixes.svg)](https://github.com/Airblader/unclutter-xfixes/stargazers)

# unclutter-xfixes

## About

This is a rewrite of the popular tool unclutter, but using the x11-xfixes extension. This means that this rewrite doesn't use fake windows or pointer grabbing and hence causes less problems with window managers and/or applications.

## Installation

### Arch / Manjaro

```
pacman -S unclutter
```

unclutter-xfixes is also available in the AUR as [unclutter-xfixes-git](https://aur.archlinux.org/packages/unclutter-xfixes-git/).

### Debian / Ubuntu

```
apt install unclutter-xfixes
```

### Fedora

unclutter-xfixes is available [via a copr repository](https://copr.fedorainfracloud.org/coprs/nbeernink/unclutter-xfixes/).

### FreeBSD

unclutter-xfixes is available in the FreeBSD Ports Collection as [misc/unclutter-xfixes](https://www.freshports.org/misc/unclutter-xfixes/).

### Manual

Make sure to install the missing dependencies first:

* libev-devel
* libX11-devel
* libXi-devel
* asciidoc 

You can then clone the git repository, compile and install it using like this:

```
git clone https://github.com/Airblader/unclutter-xfixes
cd unclutter-xfixes
make
sudo make install
```

## Usage

See `man unclutter` after installation.
