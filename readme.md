git checkout main<br>
git pull origin main<br>
git checkout -b newbranchname<br>
git add *<br>
git commit -m "pushmessage"<br>
git push orgin newbranchname<br>
to remove old local branches<br>
git fetch --prune && git branch -d $(git branch --merged | grep -v 'main')
