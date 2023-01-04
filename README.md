# CasioMessage
 
This project has the goal of implementing a messaging app to a casio fx-CG50, with an ESP32 using bluetooth and UART

# Wiring

the wiring of the ESP32 in the calculator :


![shematic](https://github.com/RaphoufouLeFou/Casio-message-and-connect/blob/main/Images/shematic.jpg)

<span style="color:red">**WARNING**, I forgot to put a limiting current resistor between IO19 and the calculator !</span>

Don't forget the 10K pullup resistor betwin 3.3V an EN !

TX of the calculator need to be connected to the RX pin of the ESP32, and the RX of the calculator to the TX pin of the ESP32.

IO19 is the control signal of the exam led. The pin can change in the code of the ESP32

# Connect app

The connect app is used to configure the ESP32 via the calculator, connect to another calculator, or set the exam led blinking mode.

The app present like this :

![shematic](https://github.com/RaphoufouLeFou/Casio-message-and-connect/blob/main/Images/Connect-1.jpg)

* F1 start the communication with the ESP32.

* F2 send the signal to the ESP32 to blink the exam led or not. IT DOED **NOT** START THE REAL EXAM MODE OF THE CALCULATOR !

* F3 is to open the test mode of the calculator.

* F4 send a request to the ESP32 to get the avalable bluetooth arount to connect to

If you press F4, you will get this window :

![shematic](https://github.com/RaphoufouLeFou/Casio-message-and-connect/blob/main/Images/Connect-2.jpg)

The avalables bluetooth are listed here. the limit of bluetooth to be display is 5.

You can click on the key 1 to 5 to connect to the corresponding bluetooth.

* F1 open the window to create a bluetooth server to let other calculator to select your's.

* F6 return to the first window.

if you press F1, you will get this window :

![shematic](https://github.com/RaphoufouLeFou/Casio-message-and-connect/blob/main/Images/Connect-3.jpg)

Now, you can  press SHIFT + ALPHA to lock the letter mode, and start typing a name for the new bluetooth to create.

* F6 send you back to the previous window.

* EXE send the signal to the ESP32 to start the bluetooth with the name typed.

# Message app

This app is to communicate with other calculators.

![shematic](https://github.com/RaphoufouLeFou/Casio-message-and-connect/blob/main/Images/Message-1.jpg)

You type your message and then press EXE to send it to the calculator previously connected in the Connect app.

* F6 to receve new messages.
