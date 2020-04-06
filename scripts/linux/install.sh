#!/bin/sh
OFVERSION=0.11.0

curl -O https://openframeworks.cc/versions/v${OFVERSION}/of_v${OFVERSION}_linux64gcc6_release.tar.gz
tar -zxf of_v${OFVERSION}_linux64gcc6_release.tar.gz
rm of_v${OFVERSION}_linux64gcc6_release.tar.gz
sudo of_v${OFVERSION}_linux64gcc6_release/scripts/linux/ubuntu/install_dependencies.sh -y
cd of_v${OFVERSION}_linux64gcc6_release/apps
mkdir app && cd $_
git clone git clone https://github.com/$TRAVIS_REPO_SLUG.git && cd $(basename https://github.com/$TRAVIS_REPO_SLUG.git .git)
make