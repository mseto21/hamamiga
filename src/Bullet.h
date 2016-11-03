#include "Types.h"
#include "Rectangle.h"
#include "TextureCache.h"

struct Vector2
{
  float x;
  float y;
};

class Bullet {
public:
  Vector2 position; //figure out how to use this with movement system?
  Vector2 velocity;
  bool alive; //alive or dead bullet?
 // bool init;
  //Texture* texture;
  //float frameTime;    /* Time for frame to update */
  //float currentFrameTime; /* Current time for frame */
  Rectangle rect;
  //uint32 age;

  /**methods*/
public:
  void load(Texture*);
  void update(uint32);
  void initialize(Rectangle, float);
  void display();
  /** Constructor */
  Bullet();

};
