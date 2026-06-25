# Step 1: Setup

In this first step we prepare the computer and the board. At the end of this step the Arduino IDE can find the XIAO ESP32C3, compile a sketch, and upload it to the board.

## What Is Arduino?

![Arduino316](https://github.com/user-attachments/assets/8df9108e-2fce-4fc7-b660-835722d35bab)

Arduino is a simple way to program small computers.

- Originally created for art installations
- Common in education and prototyping
- Programmed with C/C++ sketches
- Supported by the Arduino IDE

## Our Thing: Seeed Studio XIAO ESP32C3

<img width="500" height="636" alt="board-pic" src="https://github.com/user-attachments/assets/cfa799da-872b-4a6b-b5ff-d1169de54596" />

For this workshop we use the Seeed Studio XIAO ESP32C3.

- 4 MB flash memory
- 400 KB SRAM
- EEPROM emulated in flash
- 160 MHz clock speed
- 11 digital I/O pins, including 4 analog inputs
- 3.3 V I/O standard
- Wi-Fi and Bluetooth

The 3.3 V I/O standard is important. Do not connect 5 V signals directly to the pins of this board.

## Install the Arduino IDE

Download and install the Arduino IDE:

https://www.arduino.cc/en/software

After installation, open the Arduino IDE.

On Windows:

<img width="924" height="706" alt="Screenshot 2025-07-31 at 10 55 33" src="https://github.com/user-attachments/assets/161c5c92-fe03-46ea-a43b-34dc7c18baf0" />

On macOS:

<img width="826" height="594" alt="Screenshot 2025-07-31 at 10 41 35" src="https://github.com/user-attachments/assets/84de77a5-bac4-4c51-a94f-fc8d51b5b09c" />

## Install the ESP32 Boards

The Arduino IDE does not know the XIAO ESP32C3 by default. We first need to install the ESP32 board package.

Open the Boards Manager.

<img width="857" height="465" alt="Screenshot 2025-07-31 at 11 27 15" src="https://github.com/user-attachments/assets/4010755e-bf22-40f0-b682-d84b3306eaec" />

Search for `esp32` or `esp32-c3`, then install **esp32** by **Espressif Systems**.

<img width="711" height="366" alt="Screenshot 2025-07-31 at 11 26 41" src="https://github.com/user-attachments/assets/1ce47b49-bd64-45e5-aca2-c6d2497ddf29" />

## Connect the Board

Connect the XIAO ESP32C3 to your computer with a USB-C cable.

Use a cable that supports data. Some USB-C cables can only charge devices. If the board does not appear in the Arduino IDE, try another cable first.

## Select the Board

Open the board selector.

<img width="442" height="75" alt="Screenshot 2025-07-31 at 11 32 07" src="https://github.com/user-attachments/assets/d033d2b6-e354-43a3-9f51-d65edb1af40d" />

Select **XIAO_ESP32C3**.

<img width="688" height="494" alt="Screenshot 2025-08-03 at 16 59 28" src="https://github.com/user-attachments/assets/94952ddd-da21-408e-99a0-c541ab6e3d83" />

## Select the Port

Select the USB serial port.

<img width="686" height="488" alt="Screenshot 2025-08-03 at 16 59 47" src="https://github.com/user-attachments/assets/15c62e49-11dc-482f-9e7f-5bc1aefb7b9c" />

On Windows this is usually a `COM` port, for example `COM3`, `COM4`, or a higher number. On macOS or Linux the name usually starts with `/dev/`.

If you are not sure which port is the board, unplug the board, look at the port list, plug the board in again, and check which port appeared.

## Check the Setup

Open a new empty sketch. The Arduino IDE should show the selected board and port.

<img width="573" height="385" alt="Screenshot 2025-08-19 at 17 39 48" src="https://github.com/user-attachments/assets/2b0415e1-2954-41c4-a8ab-645fcabb0c86" />

Select the board and the port if they are not selected yet.

<img width="692" height="500" alt="Screenshot 2025-08-19 at 17 42 48" src="https://github.com/user-attachments/assets/37f76855-44ba-4985-b6c6-de7a2946b48f" />

Click **Verify** to compile the empty sketch. This checks whether the software setup is working.

Click **Upload** to transfer the sketch to the board.

<img width="578" height="361" alt="Screenshot 2025-08-19 at 17 44 35" src="https://github.com/user-attachments/assets/7bda7dee-a246-4435-ac9b-8d7276b89fa1" />

The Arduino IDE first converts the C/C++ code into machine code.

<img width="1033" height="749" alt="Screenshot 2025-08-19 at 17 45 31" src="https://github.com/user-attachments/assets/3882622c-20b6-4306-94b4-a2533121087b" />

Then it transfers the machine code to the board.

<img width="1047" height="808" alt="Screenshot 2025-08-19 at 17 46 51" src="https://github.com/user-attachments/assets/579f6fde-533f-47f3-bfe7-fecb1dc7b51e" />

With a bit of luck, everything worked and your program does absolutely nothing. Great. That means the board can be programmed.

## If the Board Is Not Detected

Try these checks:

- Use a USB-C cable that supports data.
- Unplug the board and plug it in again.
- Close the Serial Monitor if it is open.
- Reopen the **Tools > Port** menu.
- Press the **RESET** button on the board once.
- Try another USB port.
- Restart the Arduino IDE.

If uploading gets stuck on `Connecting...`, press **RESET** when the message appears and try the upload again.

## Next

Now the computer and board are ready. Continue with [Blink](Blink.md), where we add a LED and make the first visible program.
