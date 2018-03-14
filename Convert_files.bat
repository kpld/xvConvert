@echo off
for /r xvfiles %%a in (*.xv) do if /i "%%~xa"==".xv" xvConvert "%%~a"
