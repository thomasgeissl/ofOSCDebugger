#!/bin/sh
OFVERSION=0.11.0
OFPLATFORM=linux64gcc6
GH_USER=$(dirname $TRAVIS_REPO_SLUG)
GH_REPO=$(basename $TRAVIS_REPO_SLUG)

curl -O https://openframeworks.cc/versions/v${OFVERSION}/of_v${OFVERSION}_${OFPLATFORM}_release.tar.gz
tar -zxf of_v${OFVERSION}_${OFPLATFORM}_release.tar.gz
rm of_v${OFVERSION}_${OFPLATFORM}_release.tar.gz
sudo of_v${OFVERSION}_${OFPLATFORM}_release/scripts/linux/ubuntu/install_dependencies.sh -y
cd of_v${OFVERSION}_${OFPLATFORM}_release/apps
mkdir ${GH_USER} && cd ${GH_USER}
git clone https://github.com/${TRAVIS_REPO_SLUG}.git && cd ${GH_REPO}
make