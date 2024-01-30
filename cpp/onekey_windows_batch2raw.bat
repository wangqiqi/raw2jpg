@echo off

@REM 如果没有输入路径, 就使用当前路径
if "%1"=="" (
    set "im_path=%cd%"
) else (
    set "im_path=%~1"
)

@REM 输出当前处理的路径
echo Batch converting image files to raw images in path: %im_path%

@REM 遍历指定目录及其子目录下的所有.jpg, .jpeg, .bmp, .png文件
for /R "%im_path%" %%i in (*.jpg, *.jpeg, *.bmp, *.png) do (
    @REM 获取图片名称（这里保留完整路径）
    echo Processing image: %%i
    .\jpg2raw.exe "%%i"
)

pause
