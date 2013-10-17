#pragma once

#include "common.h"

class Cube;
class Face;
class Picker;

extern mat4 camera;//右键旋转变换
extern mat4 projection;//投影变换
extern mat4 view;//视图变换
extern mat4 scroll;//滚轮缩放变换

extern ivec2 mouseButtonRightLastpos;
extern bool mouseButtonRightReleased;

extern vector<Cube*> cubes;
extern vector<Face*> faces;

extern Picker picker;