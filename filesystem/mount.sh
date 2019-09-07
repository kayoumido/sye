DEVLOOP=$(sudo losetup --partscan --find --show flash)
FS_IMG=flash
SCRIPT_IMG=./create_img.sh

if [ ! -f $FS_IMG ]; then
	echo "Creating image"sh  $FS_IMG
	bash $SCRIPT_IMG $FS_IMG

	if [ $? -ne 0 ]; then
		echo "Error while creating image"
		exit 1
	fi
	sleep 1
fi

#sudo losetup -P --find --show flash
mkdir -p fs


sudo mount ${DEVLOOP}p1 fs -o umask=002,uid=$USER,gid=$USER

