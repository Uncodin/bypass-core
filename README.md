# Bypass Core

*The homepage for this project is [located here](http://uncodin.github.io/bypass/).*

## Introduction

Bypass Core represents markdown in an intermediary tree format so
that it can be rendered directly in Android and iOS apps. If you are looking
for an android library that renders markdown, [check out the
bypass-android project](https://github.com/Uncodin/bypass-android). If you are
looking for an iOS library that renders markdown, [check out
bypass-ios project](https://github.com/Uncodin/bypass-ios).

This project is licensed under the [Apache License v2.0](http://www.tldrlegal.com/l/APACHE2)
and leverages portions of [libsoldout](http://fossil.instinctive.eu/libsoldout/home),
which is licensed under the [ISC license](http://fossil.instinctive.eu/libsoldout/artifact/c8d2f5b1e9e1df422ca06d1bc846d9e3055a925b).

## Prerequisites

This project has been compiled and tested with the following configurations:

* Mac OS X 10.8.4
	* automake 1.13.1
	* autoconf 2.69
* Debian 6.0.7 (Linux 2.6.32-5-amd64)
	* automake 1.14
	* autoconf 2.69

In order to compile and test, you may have to install automake and possibly
autoconf from source since the supplied versions are probably too old to run
the custom test harness included with this project. I suspect that it will work
with versions of automake greater than or equal to 1.12, but I have never tried
it. Please report successful build configurations or make changes to this file
direcly and submit a pull request.

You can [download automake here](http://ftp.gnu.org/gnu/automake/), and
[download autoconf here](http://ftp.gnu.org/gnu/autoconf/).

## Building and Testing

After cloning this repository you can build and test by issuing the following
commands:

    autoreconf --install
    ./configure
    make check

Additionally, you can generate the documentation with:

    make doxygen-doc
