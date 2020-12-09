# 8Ball Pool Game

## Rules
This project is an implementation of the classic 8-ball Pool Game. In this Game these are the following rules/ important notes

Objective of the Game: Sink all your own color balls and then the 8ball.

### Caveats:
- First sunk ball will determine a player's ball colors
- Sinking the 8ball before sinking all your other 7 colored balls results in an automatic loss. Opponent wins
- Sinking the cue ball on any turn allows the other player to place the cue ball anywhere on the board
- Sinking a ball of your own color results in a second consecutive shot, unless a foul has been committed 

### Fouls
- Any shot that fouls automatically becomes opponents turn no matter the circumstance
- A foul in this version is when a cue ball is sunk, 
- Another foul is when on your turn, you first hit a ball color that is not your own, or the 8ball*
- *Exception to the 8ball touch is when all your balls are pocketed

# Game Controls/Screen:
- In this game the stick represents a normal cue stick. The ball is being shot in the direction "away" from the cue stick
- To aim, rotate your mouse around the cue ball. The cue stick will follow your mouse
- To shoot, click down and drag your mouse forwards or backwards, whichever feels more natural. This will lock in the direction 
- The power bar will fluctuate at the bottom, which is correlated to the length of your drag,
- If prompted, click anywhere on the boar to place the cue ball in the result of the previous player sinking a cue ball
- The Ball Racks on the sides indicate how many ball and which color balls need to still be sunk in order to win the game.

# Dependencies

**Windows or Mac**
* [CMake](https://cmake.org/)
* [Cinder (v0.9.2)](https://libcinder.org/download)
**For windows, download [Visual Studio 2015](https://my.visualstudio.com/Downloads?q=visual%20studio%202015&wt.mc_id=o~msft~vscom~older-downloads) in order to use cinder*

# How To Run Program:
1) Download Cinder v0.9.2
2) Open the ~/Cinder folder in an IDE and run it in order to build Cinder
3) For windows, download Visual Studio to run
4) Download repo into a new folder within the ~/Cinder folder ex. my-projects folder
5) Build project and run cinder-app 

# Images 

<img width="1228" alt="Screen Shot 2020-12-08 at 8 35 10 PM" src="https://user-images.githubusercontent.com/37272048/101586637-30698a00-3997-11eb-9831-da855c61a734.png">


<img width="1371" alt="Screen Shot 2020-12-08 at 8 35 22 PM" src="https://user-images.githubusercontent.com/37272048/101586737-6f97db00-3997-11eb-9691-824d4aac5e73.png">


<img width="1388" alt="Screen Shot 2020-12-08 at 8 35 40 PM" src="https://user-images.githubusercontent.com/37272048/101586816-9b1ac580-3997-11eb-8622-1aa0cddad266.png">

<img width="1397" alt="Screen Shot 2020-12-08 at 8 36 35 PM" src="https://user-images.githubusercontent.com/37272048/101586869-b1c11c80-3997-11eb-8916-e9edd152ee90.png">

<img width="1402" alt="Screen Shot 2020-12-08 at 8 37 56 PM" src="https://user-images.githubusercontent.com/37272048/101586890-bbe31b00-3997-11eb-8009-daff71a1d9f2.png">
