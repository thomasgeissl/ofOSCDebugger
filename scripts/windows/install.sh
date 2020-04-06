OFVERSION=0.11.0
OFPLATFORM=msys2
GH_USER=$(dirname $TRAVIS_REPO_SLUG)
GH_REPO=$(basename $TRAVIS_REPO_SLUG)

curl -O https://openframeworks.cc/versions/v${OFVERSION}/of_v${OFVERSION}_${OFPLATFORM}_release.zip
unzip -a -qq of_v${OFVERSION}_${OFPLATFORM}_release.zip
rm of_v${OFVERSION}_${OFPLATFORM}_release.zip
of_v${OFVERSION}_${OFPLATFORM}_release/scripts/${OFPLATFORM}/install_dependencies.sh --noconfirm
cd of_v${OFVERSION}_${OFPLATFORM}_release/apps
mkdir ${GH_USER} && cd $_
git clone https://github.com/${TRAVIS_REPO_SLUG}.git && cd ${GH_REPO}
make