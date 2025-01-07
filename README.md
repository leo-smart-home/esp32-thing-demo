This repoitory would serve as a template for any custom HW "thing" in the context of Internet of Things.
Firmware functionality:
 1. Interract with external button and LED
 2. Connect to the router at home network over WiFi
 3. Connect to the MQTT Broker (MQTT over TCP probabbly, to be considered)
 4. Publish to the MQTT telemetry data to the server
 5. Handle MQTT commands from another the server


 Ubuntu 24.04

sudo apt-get remove modemmanager
sudo usermod -a -G dialout $USER
sudo reboot

