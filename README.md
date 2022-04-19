<!-- PROJECT LOGO -->
<br />
<p align="center">
  <a href="https://github.com/PKhuang-TW/Enchanter/blob/master/imgs/3.jpg">
    <img src="imgs/3.jpg" alt="MusicBox" width="500">
  </a>
  <h3 align="center">Music Box</h3>
  <p align="center">
    Self-Made Music Box
    <br />
    <a href="https://youtu.be/K19CnvB4NbM">View Demo</a>
  </p>
</p>

<!-- ABOUT THE PROJECT -->
## About The Project

This is a **Self-Made Music Box** built with Arduino. Music will be read through SD module (LC-SD) and output by a 8-Ohm 2W speaker. Buttons on the box supports **Play/Stop, Next/Previous**; LCD was prepared to show music name. However, the LCD module doesn't support Traditional Chinese output, so the LCD was not implemented in this project finally.

- [x] Read Music From SD card
- [x] Play music from speaker
- [x] Play/Stop, Next/Previous
- [ ] Show information from LCD

<p align="center">
    <img src="./imgs/3.jpg" width="35%"> <img src="./imgs/1.jpg" width="35%"><br>
    <img src="./imgs/2.jpg" width="35%"> <img src="./imgs/4.jpg" width="35%"> 
</p>

### Built With

* [Arduino Nano](https://store.arduino.cc/usa/arduino-nano)
* [LC Technology SD Card Module LC-SD](https://www.amazon.co.uk/LC-Technology-Module-LC-SD-Arduino/dp/B01LWK2VCK)
* 8-Ohm 2-W Speaker

### Schematic
<img src = "./imgs/schematic.png" class="projectDetailImg">


<!-- GETTING STARTED -->
## Getting Started

### Libraray
Import all the libraries in ./Library/
<p align="center">
  <img src="./imgs/importLibrary.png" width="50%">
</p>  

### Music Format
Music format must be .wav, 8 Bit resolution, 8 KHz sample rate, mono audio. You can convert your audio file with [this site](https://audio.online-convert.com/convert-to-wav).
<img src = "./imgs/ConvertEx.png" class="projectDetailImg">
