#!/bin/bash

function imagemagick_from_source() {
    if [ ! -f "other/in/convert" ]; then
        wget https://imagemagick.org/archive/binaries/magick
        chmod +x magick
        mv magick other/in/convert
    fi

    # That's for libraries, fuck
    #git clone https://github.com/ImageMagick/ImageMagick.git ImageMagick
    #cd ImageMagick
    #./configure --with-modules
    #make -j$(nproc)
    #sudo make install
    #sudo ldconfig /usr/local/lib
    #cd ..
}

imagemagick_from_source

if command -v dpkg &> /dev/null; then
    echo "Detected a debian based system"
    debian_packages=("platformio" "xxd" "jq" "python3-unidecode" "libfreetype-dev" "libfreetype6" "libfreetype6-dev" "python3-venv" "imagemagick" "libltdl-dev" "make" "build-essential" "automake")
    missing_packages=()

    for pkg in "${debian_packages[@]}"; do
        if ! dpkg -s "$pkg" &> /dev/null; then
            missing_packages+=("$pkg")
        fi
    done

    if [ ${#missing_packages[@]} -gt 0 ]; then
        echo "Installing missing packages: ${missing_packages[*]}"
        timeout 1m sudo apt-get install -y "${missing_packages[@]}"
        if [ $? -ne 0 ]; then
            echo "Failed to install some packages. Exiting."
            exit 1
        fi
    else
        echo "All required packages are already installed."
    fi
    exit 0
else
    echo "I only managed to support debian based system, feel free to add other distros :)"
    echo "Oh yea good luck :D"
fi

if [ ! -f "fs/in/mklittlefs" ]; then
    echo "Downloading mklittlefs"
    wget -q -O mklittlefs.tar.gz https://github.com/earlephilhower/mklittlefs/releases/download/3.2.0/x86_64-linux-gnu-mklittlefs-975bd0f.tar.gz
    tar -xf mklittlefs.tar.gz
    chmod +x mklittlefs/mklittlefs
    mv mklittlefs/mklittlefs fs/in/
    rm -rf mklittlefs
    rm -rf mklittlefs.tar.gz
fi
