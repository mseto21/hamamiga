#include "Types.h"
#include "Rectangle.h"
#include "TextureCache.h"

struct Vector2
{
  float x;
  float y;
};

class Bullet {
  Vector2 position; //figure out how to use this with movement system?
  Vector2 velocity;
  bool alive; //alive or dead bullet?
  Texture* texture;
  uint32 bullet_life; //time until bullet dies
  Rectangle bullet;

  /**methods*/

  void load(Texture*);
  void update();
  void initialize(Rectangle);
  void display();
  /** Constructor */
public:
  Bullet();

};
