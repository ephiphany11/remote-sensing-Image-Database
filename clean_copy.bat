@echo off
echo =============================================
echo     正在创建干净的 C++ 工程副本...
echo =============================================

:: 源代码根目录
set "SRC=D:\cly.project\02.rectangleSelectImg\rectangleSelectImg"

:: 目标根目录
set "DEST=D:\clean_project\02.rectangleSelectImg\rectangleSelectImg"

:: 创建目标目录结构
mkdir "%DEST%\rectangleSelectImg" 2>nul

:: 复制 sln
copy "%SRC%\rectangleSelectImg.sln" "%DEST%\" >nul

:: 复制 vcxproj 配置
copy "%SRC%\rectangleSelectImg\rectangleSelectImg\rectangleSelectImg.vcxproj*" "%DEST%\rectangleSelectImg\" >nul

:: 复制 cpp 和 h 文件
copy "%SRC%\rectangleSelectImg\*.cpp" "%DEST%\rectangleSelectImg\" >nul
copy "%SRC%\rectangleSelectImg\*.h"   "%DEST%\rectangleSelectImg\" >nul

echo ===== 副本完成 =====
pause
