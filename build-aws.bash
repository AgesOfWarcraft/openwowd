#!/bin/bash
set -e

source build-aws-vars.bash

SHORT_SHA="$(git rev-parse --short HEAD)"
BRANCH_NAME="$CODEBUILD_GIT_CLEAN_BRANCH"

BUILD_TAG="$BRANCH_NAME-$SHORT_SHA"

if [[ "$BRANCH_NAME" == "" ]]; then
	BRANCH_NAME="adhoc"
fi

echo "Initiating a build of $DOCKERHUB_USERNAME/$PROJECT_NAME:$BUILD_TAG"

echo "$DOCKERHUB_PASSWORD" | docker login --password-stdin --username $DOCKERHUB_USERNAME
docker build . -t $DOCKERHUB_USERNAME/$PROJECT_NAME:$BUILD_TAG
docker push $DOCKERHUB_USERNAME/$PROJECT_NAME:$BUILD_TAG

if [[ "$BRANCH_NAME" == "$RELEASE_ON_BRANCH" ]]; then
	docker push $DOCKERHUB_USERNAME/$PROJECT_NAME:latest
fi
