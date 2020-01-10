# 2019 Smart Cushion
### Author : 
Wan Hsuan Lin(林宛萱) wanhsuan0506@gmail.com
楊欣睿
### Demo video : 
https://www.youtube.com/watch?v=AC_ZPusRJfo&feature=share
## 
## Abstract
We use DISCO-L475VG-IOT01A to develop a portable device that can be put in a cushion and record our sitting posture. We use 3 FSR sensors connected to L475VG to record the pressure while we sit on the chair. L475VG will calculate the data, analysizing user's current sitting posture. 
We divide sitting posture into several types : 1. empty, 2. sit well, 3. tilt left/right, 4. sitting too long, 5, moving. 
L475VG uses socket to communicate with PC. L475VG serves as socket client, while PC serves as socket server. It will send raw data and the current state of the user to PC. In PC side, we develope a desktop application to show the data and send a warning message to the user while they are in bad posture.


## Content
* Motivation
* Implemetation
* Setup

## Motivation
Nowadays, people spend a lot of time sitting on chair. However, bad sitting posture and long-time sitting will cause damage to our health. Thus, we decide to develop a smart cushion that can be portable and easily installed on our chair to help us take care our health.  


## Implemetation
I. System Structure
![](https://i.imgur.com/J2QPdSt.png)

II. HardWare Assembling
III. User Interface
Begin, press start to connect to socket client
![](https://i.imgur.com/DZcJnsd.png)
After clicking start, connecting to the L475VG
![](https://i.imgur.com/M0M4mAX.png)
Successful connection. Start to exchange data. We can click the `exit` button to disconnect.
![](https://i.imgur.com/IpDMvQe.png)
The raw data will show on the top right corner of the view.
![](https://i.imgur.com/O3AkyfR.png)
When PC recieves that the user are in bad posture, it will send warning message.
![](https://i.imgur.com/9Wtxl41.png)


## Setup
### Board Side

### User Side
Requirement : SDL2, SDL_ttf 2.0, run on Linux
1. Download our repository
2. Move to SDL folder. It contains a makefile, a src folder with source files (the main cpp is named main_render.vpp), a pic folder with the pictures used by SDL2, and a text folder withe the ttf file.
3. type `make`  , and it will generate a executable binary file, Smartcushion
5. type `./Smartcushion`, then the program will start.
