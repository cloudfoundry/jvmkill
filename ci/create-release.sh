#!/usr/bin/env bash

set -euo pipefail

RELEASE=$1
SNAPSHOT=$2

update_release() {
  local file=$1
  local repository=$2
  local version=$3

  sed -E -i '' "s|(^[ ]*ARTIFACTORY_REPOSITORY:[ ]*).+$|\1$repository|" $file
  sed -E -i '' "s|(^[ ]*VERSION:[ ]*).+$|\1$version|" $file
}

update_release ci/deploy-bionic.yml libs-release-local $RELEASE
update_release ci/deploy-osx.yml libs-release-local $RELEASE
update_release ci/deploy-trusty.yml libs-release-local $RELEASE
git add .
git commit --message "v$RELEASE Release"

git tag -s v$RELEASE -m "v$RELEASE"
git reset --hard HEAD^1

update_release ci/deploy-bionic.yml libs-snapshot-local $SNAPSHOT
update_release ci/deploy-osx.yml libs-snapshot-local $SNAPSHOT
update_release ci/deploy-trusty.yml libs-snapshot-local $SNAPSHOT
git add .
git commit --message "v$SNAPSHOT Development"
