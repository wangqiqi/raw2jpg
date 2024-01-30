@echo off

@REM 如果没有输入路径, 就使用当前路径
@if "%~1"=="" (
    set "im_path=%cd%"
) else (
    set "im_path=%~1"
)

@REM 输出批处理将要转换文件的路径信息
echo Batch converting RAW images to JPG in the current directory or specified path: %im_path%

@REM 遍历指定目录及其子目录下的所有.raw文件
for /R "%im_path%" %%i in (*.raw) do (
    @REM 获取图片名称（不包括完整路径）
    set "img_file=%%~ni"
    @REM 输出正在处理的原始图像文件名
    echo Processing image: %%i
    @REM 调用 raw2jpg.exe 进行转换，由于程序内部已经设置了输出目录和文件名规则，所以这里仅传入原始文件路径即可
    .\raw2jpg.exe "%%i"
)

@REM 暂停脚本执行，等待用户按键继续
pause