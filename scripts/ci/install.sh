#!/usr/bin/env bash
set -e

 if [ "$TARGET" != "ios" ]; then
 cd $TRAVIS_BUILD_DIR
  rm -Rf /home/travis/openFrameworks/addons/ofxImGui/example-ios
 fi
 