@echo off
FOR /F "tokens=17" %%i in ('"ipconfig | findstr IPv4"') do SET LOCAL_IP=%%i
echo %LOCAL_IP% > ip