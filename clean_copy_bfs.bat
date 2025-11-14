@echo off
echo =============================================
echo     正在创建 ImageSelectBFS 干净工程副本...
echo =============================================

:: 源工程根路径
set "SRC=D:\cly.project\03.ImageSelectBFS\ImageSelectByBFS"

:: 目标目录
set "DEST=D:\clean_project\03.ImageSelectBFS\ImageSelectByBFS"

:: 创建目录结构
mkdir "%DEST%\ImageSelectByBFS" >nul 2>nul

:: 复制 sln
copy "%SRC%\ImageSelectByBFS.sln" "%DEST%\" >nul

:: 复制 vcxproj 相关文件
copy "%SRC%\ImageSelectByBFS\ImageSelectByBFS.vcxproj*" "%DEST%\ImageSelectByBFS\" >nul

:: 复制 cpp 和 h
copy "%SRC%\ImageSelectByBFS\*.cpp" "%DEST%\ImageSelectByBFS\" >nul
copy "%SRC%\ImageSelectByBFS\*.h"   "%DEST%\ImageSelectByBFS\" >nul

echo ===== 副本完成 =====
pause
