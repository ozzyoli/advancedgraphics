#include <math.h>

float brightestPixel(unsigned int &imageSize, float* image) {
    float max = 0;
    for (int pixel = 0; pixel < imageSize; pixel++) {
        if (image[pixel] > max) {
          max = image[pixel];
        }
    }
    return max;
}
 
unsigned char* simpleToneMap(unsigned int &imageSize, float* input) {
    unsigned char* output = new unsigned char[imageSize];
    float max = brightestPixel(imageSize, input);
    for (int pixel = 0; pixel < imageSize; pixel++) {
        output[pixel] = (input[pixel] / max) * 255;
    }
    return output;
}

float* adjustExposure(unsigned int &imageSize, float* input, unsigned int stops) {
    float* output = new float[imageSize];
    for (int pixel = 0; pixel < imageSize; pixel++) {
        output[pixel] = input[pixel] * pow(2, stops);
        if (output[pixel] > 1.0) {
          output[pixel] = 1.0;
        }
    }
    return output;
}
