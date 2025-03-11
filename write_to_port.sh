#!/bin/bash
# Send the message using UDP
port=12345
echo "$1" | nc -u -w 1 127.0.0.1 "$port"

