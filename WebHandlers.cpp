#include "WebHandlers.h"

String getPhotoList() {
  String photoList;

  File root = SD_MMC.open("/");
  File file = root.openNextFile();

  while (file) {
    if (file.isDirectory()) {
      // Skip directories
    } else {
      photoList += "<li><a href='/view?photo=" + String(file.name()) + "'>" + String(file.name()) + "</a></li>";
    }
    file = root.openNextFile();
  }

  return photoList;
}

void handlePhotoRequest(AsyncWebServerRequest *request) {
    if (request->hasParam("photo")) {
        String photoName = request->getParam("photo")->value();

        if (!photoName.isEmpty()) {
            String photoPath = "/" + photoName;

            if (SD_MMC.exists(photoPath)) {
                request->send(SD_MMC, photoPath, "image/jpeg", false);
            } else {
                request->send(404, "text/plain", "File not found");
            }
        } else {
            request->send(400, "text/plain", "Empty 'photo' parameter");
        }
    } else {
        String photoList = getPhotoList();

        String html = "<html><body><h1>Photo List</h1><ul>" + photoList + "</ul></body></html>";

        request->send(200, "text/html", html);
    }
}

void handleStreamRequest(AsyncWebServerRequest *request) {
    AsyncJpegStreamResponse *response = new AsyncJpegStreamResponse();
    if (!response) {
        request->send(501);
        return;
    }
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
}