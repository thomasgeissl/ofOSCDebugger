OFVERSION=0.11.0
OFPLATTFORM=msys2
GH_USER=$(dirname $TRAVIS_REPO_SLUG)
GH_REPO=$(basename $TRAVIS_REPO_SLUG)

curl -O https://openframeworks.cc/versions/v${OFVERSION}/of_v${OFVERSION}_${OFPLATTFORM}_release.zip
unzip -a -qq of_v${OFVERSION}_${OFPLATTFORM}_release.zip
rm of_v${OFVERSION}_${OFPLATTFORM}_release.zip
cd of_v${OFVERSION}_${OFPLATTFORM}_release/apps
mkdir ${GH_USER} && cd $_
git clone https://github.com/${TRAVIS_REPO_SLUG}.git && cd ${GH_REPO}
make