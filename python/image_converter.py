# 导入必要的库，包括用于文件和目录操作的os，用于图像处理的cv2，以及系统调用的sys模块
import os
import cv2
import sys
from typing import Tuple
import numpy as np 

# 定义一个函数，根据给定的文件大小（以字节为单位）返回图像的宽度、高度及通道数
def get_image_dimensions(file_size: int) -> Tuple[int, int, int]:
    # 这个函数假设了某些特定文件大小对应着已知的图像分辨率，并且所有图像都是RGB三通道格式
    image_sizes = {
        921600: (640, 480),
        2764800: (1280, 720),
        5858640: (1896, 1030),
        6220800: (1920, 1080),
        11059200: (2560, 1440),
        12257280: (2688, 1520),
        15116544: (2592, 1944),
        3686400: (1280, 960)
    }

    # 获取与给定文件大小匹配的宽高信息，如果没有匹配项，则返回None作为宽高
    im_width, im_height = image_sizes.get(file_size, (None, None))
    
    # 假设图像深度固定为3（即RGB三通道）
    return im_width, im_height, 3

# 将RAW图像数据转换为常规图像文件（例如JPEG）的函数
def raw_to_image(input_raw_path: str, output_image_path: str):
    # 计算RAW文件的大小
    file_size = os.path.getsize(input_raw_path)

    # 根据文件大小获取图像尺寸
    im_width, im_height, im_depth = get_image_dimensions(file_size)
    if im_width is None:
        print(f"{file_size} is not supported!")
        return -1

    # 创建一个空numpy数组来存储图像数据，其形状基于得到的尺寸和通道数
    img = np.empty((im_height, im_width, im_depth), dtype=np.uint8)
    
    # 打开RAW文件并读取内容到numpy数组中
    with open(input_raw_path, "rb") as f:
        f.readinto(img.data)

    # 使用OpenCV将图像数据写入输出图像文件
    cv2.imwrite(output_image_path, img)
    
    # 输出转换成功的消息
    print("Converted RAW to image:", input_raw_path, "->", output_image_path)

# 将常规图像文件转换为RAW数据的函数
def image_to_raw(input_image_path: str, output_raw_path: str, width: int, height: int, depth=3):
    # 加载输入图像
    img = cv2.imread(input_image_path, cv2.IMREAD_UNCHANGED)
    if img is None:
        print("Failed to load the image:", input_image_path)
        return -1

    # 确保图像数据符合预期的宽高和通道数
    img = img.reshape(height, width, depth).astype(np.uint8)
    
    # 打开输出RAW文件并写入图像数据
    with open(output_raw_path, "wb") as f:
        f.write(img.tobytes())
    
    # 输出转换成功的消息
    print("Converted image to RAW:", input_image_path, "->", output_raw_path)


def process_images(directory: str, conversion_type: str):
    # 检查输入路径是否为有效的目录
    if not os.path.isdir(directory):
        print(f"Directory {directory} does not exist.")
        return

    # 获取当前脚本所在的目录作为基准输出目录
    current_script_dir = os.path.dirname(os.path.abspath(__file__))
    output_directory = os.path.join(current_script_dir, "output_" + conversion_type)

    # 确保输出目录存在
    if not os.path.exists(output_directory):
        os.makedirs(output_directory)

    # 遍历目录及其子目录中的所有文件
    for root, dirs, files in os.walk(directory):
        for file in files:
            # 构造当前文件的完整路径
            input_file_path = os.path.join(root, file)
            
            # 分离出文件名和扩展名
            base_name, ext = os.path.splitext(file)

            # 根据指定的转换类型进行不同的处理
            if conversion_type == 'raw2jpg':
                # 如果是RAW文件，将其转换为JPEG图像
                if ext.lower() != '.raw':
                    continue
                output_file_path = os.path.join(output_directory, base_name + ".jpg")
                raw_to_image(input_file_path, output_file_path)

            elif conversion_type == 'jpg2raw':
                # 如果是支持的图像文件，将其转换为RAW数据
                if not ext.lower().endswith(('.jpg', '.jpeg', '.png', '.bmp')):
                    continue
                # 获取原始图像的尺寸
                width, height, _ = cv2.imread(input_file_path, cv2.IMREAD_UNCHANGED).shape
                output_file_path = os.path.join(output_directory, base_name + ".raw")
                image_to_raw(input_file_path, output_file_path, width, height)

# 当脚本直接运行时，从命令行参数中获取输入目录和转换类型
if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: python image_converter.py [input_directory] [conversion_type (raw2jpg|jpg2raw)]")
        sys.exit(-1)

    directory = sys.argv[1]
    conversion_type = sys.argv[2]

    if conversion_type not in ['raw2jpg', 'jpg2raw']:
        print("Invalid conversion type. Please choose either 'raw2jpg' or 'jpg2raw'.")
        sys.exit(-1)

    # 开始转换过程
    process_images(directory, conversion_type)