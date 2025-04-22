#!/bin/bash

# GAIA MATRIX Server Restart Script
# This script rebuilds the web application and restarts the development server

echo "Restarting GAIA MATRIX development server..."

# Find and kill any process using port 8000
lsof -ti:8000 | xargs kill -9 2>/dev/null || true

# Run the build script to rebuild the web application
./build.sh

# Start the server again
./dev_server.sh