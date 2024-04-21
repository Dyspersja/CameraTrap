#include "AsyncJpegStreamResponse.h"
#include "WebHandlers.h"
#include "camera_pins.h"

#include "Arduino.h"
#include "esp_camera.h"
#include "ESPAsyncWebServer.h"
#include "WiFi.h"
#include <SPIFFS.h>
#include "FS.h"
#include "SD_MMC.h"

#define PIR_PIN 13

int photoDone = 0;

const char* ssid = "Test";
const char* password = "pass1234";

AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);

  pinMode(PIR_PIN, INPUT);

  connectToWiFi();
  initializeSDCard();
  if (!setupCamera()) return;

  server.on("/stream", HTTP_GET, handleStreamRequest);
  server.on("/view", HTTP_GET, handlePhotoRequest);
  server.begin();
}

void loop() {
  int pirState = digitalRead(PIR_PIN);

  if (pirState == LOW && photoDone == 0) {
    photoDone = 1;
    captureAndSavePhoto();
    delay(2000);
  } else {
    photoDone = 0;
  }

  delay(100);
}

void connectToWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.println("IP address: " + WiFi.localIP().toString());
}

void initializeSDCard() {
  if (!SD_MMC.begin()) {
    Serial.println("Card Mount Failed");
    return;
  }
  Serial.println("SD Card Mounted");
}

bool setupCamera() {
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = CAM_PIN_D0;
  config.pin_d1 = CAM_PIN_D1;
  config.pin_d2 = CAM_PIN_D2;
  config.pin_d3 = CAM_PIN_D3;
  config.pin_d4 = CAM_PIN_D4;
  config.pin_d5 = CAM_PIN_D5;
  config.pin_d6 = CAM_PIN_D6;
  config.pin_d7 = CAM_PIN_D7;
  config.pin_xclk = CAM_PIN_XCLK;
  config.pin_pclk = CAM_PIN_PCLK;
  config.pin_vsync = CAM_PIN_VSYNC;
  config.pin_href = CAM_PIN_HREF;
  config.pin_sscb_sda = CAM_PIN_SIOD;
  config.pin_sscb_scl = CAM_PIN_SIOC;
  config.pin_pwdn = CAM_PIN_PWDN;
  config.pin_reset = CAM_PIN_RESET;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  if (psramFound()) {
    config.frame_size = FRAMESIZE_QQVGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return false;
  }
  return true;
}

void captureAndSavePhoto() {
  camera_fb_t *fb = NULL;

  // Take a photo
  fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    return;
  }

  // Generate a file name
  String fileName = "/photo" + String(millis()) + ".jpg";

  // Open the file
  File file = SD_MMC.open(fileName, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    esp_camera_fb_return(fb);
    return;
  }

  // Write the photo data to the file
  file.write(fb->buf, fb->len);
  Serial.println("Photo saved: " + fileName);

  // Close the file and return the frame buffer
  file.close();
  esp_camera_fb_return(fb);
}