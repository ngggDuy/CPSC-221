#include "block.h"
#include <iostream>
#include <cmath>


int Block::width() const
{
    return data[0].size();
}

int Block::height() const
{
    return data.size();
}

/*  REQUIRES: 0 <= x
              0 <= y
    EFFECTS:  renders block at specified location (x,y) for
              blocks top left corner
*/
void Block::render(PNG &im, int column, int row) const
{
    for (int y = row; y < row + height(); y++) {
        for (int x = column; x < column + width(); x++) {
            HSLAPixel * curr = im.getPixel(x,y);
            *curr = data[y-row][x-column];
        }
    }
}

/*  REQUIRES: 0 <= row < row + height < im.height
              0 <= column < column + width < im.width
    EFFECTS: creates new block with specified region of 
             pixels copied over from im
*/
void Block::build(PNG &im, int column, int row, int width, int height)
{
    for (unsigned y = 0; y < (unsigned)height; y++) {
        vector<HSLAPixel> temp;
        for (unsigned x = 0; x < (unsigned)width; x++) {
            HSLAPixel* p = im.getPixel(x + column, y + row);
            temp.push_back(*p);
        }
        data.push_back(temp);
        //cout << "finished row" << y << endl;
    }
}

void Block::flip()
{
    unsigned height = data.size();
    vector<HSLAPixel> temp;
    for (unsigned y = 0; y < height / 2; y++) {
        temp = data[y];
        data[y] = data[height - y - 1];
        data[height - y - 1] = temp;
    }
}
