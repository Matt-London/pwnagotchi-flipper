# Pwnagotchi-Flipper interface
This program will interface the pwnagotchi with the flipper. This will be accomplished using custom code on the pwnagotchi's end to give the flipper simple rendering instructions over UART.

<img src='doc/attachments/PwnZeroBaseWFace.png' alt='Flipper screen showing base Pwnagotchi display' height="128" width="256"/>
<img src="doc/attachments/PwnInAction.gif" width="256" height="160"/>

## Layout
This project is broken down into two parts: flipagotchi and pwnzero
```
pwnagotchi-flipper
 |--> flipagotchi/
 |--> pwnzero/
```
- flipagotchi is the Flipper-side application
- pwnzero is the pwnagotchi-side application

## Setup
### Flipagotchi Setup (Flipper side)
<b>The flipagotchi app can be downloaded from the flipper app store.</b> If you would like to do things manually then follow these instructions.
1. Connect your Flipper to your computer
2. Clone the Flipper Zero firmware onto your machine
3. Place the ```flipagotchi/``` directory into the ```applications_user/```
4. Open a terminal and navigate to the root of the firmware
5. Execute the following command to compile the app and launch it on the Flipper:<br>
    ```./fbt launch_app APPSRC=applications_user/flipagotchi```
6. This will now compile and load the app onto your Flipper

### PwnZero Setup (Pwnagotchi side)
This procedure will explain how to configure the Pwnagotchi to use the PwnZero plugin to communicate with the Flipper.
1. Disable Bluetooth on the Pi by adding ```dtoverlay=disable-bt``` at the bottom of the ```/boot/firmware/config.txt``` file
    1. This needs to be disabled so that the full UART is directed to ```/dev/serial0```
2. Enter the raspberry pi configuration settings with `sudo raspi-config`
    1. Select `Interface Options`
    2. Select `Serial Port`
    3. Select `No` for shell over serial
    4. Select `Yes` for serial enabled
3. Download PwnZero.py from pwnzero directory
4. Place the PwnZero.py file somewhere on the Pi in either its own folder or a folder with other plugins
5. Open ```/etc/pwnagotchi/config.toml``` file and add ```main.custom_plugins = "/path/to/custom_plugins/directory"``` and ```main.plugins.PwnZero.enabled = true```
6. Follow hardware setup shown in `doc/HardwareSetup.md` to connect the devices
7. Restart the Pwnagotchi and open the Flipagotchi app on the Flipper Zero

### Setup note
[Chrismettal](https://github.com/Chrismettal) has designed a "backpack" for the Flipper Zero which is a board that allows you to cleanly attach various devices to the Flipper. They have created one for the Raspberry Pi Zero W which would be a great way to keep your Flipagotchi tidy! Here is a link to their [project](https://github.com/Chrismettal/flipper-zero-backpacks#raspberry-pi-zero-w).

## Development stages
### Stage 1: Simple display rendering
- Stage 1 will focus on getting the Pwnagotchi display to render on the Flipper's display

### Stage 2: App interaction
- Stage 2 will allow the user to interact and control the pwnagotchi using the Flipper's interface

## Contributing
If you would like to contribute, you may make a pull request. It will be helpful if you first open an issue describing the change that you are interested in contributing.

## License
[MIT](https://choosealicense.com/licenses/mit/)

## Disclaimer
<b>This program is meant for educational purposes ONLY. I disclaim any and all responsibility for the usage of this program by external parties (not me).</b>
