#include "AllegroResources.h"
#include "AllegroSprite.h"

AllegroSprite::AllegroSprite(const char* filename)
    : bmp(NULL), width(0), height(0), s_width(32), s_height(32), numrows(0), numcols(0),  sprites()
{
    bmp = al_load_bitmap(filename);

    if (bmp == NULL) {
        DebugWriteMsg(string_format("Error: AllegroSprite: al_load_bitmap: File not found: %s", filename));
        return;
    }
    al_convert_mask_to_alpha(bmp, (al_map_rgb(255, 0, 255)));
    width = al_get_bitmap_width(bmp);
    height = al_get_bitmap_height(bmp);
    numrows = height / s_height; //the tiles are 32/32
    numcols = width / s_width;
    for (int h = 0; h < numrows; h++)
    {
        for (int w = 0; w < numcols; w++)
        {
            sprites.push_back(al_create_sub_bitmap(bmp, w * s_width, h * s_height, s_width, s_height));
        }
    }
}

AllegroSprite::~AllegroSprite()
{
    for (auto sprite : sprites) {
        if(sprite != NULL)
            al_destroy_bitmap(sprite);
    }
    sprites.clear();
    if (bmp != NULL) {
        al_destroy_bitmap(bmp);
        bmp = NULL;
    }
}