# ESP8266 DateTime Online with Screen

This project uses an ESP8266 module to display the current date and time on an OLED screen and syncs with an online time server over WiFi. It also checks WiFi connectivity and displays an error message if it fails to establish a connection.

## Description

The code runs on an ESP8266 and performs the following tasks:

1. Configures WiFi credentials and sets time zones for your location.

2. Attempts to connect to the specified WiFi network. If it cannot establish a connection after 10 seconds, it displays an error message on the screen.

3. Once connected to WiFi, it synchronizes the time from an online time server using NTP (Network Time Protocol).

4. Displays the current date and time on the OLED screen.

5. Periodically updates the time to keep it accurate.

6. Checks WiFi signal strength and shows additional information such as IP address and signal strength.

## Usage

1. Load the code onto your ESP8266 module using the Arduino development environment.

2. Make sure to set the WiFi network name (SSID) and password (password) in the `ssid` and `password` variables.

3. Connect your ESP8266 module to a compatible OLED screen, ensuring that the hardware connections are configured correctly.

4. Run the code and observe how the date and time are displayed on the OLED screen.

## Notes

- Ensure you have access to an active WiFi network with internet access so that the ESP8266 module can synchronize the time correctly.

- If you cannot connect to the specified WiFi network, double-check that the WiFi credentials are correct and that the device is within the network's range.

- The U8g2 library is used to control the OLED screen. Make sure it is installed in your Arduino development environment.

- The code checks WiFi connection and displays an error message if a connection cannot be established. This is useful for troubleshooting connectivity issues.

Enjoy your real-time clock based on ESP8266 with this application!
