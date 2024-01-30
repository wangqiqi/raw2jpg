#include <fstream>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <unordered_map>

#ifdef _WIN32
#include <direct.h> // For _mkdir on Windows
#define MKDIR(dir) _mkdir(dir)
#else
#include <sys/stat.h> // For mkdir on POSIX systems
#define MKDIR(dir) mkdir(dir, 0755)
#endif

struct ImageSize
{
    int width;
    int height;
};

const std::unordered_map<int, ImageSize> image_sizes = {
    {921600, {640, 480}},
    {2764800, {1280, 720}},
    {5858640, {1896, 1030}},
    {6220800, {1920, 1080}},
    {11059200, {2560, 1440}},
    {12257280, {2688, 1520}},
    {15116544, {2592, 1944}},
    {3686400, {1280, 960}}};

// 跨平台创建目录函数（仅支持Linux和Windows）
bool create_directory(const std::string &path)
{
#ifdef _WIN32
    return (_mkdir(path.c_str()) == 0);
#else
    return (::mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0);
#endif
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: ./raw_image_viewer raw_image.raw" << std::endl;
        return -1;
    }

    std::string raw_image_path = argv[1];
    std::cout << "raw_image_path: " << raw_image_path << std::endl;

    // 获取文件大小
    std::ifstream file(raw_image_path, std::ios::binary | std::ios::ate);
    size_t file_size = file.tellg();
    file.close();
    std::cout << "size: " << file_size << " bytes" << std::endl;

    // 查找对应的图像尺寸
    auto it = image_sizes.find(file_size);
    if (it == image_sizes.end())
    {
        std::cout << file_size << " is not supported!" << std::endl;
        return -1;
    }
    const ImageSize &size = it->second;
    int im_width = size.width;
    int im_height = size.height;
    int im_depth = 3; // 假设所有图像都是RGB三通道

    std::cout << "im_width: " << im_width << std::endl;
    std::cout << "im_height: " << im_height << std::endl;
    std::cout << "im_depth: " << im_depth << std::endl;

    cv::Mat raw_image(im_height, im_width, CV_8UC3);

    // 读取 RAW 文件的内容
    std::ifstream raw_file(raw_image_path, std::ios::binary);
    raw_file.read(reinterpret_cast<char *>(raw_image.data), im_width * im_height * im_depth);
    raw_file.close();

    // 提取并创建 images 文件夹（跨平台）
    std::string images_path = "images/";
    bool dir_created = create_directory(images_path);

    std::string file_name = raw_image_path.substr(raw_image_path.find_last_of("/\\") + 1);
    std::string file_name_without_suffix = file_name.substr(0, file_name.find_last_of("."));
    std::string file_name_with_suffix = file_name_without_suffix + ".jpg";
    std::string output_path = images_path + file_name_with_suffix;

    cv::imwrite(output_path, raw_image);

    std::cout << " images_path: " << images_path << std::endl;
    std::cout << " file_name_with_suffix: " << file_name_with_suffix << std::endl;
    std::cout << " file save name: " << output_path << std::endl;

    if (!dir_created)
    {
        std::cerr << "Failed to create 'images' directory." << std::endl;
    }

    // （可选）显示图像
    // cv::imshow("raw_image", raw_image);
    // cv::waitKey(5);
    // cv::destroyAllWindows();

    // （可选）删除原始 RAW 文件
    // std::remove(raw_image_path.c_str());

    return 0;
}