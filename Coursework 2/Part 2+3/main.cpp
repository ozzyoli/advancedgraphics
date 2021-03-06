/*//////////////////////////////////////////////////////////////////////////
Author: Abhijeet Ghosh
Year: 2013
//////////////////////////////////////////////////////////////////////////*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "loadPNM.h"
#include "sample.h"
#include "toneMapper.h"

//#define PI 3.14159265358979323
#define uint unsigned int

#include <iostream>
#include <sstream>
#include <string>

using namespace std;

unsigned int width;
unsigned int height;
unsigned int numComponents;

void CreateAndSavePFM(const char *image_out)
{
	width = 511; // set size of image to 511x511 pixels
	height = 511;
	numComponents = 3;
	
	float *img_out = new float [width*height*numComponents];

	for ( uint i = 0 ; i < height ; ++i ) // height
    {
		for ( uint j = 0 ; j < width ; ++j ) // width
		{
			for ( uint k = 0 ; k < numComponents ; ++k ) // color channels - 3 for RGB images
			{
				uint index = i*width*numComponents + j*numComponents + k; //index within the image

				//set image to white
				img_out[index] = 1.0f; //R
				img_out[index + 1] = 1.0f;//G
				img_out[index + 2] = 1.0f;//B
				
			}

		}
	}


	WritePFM(image_out, width, height, numComponents, img_out);

}

void LoadAndSavePPM(const char *image_in, const char *image_out)
{
	unsigned char *img_in = loadPNM(image_in, width, height, numComponents);
	unsigned char *img_out = new unsigned char [width*height*numComponents];

	for ( uint i = 0 ; i < height ; ++i ) // height
    {
		for ( uint j = 0 ; j < width ; ++j ) // width
		{
			for ( uint k = 0 ; k < numComponents ; ++k ) // color channels - 3 for RGB images
			{
				uint index = i*width*numComponents + j*numComponents + k; //index within the image

				img_out[index] = img_in[index]; //R
				img_out[index + 1] = img_in[index + 1];//G
				img_out[index + 2] = img_in[index + 2];//B
				
			}

		}
	}


	WritePNM(image_out, width, height, numComponents, img_out);

}

void LoadAndSavePFM(const char *image_in, const char *image_out)
{
	float *img_in = loadPFM(image_in, width, height, numComponents);
	float *img_out = new float [width*height*numComponents];

	for ( uint i = 0 ; i < height ; ++i ) // height
    {
		for ( uint j = 0 ; j < width ; ++j ) // width
		{
			for ( uint k = 0 ; k < numComponents ; ++k ) // color channels - 3 for RGB images
			{
				uint index = i*width*numComponents + j*numComponents + k; //index within the image

				img_out[index] = img_in[index]; //R
				img_out[index + 1] = img_in[index + 1];//G
				img_out[index + 2] = img_in[index + 2];//B
				
			}

		}
	}


	WritePFM(image_out, width, height, numComponents, img_out);

}

void LoadPPMAndSavePFM(const char *image_in, const char *image_out)
{
	unsigned char *img_in = loadPNM(image_in, width, height, numComponents);
	float *img_out = new float [width*height*numComponents];

	for ( uint i = 0 ; i < height ; ++i ) // height
    {
		for ( uint j = 0 ; j < width ; ++j ) // width
		{
			for ( uint k = 0 ; k < numComponents ; ++k ) // color channels - 3 for RGB images
			{
				uint index = i*width*numComponents + j*numComponents + k; //index within the image

				//typecast 0 - 255 values to the 0.0f -> 1.0f range 
				img_out[index] = static_cast<float>(img_in[index])/255.0f; //R
				img_out[index + 1] = static_cast<float>(img_in[index + 1])/255.0f;//G
				img_out[index + 2] = static_cast<float>(img_in[index + 2])/255.0f;//B
				
			}

		}
	}


	WritePFM(image_out, width, height, numComponents, img_out);

}

void LoadPFMAndSavePPM(const char *image_in, const char *image_out)
{
	float *img_in = loadPFM(image_in, width, height, numComponents);
	unsigned char *img_out = new unsigned char [width*height*numComponents];

	for ( uint i = 0 ; i < height ; ++i ) // height
    {
		for ( uint j = 0 ; j < width ; ++j ) // width
		{
			for ( uint k = 0 ; k < numComponents ; ++k ) // color channels - 3 for RGB images
			{
				uint index = i*width*numComponents + j*numComponents + k; //index within the image
				
				//typecast 0.0f -> 1.0f values to the 0 - 255 range 
				img_out[index] = static_cast<unsigned char>(img_in[index]*255.0f); //R
				img_out[index + 1] = static_cast<unsigned char>(img_in[index + 1]*255.0f);//G
				img_out[index + 2] = static_cast<unsigned char>(img_in[index + 2]*255.0f);//B
				
			}

		}
	}

	WritePNM(image_out, width, height, numComponents, img_out);

}

int main(int argc, char** argv) {
  unsigned int width = 1024;
  unsigned int height = 512;
  unsigned int channels = 3;
  unsigned int imageSize = width * height * channels;
  float* environmentMap = loadPFM("GraceCathedral/grace_latlong.pfm", width, height, channels);
  int samples [3] = {64, 256, 1024};
  // Part 2
  for (int s = 0; s < 3; s++) {
    int sampleSize = samples[s];
    float* sampleMapPFM = loadPFM("GraceCathedral/grace_latlong.pfm", width, height, channels);
    sampleEnvironmentMap(environmentMap, sampleMapPFM, sampleSize);
    // Write PFM
    stringstream s1;
    s1 << "samples_em_" << sampleSize << ".pfm";
    WritePFM(s1.str().c_str(), width, height, channels, sampleMapPFM);    
    // Write PPM
    sampleMapPFM = simpleToneMap(imageSize, sampleMapPFM);
    sampleMapPFM = adjustExposure(imageSize, sampleMapPFM, 8);
    sampleMapPFM = adjustGamma(imageSize, sampleMapPFM, 2.2);
    unsigned char *sampleMapPPM = toPixelValues(imageSize, sampleMapPFM);
    stringstream s2;
    s2 << "samples_em_" << sampleSize << ".ppm";
    WritePNM(s2.str().c_str(), width, height, channels, sampleMapPPM);
  }
  // Part 3
  int ns [4] = { 1, 10, 50, 200};
  for (int s = 0; s < 3; s++) {
    int sampleSize = samples[s];
    for (int i = 0; i < 4 ;i++) {
      float* sampleMapPFM = loadPFM("GraceCathedral/grace_latlong.pfm", width, height, channels);
      int n = ns[i];
      samplePhongModel(sampleMapPFM, n, sampleSize);
      // Write PFM
      stringstream s2;
      s2 << "samplesphong" << "ssize" << sampleSize << "n" << n << ".pfm";
      WritePFM(s2.str().c_str(), width, height, channels, sampleMapPFM);
      // Write PPM
      sampleMapPFM = simpleToneMap(imageSize, sampleMapPFM);
      sampleMapPFM = adjustExposure(imageSize, sampleMapPFM, 8);
      sampleMapPFM = adjustGamma(imageSize, sampleMapPFM, 2.2);  
      unsigned char *sampleMapPPM = toPixelValues(imageSize, sampleMapPFM);
      stringstream s;
      s << "samplesphong" << "ssize" << sampleSize << "n" << n << ".ppm";
      WritePNM(s.str().c_str(), width, height, channels, sampleMapPPM);
    }  
  }
  return 0;
}


