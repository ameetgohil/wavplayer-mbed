// Soundboard that plays 1 of 4 .wav files stored on the SD card based on 1 of
// 4 buttons pressed

#include "mbed.h"
#include "wave_player.h"
#include "SDFileSystem.h"

// .wav files to play
const char *filenames[4] = { "/sd/good_morning.wav", 
                             "/sd/questions.wav", 
                             "/sd/lack_discipline.wav", 
                             "/sd/stop_whining.wav"};

// Define buttons
DigitalIn button_1(p11);
DigitalIn button_2(p12);
DigitalIn button_3(p13);
DigitalIn button_4(p14);

// USB serial (tx, rx)
Serial pc(USBTX, USBRX);

// SD card
SDFileSystem sd(p5, p6, p7, p8, "sd");

// Audio out (DAC)
AnalogOut       DACout(p18);
wave_player     waver(&DACout);

// Play a .wav file
int playSound(int file_num) {

    FILE *file;

    // Open sound file for reading
    file = fopen(filenames[file_num], "r");
    if ( file == NULL ) {
        error("ERROR: Could not open file for reading!\n");
        return -1;
    }

    // Play the sound file
    pc.printf("Playing sound clip %i\r\n", (file_num + 1));
    waver.play(file);

    // Reset to beginning of file and close it
    fseek(file, 0, SEEK_SET);
    fclose(file);

    return 0;
}

int main() {

    // Use internal pull-up resistors
    button_1.mode(PullUp);
    button_2.mode(PullUp);
    button_3.mode(PullUp);
    button_4.mode(PullUp);

    pc.printf("\r\nHardware Soundboard\r\n");

    while(1) {

        // Figure out which button was pressed and play that file
        if ( button_1 == 0 ) {
            playSound(0);
        }
        if ( button_2 == 0 ) {
            playSound(1);
        }
        if ( button_3 == 0 ) {
            playSound(2);
        }
        if ( button_4 == 0 ) {
            playSound(3);
        }

        // Wait 10ms before sampling the buttons again
        wait(0.01);
    }
}
