@echo off
@echo on
@echo on

@echo.
@echo 设置Qt使用环境 (注: 查看Qt目录下qtenv2.bat)
set PATH=C:\Qt\Qt5.14.2\5.14.2\mingw73_32\bin;C:\Qt\Qt5.14.2\Tools\mingw730_32\bin;%PATH%
::cd /D C:\Qt\Qt5.14.2\5.14.2\mingw73_32

@echo.
@echo 查找当前目录下的exe文件, 并执行windeployqt命令搜索相关动态库文件
::cd /D D:\sunalin
for /f "delims=" %%A in ('dir /b *.exe') do windeployqt %%A 
::echo %file_exe%

::pause

