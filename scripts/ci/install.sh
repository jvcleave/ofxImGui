#!/usr/bin/env bash
set -e

 if [ "$TARGET" == "ios" ]; then
 cd $TRAVIS_BUILD_DIR
  rm -Rf example-ios
 fi
 