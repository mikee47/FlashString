#!/bin/bash
set -ex # exit with nonzero exit code if anything fails

# Build times benefit from parallel building
export MAKE_PARALLEL="make -j3"

# Make sure Sming picks up this version of the library
export COMPONENT_SEARCH_DIRS=$TRAVIS_BUILD_DIR/..
touch .submodule

# Pull in Sming to a subdirectory of this Component
SMING_ROOT=$TRAVIS_BUILD_DIR/Sming
git clone https://github.com/mikee47/Sming --branch develop --single-branch --depth 1 $SMING_ROOT
export SMING_HOME=$SMING_ROOT/Sming

# Full compile checks please
export STRICT=1

env

# Build and run our tests
cd test
$MAKE_PARALLEL execute SMING_ARCH=Host
