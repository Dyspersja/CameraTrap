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

The picture below illustrates the real system setup on a breadboard.

![system](https://github.com/Dyspersja/CameraTrap/assets/146620220/ea7a1eb2-3133-4f78-b81c-57b5ea0341fa)

## Live View
Below is a demonstration of live video streaming and viewing it in a web browser at **/stream**.

![stream](https://github.com/Dyspersja/CameraTrap/assets/146620220/cf7a3a91-a1b2-4a3f-a9de-00f4efcd2bb1)

## List of Photos
A demonstration of the list of photos taken by the camera and saved on the microSD card at **/view**.

![photolist](https://github.com/Dyspersja/CameraTrap/assets/146620220/d5c540e5-979a-4881-98e3-0b6c0e868fe0)

## Motion Detection
When motion is detected by the PIR sensor, a photo is automatically taken.

![PIR](https://github.com/Dyspersja/CameraTrap/assets/146620220/ab5ae56d-8a8a-45d0-885a-23ee73589ce6)

## Special thanks
Special thanks to the [me-no-dev](https://github.com/me-no-dev).  
Thanks to his code from one of his GitHub gists, I was able to add functionality to the application for streaming live view from the camera.

The gist I mentioned:
https://gist.github.com/me-no-dev/d34fba51a8f059ac559bf62002e61aa3

Another thanks to me-no-dev for the library ESPAsyncWebServer which I used to run the server on my ESP32.

GitHub Repository of the library: 
https://github.com/me-no-dev/ESPAsyncWebServer
