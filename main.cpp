#include <opencv2/opencv.hpp>
#include <opencv2/freetype.hpp>
#include <string>
#include <thread>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

using namespace cv;
#define TERM_WIDTH 300
#define TERM_HEIGTH 110
#define NR_THREADS 4

/*
Desaturares and resizes the image.

input is the income image you wish to turn in to black and white and resize
output contains the resized and desaturated images
width is the width that output image should become
height is the height that the output image should become
*/
void imageDesatAndResize(Mat *input, Mat *output, const uint width, const uint height)
{
  if (input == NULL)
    return;

  cvtColor(*input, *output, COLOR_BGR2GRAY);
  cv::resize(*output, *output, Size(width, height), INTER_LINEAR);
}

/*
maps the value x, in a range between out_min and out_max in relation to in_min and in_max

x is the number to remap
in_min is the minimal number that x can be
in_max is the maximum number that x can be
out_min is the minimum number that the result can become
out_max is the maximum number that the result can become

returns a long that is mapped between out_min and out_max
*/
long map(int x, int in_min, int in_max, int out_min, int out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/*
Pad left.
str will be padded
num is the number of the left padding
paddingChar is the character that needs to be padded to left
*/
void padTo(std::string &str, const size_t num, const char paddingChar = ' ')
{
    if(num > str.size())
        str.insert(0, num - str.size(), paddingChar);
}

/*
turnImageToAscii will look at each pixel value and find based on it's value the appropriate char from chars array.
These chars in the chars array, simulate the luminance.
Each pixel maps to a single character. So when you have 80x24 char screen the input images should be 80x24 pixels.

input is the rescaled, grey scale image to be mapped in ascii chars
*/
void turnImageToAscii(Mat *image, const char *path, const uint frame_count)
{
  if (image == NULL)
    return;

  cv::Mat small(TERM_HEIGTH, TERM_WIDTH, CV_8UC3, cv::Scalar(0));
  cv::Mat output(1080, 1920, CV_8UC3, cv::Scalar(0));

  imageDesatAndResize(image, &small, TERM_WIDTH, TERM_HEIGTH);
  
  int cn = small.channels();
  static char chars[] = "B@#W$9876543210?!abc;:+=-,._   ";

  std::string sfcount = std::to_string(frame_count);
  padTo(sfcount, 6, '0');
  std::string p = path + std::string("/") +  sfcount + ".png";

  int fontHeight = 12;
  int thickness = -1;
  int linestyle = 8;
  int baseline = 0;
  cv::Ptr<cv::freetype::FreeType2> ft2;
  ft2 = cv::freetype::createFreeType2();
  ft2->loadFontData( "./ibm.ttf", 0 );
  std::string s = "";

  for (int i = 0; i < small.rows; i++)
  {
    s = "";
    for (int j = 0; j < small.cols; j++){
      s += chars[map(small.at<uint8_t>(i, j), 255, 0, 0, strlen(chars) - 1)];
    }

    //Size textSize = ft2->getTextSize(s,fontHeight,thickness,&baseline);
    //std::cout << std::to_string(textSize.height) << std::endl;
    Point textOrg(1, (10 * i));    
    ft2->putText(output, s, textOrg, fontHeight, Scalar(0,255,0), thickness, linestyle, true );
  }

  imwrite(p, output);
  //delete(ft2);
}

/*
just a convenience wrapper for the thread
*/
void t_resize(Mat *image, const int width, const int height)
{
  cv::resize(*image, *image, Size(width, height), INTER_LINEAR);
}

bool file_exists(const char *name)
{
  struct stat buffer;
  return (stat(name, &buffer) == 0);
}

/*
Returns > 0 when dir is found
Returns 0 when dir is not a dir or can't be found
Returns -1 when if an error occurs
*/
int dir_exits(const char *path){
   struct stat info;

    int statRC = stat( path, &info );
    if( statRC != 0 )
    {
        if (errno == ENOENT || errno == ENOTDIR) return 0; 
        return -1;
    }

    return ( info.st_mode & S_IFDIR ) ? 1 : 0;
}

/*
Check the input arguments.
We require <source video> which is the video you want to convert into ascii
We require <render directory> where the PNG sequence will be stored
*/
void arguments(VideoCapture *cap, int argc, char **args)
{
  if (argc != 3)
  {
    std::cerr << args[0] << " <source video> <render directory>" << std::endl;
    exit(1);
  }
  
  if (!file_exists(args[1])){
    std::cerr << "could not find file " << args[1] << std::endl;
    exit(1);
  }

  if (dir_exits(args[2])!=1){
    std::cerr << "could not find or use target directory " << args[2] << std::endl;
    exit(1);
  }

  if (cap==nullptr){
    std::cerr << "could not initialize video capture" << std::endl;
    exit(1);
  } else{
    cap->open(args[1]);
  }
}

/*
Check the input arguments.
We require <source video> which is the video you want to convert into ascii
We require <render directory> where the PNG sequence will be stored
*/
int main(const int argc, char **argv)
{
  Mat image[NR_THREADS];
  bool running = true;
  VideoCapture cap;

  arguments(&cap, argc, argv);
  
  if (!cap.isOpened())
    return 1;

  static uint frame_count = 0;
  std::thread threads[NR_THREADS];

  while (running)
  {

    for (int i=0; i<NR_THREADS; i++){
      cap >> image[i];
      
      if (image[i].empty())
        return 0;

      // takes the small black and white image and maps each pixel to a character and prints it to the terminal
      threads[i] = std::thread(turnImageToAscii, &image[i], argv[2], frame_count);

      frame_count++;
    }

    for(int i=0; i<NR_THREADS; i++){
      threads[i].join();
    }
  }

  return 0;
}
