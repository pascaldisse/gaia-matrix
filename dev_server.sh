#!/bin/bash

# GAIA MATRIX Development Server
# This script starts a simple HTTP server for development

echo "Starting GAIA MATRIX development server..."

# Check if Python 3 is available
if command -v python3 &> /dev/null; then
    echo "Using Python 3 HTTP server"
    cd web_build
    python3 -m http.server 8000
# Check if Python 2 is available
elif command -v python &> /dev/null; then
    echo "Using Python 2 HTTP server"
    cd web_build
    python -m SimpleHTTPServer 8000
# Check if PHP is available
elif command -v php &> /dev/null; then
    echo "Using PHP built-in server"
    cd web_build
    php -S localhost:8000
else
    echo "Error: Cannot find Python or PHP to start a development server"
    exit 1
fi

echo "Server stopped"