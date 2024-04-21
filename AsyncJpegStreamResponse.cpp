#include "AsyncJpegStreamResponse.h"
#include "esp_camera.h"
#include "camera_pins.h"

#define PART_BOUNDARY "123456789000000000000987654321"
static const char* STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=" PART_BOUNDARY;
static const char* STREAM_BOUNDARY = "\r\n--" PART_BOUNDARY "\r\n";
static const char* STREAM_PART = "Content-Type: %s\r\nContent-Length: %u\r\n\r\n";

static const char * JPG_CONTENT_TYPE = "image/jpeg";

AsyncJpegStreamResponse::AsyncJpegStreamResponse() {
        _callback = nullptr;
        _code = 200;
        _contentLength = 0;
        _contentType = STREAM_CONTENT_TYPE;
        _sendContentLength = false;
        _chunked = true;
        _index = 0;
        _jpg_buf_len = 0;
        _jpg_buf = NULL;
        memset(&_frame, 0, sizeof(camera_frame_t));
}

AsyncJpegStreamResponse::~AsyncJpegStreamResponse() {
        if (_frame.fb) {
            if (_frame.fb->format != PIXFORMAT_JPEG) {
                free(_jpg_buf);
            }
            esp_camera_fb_return(_frame.fb);
        }
}

bool AsyncJpegStreamResponse::_sourceValid() const {
    return true;
}

size_t AsyncJpegStreamResponse::_fillBuffer(uint8_t *buf, size_t maxLen) {
        size_t ret = _content(buf, maxLen, _index);
        if (ret != RESPONSE_TRY_AGAIN) {
            _index += ret;
        }
        return ret;
}

size_t AsyncJpegStreamResponse::_content(uint8_t *buffer, size_t maxLen, size_t index) {
        if (!_frame.fb || _frame.index == _jpg_buf_len) {
            if (index && _frame.fb) {
                if (_frame.fb->format != PIXFORMAT_JPEG) {
                    free(_jpg_buf);
                }
                esp_camera_fb_return(_frame.fb);
                _frame.fb = NULL;
                _jpg_buf_len = 0;
                _jpg_buf = NULL;
            }
            if (maxLen < (strlen(STREAM_BOUNDARY) + strlen(STREAM_PART) + strlen(JPG_CONTENT_TYPE) + 8)) {
                return RESPONSE_TRY_AGAIN;
            }

            _frame.index = 0;
            _frame.fb = esp_camera_fb_get();

            if (_frame.fb == NULL) {
                return 0;
            }

            if (_frame.fb->format != PIXFORMAT_JPEG) {
                unsigned long st = millis();
                bool jpeg_converted = frame2jpg(_frame.fb, 80, &_jpg_buf, &_jpg_buf_len);
                if (!jpeg_converted) {
                    esp_camera_fb_return(_frame.fb);
                    _frame.fb = NULL;
                    _jpg_buf_len = 0;
                    _jpg_buf = NULL;
                    return 0;
                }
            } else {
                _jpg_buf_len = _frame.fb->len;
                _jpg_buf = _frame.fb->buf;
            }

            size_t blen = 0;
            if (index) {
                blen = strlen(STREAM_BOUNDARY);
                memcpy(buffer, STREAM_BOUNDARY, blen);
                buffer += blen;
            }

            size_t hlen = sprintf((char *)buffer, STREAM_PART, JPG_CONTENT_TYPE, _jpg_buf_len);
            buffer += hlen;

            hlen = maxLen - hlen - blen;
            if (hlen > _jpg_buf_len) {
                maxLen -= hlen - _jpg_buf_len;
                hlen = _jpg_buf_len;
            }

            memcpy(buffer, _jpg_buf, hlen);
            _frame.index += hlen;
            return maxLen;
        }

        size_t available = _jpg_buf_len - _frame.index;
        if (maxLen > available) {
            maxLen = available;
        }

        memcpy(buffer, _jpg_buf + _frame.index, maxLen);
        _frame.index += maxLen;

        return maxLen;
    }