#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/stitching.hpp"
#include "opencv2/opencv.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/types.h>
#include <stdio.h>
#include <cstdio>
#include <memory>
#include <stdexcept>


using namespace std;
using namespace cv;

vector<Mat> imgs;
string execute(const char* cmd) {
    char buffer[128];
    std::string result = "";
    FILE* pipe = _popen(cmd, "r");
    if (!pipe) throw std::runtime_error("popen() failed!");
    try {
        while (fgets(buffer, sizeof buffer, pipe) != NULL) {
            result += buffer;
        }
    }
    catch (...) {
        _pclose(pipe);
        throw;
    }
    _pclose(pipe);
    return result;
}

int main(int argc, char* argv[])
{

    String path = "D:\\OneDrive - Avans Hogeschool\\Drone Area Mapping\\10. Example_data\\example_images2\\";

    cout << String("test") << endl;
    //cout << execute(String("cd " + path + " & powershell \"Get-ChildItem | Measure-Object | %{$_.Count}\"").c_str()) << endl;
    int fileCounter = 0;
    stringstream numOfFiles(execute(String("cd " + path + " & powershell \"Get-ChildItem | Measure-Object | %{$_.Count}\"").c_str()));
    numOfFiles >> fileCounter;
    cout << "fileCounter: " << fileCounter << endl;

    for (int i = 1; i < fileCounter; ++i)
    {
        String file = to_string(i + 1) + ".jpg";
        Mat img = imread(path+file);
        if (img.empty())
        {
            cout << "Can't read image '" << argv[i] << "'\n";
            return -1;
        }
        imgs.push_back(img);
    }

    Mat pano;

    Ptr<Stitcher> stitcher = Stitcher::create(Stitcher::SCANS);
    Stitcher::Status status = stitcher->stitch(imgs, pano);

    if (status != Stitcher::OK)
    {
        cout << "Can't stitch images\n";
        return -1;
    }
    imwrite(path+"result.jpg", pano);
    exit(0);
}

