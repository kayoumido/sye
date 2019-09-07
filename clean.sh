#!/bin/bash

echo Cleaning metadata locally and remotely...

git rm -rf .metadata

find . -name ".project" | xargs git rm -rf
find . -name ".cproject" | xargs git rm -rf
find . -name ".settings" | xargs git rm -rf

echo Make sure metadata locally disappears...
rm -rf .metadata
find . -name ".project" | xargs rm -rf
find . -name ".cproject" | xargs rm -rf
find . -name ".settings" | xargs rm -rf

echo Committing git...
git commit -m "Cleaning Eclipse metadata."

echo Pushing ...
git push


