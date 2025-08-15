#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

// Initialize software serial on pins 10 and 11
SoftwareSerial mySoftwareSerial(10, 11);  // RX, TX
DFRobotDFPlayerMini myDFPlayer;
String line;
char command;
int pause = 0;
int repeat = 0;

// Define PINS !!!!!!!!!!!!!!!!!!!
// #define PWMA_PIN 16
// #define AIN1_PIN 17
// #define AIN2_PIN 5
// #define PWMB_PIN 14

void setup() {
  // Serial communication with the module
  mySoftwareSerial.begin(9600);
  // Initialize Arduino serial
  Serial.begin(115200);
  // Check if the module is responding and if the SD card is found
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini"));
  Serial.println(F("Initializing DFPlayer module ... Wait!"));

  if (!myDFPlayer.begin(mySoftwareSerial)) {
    Serial.println(F("Not initialized:"));
    Serial.println(F("1. Check the DFPlayer Mini connections"));
    Serial.println(F("2. Insert an SD card"));
    while (true)
      ;
  }
  Serial.println();
  Serial.println(F("DFPlayer Mini module initialized!"));
  // Initial settings
  myDFPlayer.setTimeOut(500);  // Serial timeout 500ms
  myDFPlayer.volume(5);        // Volume 5
  myDFPlayer.EQ(0);            // Normal equalization
  //  myDFPlayer.EQ(DFPLAYER_EQ_POP);
  //  myDFPlayer.EQ(DFPLAYER_EQ_ROCK);
  //  myDFPlayer.EQ(DFPLAYER_EQ_JAZZ);
  //  myDFPlayer.EQ(DFPLAYER_EQ_CLASSIC);
  //  myDFPlayer.EQ(DFPLAYER_EQ_BASS);
  menu_options();

  // pinMode(PWMA_PIN, OUTPUT);
  // pinMode(AIN1_PIN, OUTPUT);
  // pinMode(PWMC_PIN, OUTPUT);

  // PUSH BUTTON
  // pinMode(AIN2_PIN, INPUT); 

}

void loop() {
  // Waits for data entry via serial
  while (Serial.available() > 0) {
    command = Serial.peek();
    line = Serial.readStringUntil('\n');

    // Play from first 9 files
    if ((command >= '1') && (command <= '9')) {
      Serial.print("Music reproduction");
      Serial.println(command);
      command = command - 48;
      myDFPlayer.play(command);
      menu_options();
    }

    //Play from specific folder
    if (command == 'f') {
      int indexF = line.indexOf('f');
      int indexS = line.indexOf('s');
      if (indexF != -1 && indexS != -1 && indexF < indexS) {
        int folder = line.substring(indexF + 1, indexS).toInt();
        int song = line.substring(indexS + 1).toInt();
        Serial.print("From folder: ");
        Serial.print(folder);
        Serial.print(", playing song: ");
        Serial.println(song);
        myDFPlayer.playFolder(folder, song);  //play specific mp3 in SD:/folder/song.mp3; Folder Name(1~99); File Name(1~255)
      } else {
        Serial.println("Incomplete 'f' command. Specify both folder and song numbers.");
      }
      menu_options();
    }

    // Reproduction
    // Stop
    if (command == 's') {
      myDFPlayer.stop();
      Serial.println("Music Stopped!");
      menu_options();
    }

    // Pause/Continue the music
    if (command == 'p') {
      pause = !pause;
      if (pause == 0) {
        Serial.println("Continue...");
        myDFPlayer.start();
      }
      if (pause == 1) {
        Serial.println("Music Paused!");
        myDFPlayer.pause();
      }
      menu_options();
    }

    // Toggle repeat mode
    if (command == 'r') {
      repeat = !repeat;
      if (repeat == 1) {
        myDFPlayer.enableLoop();
        Serial.println("Repeat mode enabled.");
      } else {
        myDFPlayer.disableLoop();
        Serial.println("Repeat mode disabled.");
      }
      menu_options();
    }

    // Set volume
    if (command == 'v') {
      int myVolume = line.substring(1).toInt();
      if (myVolume >= 0 && myVolume <= 30) {
        myDFPlayer.volume(myVolume);
        Serial.print("Current Volume:");
        Serial.println(myDFPlayer.readVolume());
      } else {
        Serial.println("Invalid volume level, choose a number between 0-30.");
      }
      menu_options();
    }

    // Increases volume
    if (command == '+') {
      myDFPlayer.volumeUp();
      Serial.print("Current Volume:");
      Serial.println(myDFPlayer.readVolume());
      menu_options();
    }
    // Decreases volume
    if (command == '-') {
      myDFPlayer.volumeDown();
      Serial.print("Current Volume:");
      Serial.println(myDFPlayer.readVolume());
      menu_options();
    }

    // Play previouse
    if (command == '<') {
      myDFPlayer.previous();
      Serial.println("Previous:");
      Serial.print("Current track:");
      Serial.println(myDFPlayer.readCurrentFileNumber() - 1);
      menu_options();
    }

    // Play next
    if (command == '>') {
      myDFPlayer.next();
      Serial.println("Next:");
      Serial.print("Current track:");
      Serial.println(myDFPlayer.readCurrentFileNumber() + 1);
      menu_options();
    }



    // PUSH BUTTON TRIGGERINGS !!!!!!
    // if (AIN2_PIN == 1) {

    // }
  }
}

void menu_options() {
  Serial.println();
  Serial.println(F("=================================================================================================================================="));
  Serial.println(F("Commands:"));
  Serial.println(F(" [1-9] To select the MP3 file"));
  Serial.println(F(" [fXsY] Play song from folder X, song Y"));
  Serial.println(F(" [s] stopping reproduction"));
  Serial.println(F(" [p] pause/continue music"));
  Serial.println(F(" [r] toggle repeat mode"));
  Serial.println(F(" [vX] set volume to X"));
  Serial.println(F(" [+ or -] increases or decreases the volume"));
  Serial.println(F(" [< or >] forwards or backwards the track"));
  Serial.println();
  Serial.println(F("================================================================================================================================="));
}

