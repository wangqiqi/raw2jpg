@echo off

@REM 设置构建目录名称为win_build
set "build_dir=win_build"

@REM 检查构建目录是否存在，如果存在则删除并重新创建（包括子目录及所有文件）
if exist "%build_dir%" (
    rmdir /s /q %build_dir%
)

@REM 创建新的构建目录
mkdir "%build_dir%"

@REM 进入构建目录工作路径
cd /d "%build_dir%"

:: 配置CMake项目，使用Visual Studio 16 2019版本和x64架构生成解决方案
cmake -G "Visual Studio 16 2019" -A x64 ..

@REM 暂停脚本执行，等待用户按键继续，以便查看命令行输出结果
pause
