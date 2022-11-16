/**
 * @file HSLAPixel.h
 *
 * @author ngduy28
 */

#ifndef CS221UTIL_HSLAPixel_H
#define CS221UTIL_HSLAPixel_H

using namespace std;

namespace cs221util {
    class HSLAPixel {
        public:
            double h, s, l, a;  // h = hue; s = saturation; l = luminance; a = alpha (transparency)

            /**
            * Creates a default HSLA Pixel and sets to white.
            * h = hue; s = saturation; l = luminance; a = alpha (transparency)
            */
            HSLAPixel();
            /**
            * 3 arg constructor
            * @param hue Hue of pixel
            * @param sat Saturation of pixel
            * @param lum Luminance of pixel
            */
            HSLAPixel(double hue, double sat, double lum);
            /**
            * 4 arg constructor
            * @param hue Hue of pixel
            * @param sat Saturation of pixel
            * @param lum Luminance of pixel
            * @param alp opacity of pixel
            */
            HSLAPixel(double hue, double sat, double lum, double alp);
    };
}



#endif