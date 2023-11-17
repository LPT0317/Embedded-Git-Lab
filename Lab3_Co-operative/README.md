#LAB3 - Co-operative
##Introduction
This lab is an example of using serial monitor to check whether the ESP32 development board sends the text: "Hello world!" or not.
This lab is writting and running with the Espressif IDE.
##How to run
1. Import project by choosing File->Open Project from Filesystem
2. Choose Directory... and select lab directory
3. Unchoose Search for nested projects and click on Finish
4. On the toolbar, select ESP Target this option is after the word "on" and change the target to "esp32"  and COM port (You can setup this by click on the icon in this option).
5. On the toolbar, choose Run->Run or Ctrl+F11 (on Windows) to run the project on ESP32 board.
6. Open FreeRTOSConfig.h by using Ctrl + left mouse button on "#include "freertos/FreeRTOSConfig.h" (Espressif-IDE)
7. Change value of "configUSE_PREEMPTION" to 0
8. Open sdkconfig file and choose tab "FreeRTOS" on the left hand side of menu
0. Tick the box "Run FreeRTOS only on first core" to yes