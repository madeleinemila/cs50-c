#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "helpers.h"

#define CHANNELS 3

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int row = 0; row < height; row++)
    {
        for (int px = 0; px < width; px++)
        {
            RGBTRIPLE *pixel = &image[row][px];
            float average = (pixel->rgbtBlue + pixel->rgbtGreen + pixel->rgbtRed) / 3.0;
            pixel->rgbtBlue = round(average);
            pixel->rgbtGreen = round(average);
            pixel->rgbtRed = round(average);
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int row = 0; row < height; row++)
    {
        int left = 0;
        int right = width - 1;

        while (left < right)
        {
            RGBTRIPLE tmp = image[row][left];
            image[row][left] = image[row][right];
            image[row][right] = tmp;

            left++;
            right--;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Make a buffer to hold the new blurred values
    RGBTRIPLE blurred[height][width];

    // For each pixel
    for (int row = 0; row < height; row++)
    {
        for (int px = 0; px < width; px++)
        {
            // Init vars to tally r, g, b values for averaging
            int total_red = 0;
            int total_green = 0;
            int total_blue = 0;

            // Init var to tally number of pixels relevant to the box blur
            // (e.g. a corner pixel will only have 4, a middle pixel will have 9)
            int num_pixels = 0;

            // For each pixel in a 3 x 3 kernel
            for (int i = -1; i <= 1; i++)
            {
                for (int j = -1; j <= 1; j++)
                {
                    // If it's within bounds
                    if (row + i >= 0 && row + i < height && px + j >= 0 && px + j < width)
                    {
                        // Tally values
                        total_red += image[row + i][px + j].rgbtRed;
                        total_green += image[row + i][px + j].rgbtGreen;
                        total_blue += image[row + i][px + j].rgbtBlue;
                        num_pixels++;
                    }
                }
            }

            // Assign averaged values to blurred image buffer
            blurred[row][px].rgbtRed = round(total_red / (float) num_pixels);
            blurred[row][px].rgbtGreen = round(total_green / (float) num_pixels);
            blurred[row][px].rgbtBlue = round(total_blue / (float) num_pixels);
        }
    }

    // Update original image with blurred version
    for (int row = 0; row < height; row++)
    {
        for (int px = 0; px < width; px++)
        {
            image[row][px] = blurred[row][px];
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Make a buffer to hold the new edge detect values
    RGBTRIPLE edge_detect[height][width];

    // Init constants for max value for 8-bit color and indices for RGB
    const int MAX = 255;
    const int R_INDEX = 0;
    const int G_INDEX = 1;
    const int B_INDEX = 2;

    // Make a black pixel for any edges
    RGBTRIPLE edge;
    edge.rgbtRed = 0;
    edge.rgbtGreen = 0;
    edge.rgbtBlue = 0;

    // Make a buffer for current kernel
    const int KERNEL_SIZE = 9; // 3 x 3 kernel
    RGBTRIPLE kernel[KERNEL_SIZE];

    // Set gx and gy weights
    int gx_weights[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    int gy_weights[] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};

    // For each pixel
    for (int row = 0; row < height; row++)
    {
        for (int px = 0; px < width; px++)
        {
            // Get values of pixels in 3x3 kernel, adding in black edges
            // For each pixel in 3 x 3 kernel:
            int cell_num = 0;
            for (int i = -1; i <= 1; i++)
            {
                for (int j = -1; j <= 1; j++)
                {
                    // If it's within bounds:
                    if (row + i >= 0 && row + i < height && px + j >= 0 && px + j < width)
                    {
                        // Assign pixel value:
                        kernel[cell_num] = image[row + i][px + j];
                    }
                    else
                    {
                        // Otherwise it's an edge:
                        kernel[cell_num] = edge;
                    }
                    cell_num++;
                }
            }

            // Make arrays to store the gx and gy values for each channel
            int gx[CHANNELS] = {0, 0, 0};
            int gy[CHANNELS] = {0, 0, 0};

            // Multiply kernel by weights
            for (int i = 0; i < KERNEL_SIZE; i++)
            {
                gx[R_INDEX] += kernel[i].rgbtRed * gx_weights[i];
                gx[G_INDEX] += kernel[i].rgbtGreen * gx_weights[i];
                gx[B_INDEX] += kernel[i].rgbtBlue * gx_weights[i];
                gy[R_INDEX] += kernel[i].rgbtRed * gy_weights[i];
                gy[G_INDEX] += kernel[i].rgbtGreen * gy_weights[i];
                gy[B_INDEX] += kernel[i].rgbtBlue * gy_weights[i];
            }

            // For each channel R, G, B
            for (int i = 0; i < CHANNELS; i++)
            {
                // Calculate the value as per Sobel
                int sobel_raw = round(sqrt(pow(gx[i], 2) + pow(gy[i], 2)));

                // Clip the value if it's gone beyond the max
                int sobel_clipped = sobel_raw < MAX ? sobel_raw : MAX;

                // Assign to the edge detect buffer
                if (i == R_INDEX)
                {
                    edge_detect[row][px].rgbtRed = sobel_clipped;
                }
                if (i == G_INDEX)
                {
                    edge_detect[row][px].rgbtGreen = sobel_clipped;
                }
                if (i == B_INDEX)
                {
                    edge_detect[row][px].rgbtBlue = sobel_clipped;
                }
            }
        }
    }

    // Update original image with blurred version
    for (int row = 0; row < height; row++)
    {
        for (int px = 0; px < width; px++)
        {
            image[row][px] = edge_detect[row][px];
        }
    }

    return;
}
