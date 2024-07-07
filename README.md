# Assault Cube 1.3.0.2 Internal Cheat
Assault Cube 1.3.0.2 cheat for windows written in C99, can be injected with any injector.

## Customization
When injected first time program automatically generates `config.json` at `Users\<username>\Documents\AssaultCubeInternal`.
```json
{
   "Aimbot": {
       "Enabled": true,
       "FOV": 100
   },
   "Triggerbot": {
       "Enabled": true,
       "Delay": 0
   },
   "ESP": {
       "Enabled": true,
       "RectangleOutlineColorFriend": [0, 255, 0, 240],
       "RectangleOutlineColorEnemy": [255, 0, 0, 240],
       "RectangleFillColorFriend": [0, 255, 0, 128],
       "RectangleFillColorEnemy": [255, 0, 0, 128],
       "RectangleVisibleFillColorFriend": [0, 255, 0, 64],
       "RectangleVisibleFillColorEnemy": [255, 0, 0, 64],
       "RectangleNameColorFriend": [0, 255, 0, 240],
       "RectangleNameColorEnemy": [255, 0, 0, 240],
       "LineColorFriend": [0, 255, 0, 240],
       "LineColorEnemy": [255, 0, 0, 240]
   }
}
```
Arrays for esp properties represents [R, G, B, A].

## Requirements
* MSVC
* Clang (Optional)
* Makefile (Optional)

## Disclaimer
This project is developed for educational purposes only. It is intended to demonstrate and teach various programming techniques, concepts, and methods. The author does not endorse or condone any misuse of the project.