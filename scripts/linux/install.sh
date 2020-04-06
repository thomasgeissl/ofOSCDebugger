#!/bin/sh
OFVERSION=0.11.0
GH_USER=$(dirname $TRAVIS_REPO_SLUG)
GH_REPO=$(basename $TRAVIS_REPO_SLUG)

curl -O https://openframeworks.cc/versions/v${OFVERSION}/of_v${OFVERSION}_linux64gcc6_release.tar.gz
tar -zxf of_v${OFVERSION}_linux64gcc6_release.tar.gz
rm of_v${OFVERSION}_linux64gcc6_release.tar.gz
sudo of_v${OFVERSION}_linux64gcc6_release/scripts/linux/ubuntu/install_dependencies.sh -y
cd of_v${OFVERSION}_linux64gcc6_release/apps
mkdir ${GH_USER} && cd $_
git clone https://github.com/${TRAVIS_REPO_SLUG}.git && cd ${GH_REPO}
make