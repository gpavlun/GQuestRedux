#!/bin/sh
set -e

cargo build
cp target/debug/demo bin/demo