#!/bin/bash

python3 PopulateSongsDB.py

make clean
make Chatbot

node server/server.js