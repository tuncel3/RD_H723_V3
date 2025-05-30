@echo off
cd /d "D:\aaa\CubeMX2\RD_H723_V3"

:: Show the last 10 commits
echo Listing last 100 commits:
git --no-pager log --oneline -n 500
echo.

:: Get the commit hash for the 2nd previous backup
set /p commit_hash="Copy and paste the commit hash: "

:: Confirm before proceeding
echo You are about to restore files from commit %commit_hash% and save it as a new backup.

:: Reset local files to the selected commit (without deleting remote history)
git checkout %commit_hash% -- .

:: Add and commit the restored files as a new backup
git add .
git commit -m "Restored from %commit_hash% and saved as new backup on %date% %time%"

:: Push the new backup to the remote repository
git push origin main

echo Restore complete! Your local files are now saved as a new backup remotely.
