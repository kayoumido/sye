echo Deploying user application into the root filesystem ... 
cd ..
cd filesystem
./mount.sh
sudo cp ../usr/*.elf fs
sudo cp ../usr/*.txt fs
./umount.sh
cd ../usr
