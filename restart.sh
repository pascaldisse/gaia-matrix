#!/bin/bash

# GAIA MATRIX Server Restart Script
# This script rebuilds the web application and restarts the development server

echo "Restarting GAIA MATRIX development server..."

# Kill any existing server process
pkill -f "python -m http.server 8000" || pkill -f "python -m SimpleHTTPServer 8000" || pkill -f "php -S localhost:8000" || true

# Run the build script to rebuild the web application
./build.sh

# Start the server again
./dev_server.sh