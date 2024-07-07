#ifndef CONFIG_H
#define CONFIG_H

#include "gl_utils.h"

extern float c_fTriggerbotDelay;
extern float c_fAimbotFov;

extern Color4 c_ESPRectangleOutlineColorFriend;
extern Color4 c_ESPRectangleOutlineColorEnemy;
extern Color4 c_ESPRectangleFillColorFriend;
extern Color4 c_ESPRectangleFillColorEnemy;
extern Color4 c_ESPRectangleVisibleFillColorFriend;
extern Color4 c_ESPRectangleVisibleFillColorEnemy;
extern Color4 c_ESPRectangleNameColorFriend;
extern Color4 c_ESPRectangleNameColorEnemy;
extern Color4 c_ESPLineColorFriend;
extern Color4 c_ESPLineColorEnemy;

void Config_Load();

#endif