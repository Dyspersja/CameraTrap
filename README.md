# CameraTrap
ESP32-based camera trap with PIR sensor.

The project aimed to create a cameratrap system. It uses a PIR motion sensor along with the ESP32-CAM microcontroller equipped with an OV2640 camera and a microSD card reader. The camera captures images in response to the PIR motion sensor signal and saves them on the microSD card. Additionally, the ESP32 enables remote access to the system via WiFi, allowing for viewing of captured photos and live view from the camera through a web browser.

The functionalities of the project include:

1. **Motion Detection:** The device can detect motion using a PIR sensor.
2. **Instant Photo Capture:** Upon detecting motion, the camera immediately captures a photo.
3. **Storage:** Captured photos are saved onto a microSD card for storage.
4. **Remote Photo Browsing:** User can remotely browse through a list of all photos taken by the camera.
5. **Photo Preview:** The system enables user to select a specific photo from the list and view its preview in full resolution.
6. **Real-time Camera Stream:** Users can remotely access and stream live camera footage.

## Special thanks
Special thanks to the [me-no-dev](https://github.com/me-no-dev).  
Thanks to his code from one of his GitHub gists, I was able to add functionality to the application for streaming live view from the camera.

The gist I mentioned:
https://gist.github.com/me-no-dev/d34fba51a8f059ac559bf62002e61aa3

Another thanks to me-no-dev for the library ESPAsyncWebServer which I used to run the server on my ESP32.

GitHub Repository of the library: 
https://github.com/me-no-dev/ESPAsyncWebServer
