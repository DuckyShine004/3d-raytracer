#!/bin/bash

cd snapshots/

# Convert .ppm to .png format
for file in *.ppm; do
    magick "$file" "${file%.ppm}.png"
done

cd ../
