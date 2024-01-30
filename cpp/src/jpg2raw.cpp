#include <fstream>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

// Windows 和 Linux 下通用的创建目录函数
#ifdef _WIN32
#include <direct.h> // For _mkdir on Windows
#define MKDIR(dir) _mkdir(dir)
#else
#include <sys/stat.h> // For mkdir on POSIX systems
#define MKDIR(dir) mkdir(dir, 0755)
#endif

bool create_directory(const std::string &dir_path)
{
#ifdef _WIN32
    if (_mkdir(dir_path.c_str()) != 0)
    {
        return false;
    }
#else
    if (MKDIR(dir_path.c_str()) != 0 && errno != EEXIST)
    {
        return false;
    }
#endif
    return true;
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: ./image_to_raw input_image.jpg" << std::endl;
        return -1;
    }

    std::string input_image_path = argv[1];
    std::string base_name = input_image_path.substr(input_image_path.find_last_of('/') + 1); // Unix风格路径分隔符
#ifdef _WIN32
    std::replace(base_name.begin(), base_name.end(), '\\', '/'); // 对于Windows下的反斜杠，统一转换为正斜杠
#endif

    std::cout <<" ----------- input_image_path: " << input_image_path << " ------------------------"<<std::endl;
    std::cout <<" ----------- basename: " << base_name << " ------------------------"<<std::endl;

    std::string raw_dir = "raws/";
    // std::string output_raw_path = raw_dir + base_name + ".raw";

    std::string file_name = input_image_path.substr(input_image_path.find_last_of("/\\") + 1);
    std::string file_name_without_suffix = file_name.substr(0, file_name.find_last_of("."));
    std::string file_name_with_suffix = file_name_without_suffix + ".raw";
    std::string output_raw_path = raw_dir + file_name_with_suffix;


    // 创建 'raw' 子目录（如果不存在）
    if (!create_directory(raw_dir))
    {
        std::cerr << "Failed to create directory: " << raw_dir << std::endl;
        //return -1;
    }

    cv::Mat img = cv::imread(input_image_path, 1);
    if (img.empty())
    {
        std::cerr << "Failed to load the image: " << input_image_path << std::endl;
        return -1;
    }

    int im_width = img.cols;
    int im_height = img.rows;
    int im_depth = img.channels();

    std::cout << "im_width: " << im_width << std::endl;
    std::cout << "im_height: " << im_height << std::endl;
    std::cout << "im_depth: " << im_depth << std::endl;

    // 写入 RAW 文件
    std::ofstream raw_file(output_raw_path, std::ios::binary);
    if (!raw_file.is_open())
    {
        std::cerr << "Failed to open file for writing: " << output_raw_path << std::endl;
        return -1;
    }
    raw_file.write(reinterpret_cast<const char *>(img.data), im_width * im_height * im_depth);
    raw_file.close();

    std::cout << "Converted " << input_image_path << " to " << output_raw_path << std::endl;

    return 0;
}