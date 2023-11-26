
#ifndef DEBUG
#define DEBUG
#endif

#ifndef MAJORNO
#define MAJORNO 0
#endif

/* Initialising device id */
#ifndef MINORNO
#define MINORNO 0
#endif

#ifndef DEVNAME
#define DEVNAME "cdd54W"
#endif

#ifndef DEVSIZE
#define DEVSIZE 3
#endif

#ifndef DATASIZE
#define DATASIZE 0
#endif

// defining base address for parallel port
#ifndef BASE_ADDR
#define BASE_ADDR 0x378
#endif

// giving name to our parallel port
#ifndef PARALLEL_NAME
#define PARALLEL_NAME "parallel_port_54w_120"
#endif

// num of consecutive address from base address need to acquire
#ifndef NUM
#define NUM 3
#endif


/* giving our module general public license and author name using
 * module_license() and module_author declared in moduleparam.h */
MODULE_LICENSE("GPL");	
MODULE_AUTHOR("SOURABH");

extern int majorno, minorno,nodev,devsize,datasize,noofReg,Regsize;
extern dev_t deviceId;	/* deriver + device id */
//extern volatile long unsigned int bit;

typedef struct Qset
{
	// simulate device architecture on RAM
	struct Qset* next;
	void** data;
}Qset;

typedef struct Dev
{
	struct Qset *first;
	struct cdev c_dev;	// kernel representation of device
	int devsize,datasize,noofReg,Regsize;
}Dev;

extern Dev *dev;

int openDev(struct inode*,struct file*);
ssize_t readDev(struct file *, char __user *, size_t, loff_t *);
int releaseDev(struct inode*,struct file*);
