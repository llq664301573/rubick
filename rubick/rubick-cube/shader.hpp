#ifndef SHADER_HPP
#define SHADER_HPP

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

#include <GL/glew.h>

GLuint loadShaders(int n, ...);

#endif
