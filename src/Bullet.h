#include "Types.h"
#include "Rectangle.h"
#include "TextureCache.h"

struct Vector2
{
  float x;
  float y;
};

class Bullet {
 private:
  //Vector2 position; //figure out how to use this with movement system?
  //Vector2 speed;
  uint16 x;
  uint16 y;
  uint16 w;
  uint16 h;

 public:
 	//Rectangle bullet;
  bool alive; //alive or dead bullet?
  Texture* texture;
	//uint16 width; //bullet width
  //uint16 height; //bullet height

  /**methods*/

  void load(Texture*);
  void update();
  void initialize(Rectangle);
  void display();
  /** Constructor */
  Bullet();

};
