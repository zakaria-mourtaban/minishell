#!/bin/bash

# Define the branches to be updated
branches=("ahmad" "mkraytem" "zakaria" "omar")

# Update local main branch
echo "Updating local main branch..."
git checkout main
git pull origin main

# Iterate over each branch and merge main into it
for branch in "${branches[@]}"; do
  echo "Switching to branch $branch..."
  
  # Check if the branch exists locally
  if git show-ref --verify --quiet refs/heads/"$branch"; then
    git checkout "$branch"
  else
    # Fetch the branch if it does not exist locally
    echo "Branch $branch not found locally. Fetching from remote..."
    git fetch origin "$branch:$branch"
    git checkout "$branch"
  fi
  
  echo "Merging main into $branch with 'theirs' strategy..."
  
  # Perform the merge with conflict resolution strategy
  git merge -X theirs main

  if [ $? -ne 0 ]; then
    echo "Merge conflicts occurred in branch $branch. Automatically resolved by choosing 'theirs' version."
  fi

  echo "Pushing changes to remote branch $branch..."
  git push origin "$branch"
  
  echo "Done with branch $branch."
done

echo "All branches updated successfully."

