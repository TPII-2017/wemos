#!/bin/bash

./merge config.html
./merge authentication.html
./merge admin.html

cd outputs/

gzip config.html
gzip authentication.html
gzip admin.html

cp -rf config.html.gz ../../wemos/data/config.html.gz
cp -rf authentication.html.gz ../../wemos/data/authentication.html.gz
cp -rf admin.html.gz ../../wemos/data/admin.html.gz
