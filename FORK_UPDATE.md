# Add a new remote upstream repository
git remote add upstream https://github.com/pbialas7-Teaching/3DGraphicsCodeBase.git

# Sync your fork
git fetch upstream
git checkout main
git merge upstream/main
