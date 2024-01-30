# 描述

将图像的 raw 数据转换为 jpg 等图像格式的文件；
将jpg等图像格式的文件转换为 raw 数据。

同理，可以仿照程序，编写 jpg 转换为 yuv 的数据；

## C++ 版本与批处理文件

### Windows

确保已经安装了 `visual studio`， `cmake`, `opencv` 等。

#### 生成解决方案

双击打开 `onekey_windows_gensln.bat`, 即可一键生成 vs2019 sln 文件。

#### 编译

进入 `win_build` 文件夹，打开 `convertRAW2jpg.sln` 文件就可以编译生成 `exe` 了；

**NOTE:** `cmake -G "Visual Studio 16 2019" -A x64 ..` 可以修改此处，生成适用于不同版本的 vs 项目。

另外，需要修改 `CMakeLists.txt` 文件里的 `OpenCV` 路径 `D:/opencv480vc16/build` 为你自己的 OpenCV 安装路径。我这里是直接下载的 binary 文件。

#### 执行

在 `temp_example` 下有两个 `exe`, `jpg2raw.exe` 和 `raw2jpg.exe`， 可以在控制台对单张图片进行转换；如果需要批量操作，可以使用批处理脚本；

**批量将 jpg 文件转换为 raw 文件：**
```
.\onekey_windows_batch2raw.bat .\testjpgs\
```

**批量将 raw 文件转换为 jpg 文件：**

**NOTE:** 在 `cpp/src` 目录下 `raw2jpg.cpp` 文件里，修改 `image_sizes` 以便支持更多的分辨率；

```
.\onekey_windows_batch2jpg.bat .\testraws\
```


## Python 版本

```bash
python image_converter.py [input_directory] [conversion_type (raw2jpg | jpg2raw)]
```

`[input_directory]`: 需要转换的文件路径；

`[conversion_type]`: 转换类型，raw2jpg 表示将 raw 文件转换为 jpg 文件，jpg2raw 表示将 jpg 文件转换为 raw 文件；


### Ubuntu

```bash
bash onekey_ubuntu_genmake.sh
```

然后在 `temp_example` 文件夹下执行批处理脚本或者可执行程序即可；

**批量将raw转换为jpg**

```bash
bash onekey_ubuntu_batch2jpg.sh ./testraws/
```

**批量将jpg转换为raw**

```bash
bash onekey_ubuntu_batch2raw.sh ./testjpgs/
```
