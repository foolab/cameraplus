#!/bin/sh

set -e

echo "Restarting theme daemon."
killall -HUP mthemedaemon
