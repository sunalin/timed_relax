@echo off
@echo on
@echo on

@echo.
@echo ����Qtʹ�û��� (ע: �鿴QtĿ¼��qtenv2.bat)
set PATH=C:\Qt\Qt5.14.2\5.14.2\mingw73_32\bin;C:\Qt\Qt5.14.2\Tools\mingw730_32\bin;%PATH%
::cd /D C:\Qt\Qt5.14.2\5.14.2\mingw73_32

@echo.
@echo ���ҵ�ǰĿ¼�µ�exe�ļ�, ��ִ��windeployqt����������ض�̬���ļ�
::cd /D D:\sunalin
for /f "delims=" %%A in ('dir /b *.exe') do windeployqt %%A 
::echo %file_exe%

::pause

