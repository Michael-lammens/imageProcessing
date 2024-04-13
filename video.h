#include <stdio.h>
#include <stdlib.h>

#define IMAGE_WIDTH  640   
#define IMAGE_HEIGHT 480

// Function prototypes
unsigned char* readRawImage(const char* filename);
double calculateAverageIntensity(const unsigned char* image_data);
void freeImageData(unsigned char* image_data);

unsigned char* readRawImage(const char* filename) {
    // Open the raw image file
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }

    // Allocate memory for pixel data
    unsigned char *image_data = (unsigned char *)malloc(IMAGE_WIDTH * IMAGE_HEIGHT);
    if (image_data == NULL) {
        perror("Error allocating memory");
        fclose(file);
        return NULL;
    }

    // Read raw image data from file
    size_t bytes_read = fread(image_data, 1, IMAGE_WIDTH * IMAGE_HEIGHT, file);
    if (bytes_read != IMAGE_WIDTH * IMAGE_HEIGHT) {
        perror("Error reading file");
        free(image_data);
        fclose(file);
        return NULL;
    }

    // Close file
    fclose(file);

    return image_data;
}


// Function to calculate average pixel intensity
double calculateAverageIntensity(const unsigned char* image_data) {
    double total_intensity = 0.0;
    for (int i = 0; i < IMAGE_WIDTH * IMAGE_HEIGHT; i++) {
        total_intensity += image_data[i];
    }
    return total_intensity / (IMAGE_WIDTH * IMAGE_HEIGHT);
}

// Function to free allocated memory for image data
void freeImageData(unsigned char* image_data) {
    free(image_data);
}



/*
int main() {
    // Read raw image data from file
    unsigned char* image_data = readRawImage("output_image.raw");
    if (image_data == NULL) {
        return 1;
    }

    // Calculate average pixel intensity
    double average_intensity = calculateAverageIntensity(image_data);

    // Output result
    printf("Average pixel intensity: %f\n", average_intensity);

    // Free memory
    freeImageData(image_data);

    return 0;
}
*/