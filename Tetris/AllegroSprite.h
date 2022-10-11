#pragma once
class AllegroSprite
{
private:
	ALLEGRO_BITMAP* bmp;
	int width;
	int height;
	int s_width;
	int s_height;
	int numrows;
	int numcols;
public:
	std::vector<ALLEGRO_BITMAP*> sprites;
	AllegroSprite(const char* filename);
	~AllegroSprite();
};

