# FlappyStorm-Game
Flappystorm is a Flappy Bird-inspired game with additional elements such as thunderclouds, lightning strikes, and power-ups. The game has its own 2D game engine, providing multiple useful features such as physics, game object management, XMB icon compatibility, and support for dynamic text labels. This project was created as a final project for the IS1200 - Computer Hardware Engineering course at (KTH) Royal Institute of Technology in Sweden. It's my first project in C and left me with much new knowledge and experience in low-level programming. The code can run on a ChipKIT (PIC32) with an attached Basic I/O Shield.

# Awards
* [KTH - Course IS1200/IS1500 - Best Projects Awards 2024](https://i.imgur.com/lRxViFd.jpeg)

## Media
   ![Picture of small monochrome screen of the game](https://i.imgur.com/sj5S7n3.png)
   * [Youtube Video](https://www.youtube.com/watch?v=bQGde61NOaw)

## Features
* 2D Game engine
  * Basic object physics (Such as gravity & velocity)
  * Dynamic game objects (Static objects)
  * Dynamic entities (Interactive objects)
  * Dynamic text labels
  * XBM icons compatibility
* Infinite side-scrolling game
* Pixel-by-pixel graphic
* Randomly generated obstacles
  * Stationary pipes
  * Thunder clouds
  * Lightning strikes
* Randomly generated power-ups
  * Mario star (Makes player invincible for a short time)
* Continuous scrolling ground texture
* 3 different player profiles
  * Individual high scores
  * Individual usernames
* HUD
  * Current score (Measured in time alive)
* Progressive difficulty
  * Faster moving obstacles
  * Shorter spawn delay between obstacles
  * Shorter activation time from power-ups
* Menus
  * Main menu (Select where to navigate)
  * Player selection (Select which player profile to use)
  * Name input (Select a username for player profile)
  * Leaderboard (See the top 3 high scores for all player profiles)
  * Death screen (See current score)
* Other
  * Pseudo-random number generator
  * 8 custom XBM icons

## Prerequisites
* [MCB32MX Toolchain](https://github.com/is1200-example-projects/mcb32tools)
* ChipKIT (PIC32)
* ChipKIT Basic I/O Shield

## Compile & Install
1. Enter the [MCB32MX Toolchain](https://github.com/is1200-example-projects/mcb32tools)
2. Enter the project directory
3. Run the command "make"
4. Run the command "make install"

## Credits:
This project includes small amounts of code from other developers.
See [CREDITS.md](CREDITS.md) for more information!
