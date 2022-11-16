
// File:        tileUtil.cpp
// Author:      Cinda
// Date:        2020-10-31
// Description: file handling functions that prepare data for the tiling algorithm


#include "tileUtil.h"
#include <map>
#define THUMBNAIL_SIZE 900
#define TILE_SIZE 30

/**
 * Function tile:
 * @param PNG & target: an image to use as base for the mosaic. it's pixels will be
 *                      be replaced by thumbnail images whose average color is close
 *                      to the pixel.
 * @param rgbtree & ss: a kd-tree of RGBAPixels, used as a query structure for
 *                      nearest neighbor search. 
 * @param map<RGBAPixel, string> & photos: a map that takes a color key and returns the
 *                      filename of an image whose average color is that key.
 *
 * returns: a PNG whose dimensions are TILESIZE times that of the target. Each
 * pixel in the target is used as a query to ss.findNearestNeighbor, and the response
 * is used as a key in photos. 
 */

PNG tiler::tile(PNG & target, const rgbtree & ss, map<RGBAPixel,string> & photos)
{
    PNG returnImage(target.width() * TILE_SIZE, target.height() * TILE_SIZE);
    for (unsigned int x = 0; x < target.width(); x++) {
        for (unsigned int y = 0; y < target.height(); y++) {
            RGBAPixel * query = target.getPixel(x, y);
            RGBAPixel neighbour = ss.findNearestNeighbor(*query);
            PNG thumb; thumb.readFromFile(photos[neighbour]);
            unsigned int x_coord = x * TILE_SIZE;
            unsigned int x_end = (x + 1) * TILE_SIZE;
            unsigned int y_coord = y * TILE_SIZE;
            unsigned int y_end = (y + 1) * TILE_SIZE;
            for (unsigned int i = 0; i < TILE_SIZE; i++) {
                if (x_coord <= x_end){
                    for (unsigned int j = 0; j < TILE_SIZE; j++) {
                        if (y_coord <= y_end){
                            RGBAPixel * p = thumb.getPixel(i, j);
                            RGBAPixel * toPaint = returnImage.getPixel(x_coord, y_coord);
                            *toPaint = * p;
                        }
                        y_coord++;
                    }
                }
                x_coord++;
            }
        }
    } 
    return returnImage;
}

/* buildMap: function for building the map of <key, value> pairs, where the key is an
 * RGBAPixel representing the average color over an image, and the value is 
 * a string representing the path/filename.png of the TILESIZExTILESIZE image
 * whose average color is the key.
 * 
 * We've provided a bit of the C++ code that allows you to iterate over the files
 * in a directory. It is up to you to figure out what it means, and how to use it.
 * 
 * @param path is the subdirectory in which the tiles can be found. In our examples
 * this is imlib.
 *
*/
map<RGBAPixel, string> tiler::buildMap(string path) {

    map <RGBAPixel, string> thumbs;
    for (const auto & entry : fs::directory_iterator(path)) {
        PNG curr; curr.readFromFile(entry.path()); 

        unsigned int totalRed, totalGreen, totalBlue;
        totalRed = 0; 
        totalGreen = 0; 
        totalBlue = 0;

        for (unsigned int x = 0; x < curr.width(); x++) {
            for (unsigned int y = 0; y < curr.height(); y++) {
                RGBAPixel * p = curr.getPixel(x, y);
                totalRed   += p->r;
                totalGreen += p->g;
                totalBlue  += p->b;
            }
        }

        unsigned int averageRed   = totalRed   / THUMBNAIL_SIZE;
        unsigned int averageGreen = totalGreen / THUMBNAIL_SIZE;
        unsigned int averageBlue  = totalBlue  / THUMBNAIL_SIZE;
        RGBAPixel key(averageRed, averageGreen, averageBlue);
        thumbs.insert({key, entry.path()});
    }
    return thumbs;
}


