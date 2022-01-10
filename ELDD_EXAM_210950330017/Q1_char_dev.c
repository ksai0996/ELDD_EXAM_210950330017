/*Q1. Verify user read/write oeprations using IOCTL functionality*/


#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/sched.h>
#include<linux/types.h>.
#include<linux/fs.h>
#include<linux/fcntl.h>
#include<linux/cdev.h>
#include <asm/uaccess.h>
#include <linux/uaccess.h>

#define MAX_DEV 1

static int mychardev_open(struct inode *inode, struct file *file);
static int mychardev_release(struct inode *inode, struct file *file);
static long mychardev_ioctl(struct file *file, unsigned int cmd, unsigned long arg);
static ssize_t mychardev_read(struct file *file, char __user *buf, size_t count, loff_t *offset);
static ssize_t mychardev_write(struct file *file, const char __user *buf, size_t count, loff_t *offset);


static const struct file_operations mychardev_fops = {
    .owner      = THIS_MODULE,
    .open       = mychardev_open,
    .release    = mychardev_release,
    .unlocked_ioctl = mychardev_ioctl,
    .read       = mychardev_read,
    .write       = mychardev_write
};


struct mychar_device_data {
    struct cdev cdev;
};





static int dev_major = 0;

static int __init mychardev_init(void)
{
    int err;
    dev_t dev;

    err = alloc_chrdev_region(&dev, 0, MAX_DEV, "mychardev");

    dev_major = MAJOR(dev);


        cdev_init(&mychardev_data.cdev, &mychardev_fops);
        mychardev_data.cdev.owner = THIS_MODULE;

        cdev_add(&mychardev_data.cdev, MKDEV(dev_major, MAX_DEV), 1);


    return 0;
}

static void __exit mychardev_exit(void)
{
   

    unregister_chrdev_region(MKDEV(dev_major, 0), MINORMASK);
}



static int mychardev_open(struct inode *inode, struct file *file)
{
    printk("MYCHARDEV: Device open\n");
    return 0;
}

static int mychardev_release(struct inode *inode, struct file *file)
{
    printk("MYCHARDEV: Device close\n");
    return 0;
}

static long mychardev_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    printk("MYCHARDEV: Device ioctl\n");
    return 0;
}


static ssize_t mychardev_read(struct file *file, char __user *buf, size_t count, loff_t *offset)
{
    uint8_t *data = "Hello DESD HYDERABAD!\n";
    size_t datalen = strlen(data);

    printk("Reading device: %d\n", MINOR(file->f_path.dentry->d_inode->i_rdev));

    if (count > datalen) {
        count = datalen;
    }

    if (copy_to_user(buf, data, count)) {
        return -EFAULT;
    }

    return count;
}

static ssize_t mychardev_write(struct file *file, const char __user *buf, size_t count, loff_t *offset)
{
    size_t maxdatalen = 30, ncopied;
    uint8_t databuf[maxdatalen];

    printk("Writing device: %d\n", MINOR(file->f_path.dentry->d_inode->i_rdev));

    if (count < maxdatalen) {
        maxdatalen = count;
    }

    ncopied = copy_from_user(databuf, buf, maxdatalen);

    if (ncopied == 0) {
        printk("Copied %d bytes from the user\n", maxdatalen);
    } else {
        printk("Could't copy %d bytes from the user\n", ncopied);
    }

    databuf[maxdatalen] = 0;

    printk("Data from the user: %s\n", databuf);

    return count;
}




MODULE_LICENSE("GPL");
MODULE_AUTHOR("Saikumar");

module_init(mychardev_init);
module_exit(mychardev_exit);
