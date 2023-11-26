// mapping for system call possible on device are mapped here with userdefined routines
struct file_operations fops=
{
	open: 	openDev,
	write: writeDev,
	release: releaseDev
};
