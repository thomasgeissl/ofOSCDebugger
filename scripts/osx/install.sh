#!/bin/sh
OFVERSION=0.11.0

curl -O https://openframeworks.cc/versions/v${OFVERSION}/of_v${OFVERSION}_osx_release.zip
unzip -a -qq of_v${OFVERSION}_osx_release.zip
rm of_v${OFVERSION}_osx_release.zip
cd of_v${OFVERSION}_osx_release/apps
mkdir app && cd $_
git clone git clone https://github.com/$TRAVIS_REPO_SLUG.git && cd $(basename https://github.com/$TRAVIS_REPO_SLUG.git .git)
make