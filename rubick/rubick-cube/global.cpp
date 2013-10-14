#include "global.h"

mat4 camera;//右键旋转变换
mat4 projection;//投影变换
mat4 view;//视图变换
mat4 scroll;//滚轮缩放变换

ivec2 mouseButtonRightLastpos;
bool mouseButtonRightReleased = true;

Pick pick;

vector<Cube*> cubes;