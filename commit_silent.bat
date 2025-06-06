@echo off
cd /d "D:\aaa\CubeMX2\RD_H723_V3"
git config --global credential.helper manager

:: Get current date and time
for /f "tokens=2 delims==" %%I in ('wmic os get localdatetime /value') do set datetime=%%I
set datetime=%datetime:~0,4%-%datetime:~4,2%-%datetime:~6,2% %datetime:~8,2%:%datetime:~10,2%

:: Skip commit+push if no changes (new files dahil)
git status --porcelain | findstr . >nul
if %errorlevel% neq 0 (
    echo No changes to commit. Exiting.
    goto :eof
)

:: Auto message
set user_message=auto
set commit_message="Auto commit on %datetime% - %user_message%"

echo Adding changes...
git add .

echo Committing changes...
git commit -m %commit_message%

echo Pushing to GitHub...
git push origin main 2> error.log

if %errorlevel% neq 0 (
    echo Normal push failed! Attempting FORCE PUSH...
    git push --force origin main 2>> error.log
)

if %errorlevel% neq 0 (
    echo Force push also failed! See error.log for details.
    pause
) else (
    echo Push successful!
)
