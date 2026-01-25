/* ESP32-CAM Edge Impulse Vegetable Detection - SIMPLE VERSION */
#include <vegaitable_detection_inferencing.h>
#include "esp_camera.h"

// AI Thinker ESP32-CAM pins
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

// Camera configuration - use GRAYSCALE for simplicity
static camera_config_t camera_config = {
    .pin_pwdn = PWDN_GPIO_NUM,
    .pin_reset = RESET_GPIO_NUM,
    .pin_xclk = XCLK_GPIO_NUM,
    .pin_sscb_sda = SIOD_GPIO_NUM,
    .pin_sscb_scl = SIOC_GPIO_NUM,
    .pin_d7 = Y9_GPIO_NUM,
    .pin_d6 = Y8_GPIO_NUM,
    .pin_d5 = Y7_GPIO_NUM,
    .pin_d4 = Y6_GPIO_NUM,
    .pin_d3 = Y5_GPIO_NUM,
    .pin_d2 = Y4_GPIO_NUM,
    .pin_d1 = Y3_GPIO_NUM,
    .pin_d0 = Y2_GPIO_NUM,
    .pin_vsync = VSYNC_GPIO_NUM,
    .pin_href = HREF_GPIO_NUM,
    .pin_pclk = PCLK_GPIO_NUM,
    .xclk_freq_hz = 10000000,
    .ledc_timer = LEDC_TIMER_0,
    .ledc_channel = LEDC_CHANNEL_0,
    .pixel_format = PIXFORMAT_GRAYSCALE,  // SIMPLER: Grayscale
    .frame_size = FRAMESIZE_96X96,        // EXACT MATCH: 96x96!
    .jpeg_quality = 0,
    .fb_count = 1,
    .fb_location = CAMERA_FB_IN_PSRAM,
    .grab_mode = CAMERA_GRAB_WHEN_EMPTY,
};

// Buffer for grayscale image
static uint8_t *image_buffer = NULL;

// Data getter function for grayscale
static int ei_camera_get_data(size_t offset, size_t length, float *out_ptr) {
    if (!image_buffer) return -1;
    
    for (size_t i = 0; i < length; i++) {
        uint8_t pixel = image_buffer[offset + i];
        // Convert grayscale to RGB format expected by Edge Impulse
        out_ptr[i] = (pixel << 16) + (pixel << 8) + pixel;
    }
    return 0;
}

void setup() {
    Serial.begin(115200);
    delay(3000);
    
    Serial.println("\n=== VEGETABLE DETECTION - SIMPLE VERSION ===");
    Serial.printf("Model: %dx%d grayscale\n", EI_CLASSIFIER_INPUT_WIDTH, EI_CLASSIFIER_INPUT_HEIGHT);
    Serial.printf("Detects: onion, potato (threshold: %.2f)\n", EI_CLASSIFIER_OBJECT_DETECTION_THRESHOLD);
    
    // Initialize camera
    Serial.println("\nInitializing camera (96x96 grayscale)...");
    esp_err_t err = esp_camera_init(&camera_config);
    if (err != ESP_OK) {
        Serial.printf("Camera init failed: 0x%x\n", err);
        Serial.println("Trying with default settings...");
        
        // Fallback to JPEG
        camera_config.pixel_format = PIXFORMAT_JPEG;
        camera_config.frame_size = FRAMESIZE_QVGA;
        camera_config.jpeg_quality = 12;
        camera_config.xclk_freq_hz = 20000000;
        
        err = esp_camera_init(&camera_config);
        if (err != ESP_OK) {
            Serial.printf("Camera still failed: 0x%x\n", err);
            while(1) {
                delay(1000);
                Serial.print(".");
            }
        }
        Serial.println("Using JPEG as fallback");
    } else {
        Serial.println("Camera initialized with 96x96 grayscale");
    }
    
    Serial.println("\nSystem ready! Point camera at vegetables...");
    Serial.println("Inference runs automatically every 3 seconds.");
    Serial.println("==============================================\n");
}

void loop() {
    static unsigned long last_inference = 0;
    
    // Run inference every 3 seconds
    if (millis() - last_inference < 3000) {
        delay(100);
        return;
    }
    
    last_inference = millis();
    Serial.printf("\n[%lu] Capturing image...\n", millis());
    
    // Capture image
    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb) {
        Serial.println("Camera capture failed");
        return;
    }
    
    Serial.printf("Image: %dx%d, format: %d, size: %d bytes\n", 
                  fb->width, fb->height, fb->format, fb->len);
    
    // Handle different formats
    if (fb->format == PIXFORMAT_GRAYSCALE) {
        // Direct use of grayscale
        image_buffer = fb->buf;
        Serial.println("Using grayscale directly");
    } 
    else if (fb->format == PIXFORMAT_JPEG) {
        // Convert JPEG to grayscale
        Serial.println("Converting JPEG to grayscale...");
        
        // Allocate buffer for RGB
        uint8_t *rgb_buffer = (uint8_t*)malloc(fb->width * fb->height * 3);
        if (!rgb_buffer) {
            Serial.println("Failed to allocate RGB buffer");
            esp_camera_fb_return(fb);
            return;
        }
        
        // Convert to RGB
        if (!fmt2rgb888(fb->buf, fb->len, PIXFORMAT_JPEG, rgb_buffer)) {
            Serial.println("JPEG to RGB conversion failed");
            free(rgb_buffer);
            esp_camera_fb_return(fb);
            return;
        }
        
        // Convert RGB to grayscale (simplified)
        image_buffer = (uint8_t*)malloc(fb->width * fb->height);
        if (!image_buffer) {
            Serial.println("Failed to allocate grayscale buffer");
            free(rgb_buffer);
            esp_camera_fb_return(fb);
            return;
        }
        
        // Simple RGB to grayscale conversion
        for (int i = 0; i < fb->width * fb->height; i++) {
            uint8_t r = rgb_buffer[i * 3];
            uint8_t g = rgb_buffer[i * 3 + 1];
            uint8_t b = rgb_buffer[i * 3 + 2];
            // Average method
            image_buffer[i] = (r + g + b) / 3;
        }
        
        free(rgb_buffer);
        Serial.println("JPEG converted to grayscale");
    }
    else {
        Serial.printf("Unsupported format: %d\n", fb->format);
        esp_camera_fb_return(fb);
        return;
    }
    
    // Return the frame buffer (but keep image_buffer if it's our allocation)
    if (fb->format == PIXFORMAT_GRAYSCALE) {
        // We're using the frame buffer directly, don't free yet
    } else {
        esp_camera_fb_return(fb);
    }
    
    // Create signal for Edge Impulse
    ei::signal_t signal;
    signal.total_length = EI_CLASSIFIER_INPUT_WIDTH * EI_CLASSIFIER_INPUT_HEIGHT;
    signal.get_data = ei_camera_get_data;
    
    // Run inference
    Serial.println("Running vegetable detection...");
    ei_impulse_result_t result = {0};
    
    EI_IMPULSE_ERROR err = run_classifier(&signal, &result, false);
    
    if (err != EI_IMPULSE_OK) {
        Serial.printf("Inference error: %d\n", err);
    } else {
        // Display results
        Serial.println("\n=== DETECTION RESULTS ===");
        Serial.printf("Timing: DSP=%dms, Classification=%dms\n", 
                      result.timing.dsp, result.timing.classification);
        
        bool detected = false;
        for (uint32_t i = 0; i < result.bounding_boxes_count; i++) {
            ei_impulse_result_bounding_box_t bb = result.bounding_boxes[i];
            
            if (bb.value > EI_CLASSIFIER_OBJECT_DETECTION_THRESHOLD) {
                detected = true;
                Serial.printf("✓ Detected: %s (%.2f confidence)\n", bb.label, bb.value);
                Serial.printf("  Position: [x:%u, y:%u, w:%u, h:%u]\n", 
                             bb.x, bb.y, bb.width, bb.height);
            }
        }
        
        if (!detected) {
            Serial.println("No vegetables detected above threshold");
            
            // Show low confidence detections for debugging
            for (uint32_t i = 0; i < result.bounding_boxes_count; i++) {
                ei_impulse_result_bounding_box_t bb = result.bounding_boxes[i];
                if (bb.value > 0.1) {
                    Serial.printf("Low confidence: %s = %.3f\n", bb.label, bb.value);
                }
            }
        }
    }
    
    // Cleanup
    if (fb->format != PIXFORMAT_GRAYSCALE && image_buffer) {
        free(image_buffer);
        image_buffer = NULL;
    }
    
    if (fb->format == PIXFORMAT_GRAYSCALE) {
        esp_camera_fb_return(fb);
    }
    
    Serial.println("===========================\n");
}