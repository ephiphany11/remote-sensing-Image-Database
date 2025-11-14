@echo off
echo =============================================
echo     正在创建干净的 ImageSelectBFS 工程副本...
echo =============================================

:: 源工程根路径
set "SRC=D:\cly.project\ImageSelectBFS"

:: 目标目录
set "DEST=D:\clean_project\ImageSelectBFS"

:: 创建目标目录
mkdir "%DEST%" 2>nul

:: 复制 sln
copy "%SRC%\ImageSelectBFS.sln" "%DEST%\" >nul

:: 复制 vcxproj 文件
copy "%SRC%\ImageSelectBFS\ImageSelectBFS.vcxproj*" "%DEST%\" >nul

:: 复制 cpp 和 h 文件
copy "%SRC%\ImageSelectBFS\*.cpp" "%DEST%\" >nul
copy "%SRC%\ImageSelectBFS\*.h"   "%DEST%\" >nul

echo ===== ImageSelectBFS 项目副本完成 =====
pause
