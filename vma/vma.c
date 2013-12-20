#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/types.h>
#include <linux/slab.h>

#define ERR(...) \
{\
	printk("*FUNC[%s] - LINE[%d]*", __FUNCTION__,__LINE__);\
	printk(__VA_ARGS__);\
}

#define MODULE_NAME "vmadev"

char * _devname = MODULE_NAME;

struct dev_vma {
	dev_t devid;
	struct cdev *dev;
	char *name;
};

struct dev_vma *g_pdev;

ssize_t  vma_read(struct file* f,char *buff, size_t count, loff_t * off)
{
return 0;
}

ssize_t vma_write(struct file*f, const char *buff, size_t count, loff_t * off)
{
return 0;
}

int vma_open(struct inode *i, struct file*f)
{
	return 0;
}

int vma_release(struct inode*i, struct file*f)
{
	return 0;
}

int vma_mmap(struct file*f,struct vm_area_struct* pvma)
{
	printk("vma_mmap: file=0x%08x,pvma=0x%08x\n",	(int)f,(int)pvma);
	return 0;
}

struct file_operations vma_file_ops = {
	.owner= THIS_MODULE,
	.read = vma_read,
	.write = vma_write,
	.open = vma_open,
	.release= vma_release,
	.mmap = vma_mmap
};

void deinit_device(void)
{
	if(g_pdev->dev)
	{
		cdev_del(g_pdev->dev);
		g_pdev->dev = NULL;
	}
	if(g_pdev->devid)
	{
		unregister_chrdev_region(g_pdev->devid,0);
		g_pdev->devid = 0;
	}
	kfree(g_pdev);
	g_pdev = 0;
}

int init_device(void)
{
	int ret;

	g_pdev = (struct dev_vma*)kmalloc(sizeof(struct dev_vma), GFP_KERNEL);
	if(!g_pdev)
	{
		ERR("cannot create dev_vma\n");
		return -ENOMEM;
	}	

	g_pdev->name = _devname;	

	ret = alloc_chrdev_region(&g_pdev->devid,0,1,g_pdev->name);
	if(ret < 0)
	{
		ERR("Failed in alloc dev number for %s\n",g_pdev->name);
		goto EXIT;
	}
	g_pdev->dev = cdev_alloc();
	if(!g_pdev->dev)
	{
		ERR("Failed in allocate for cdev\n");
		ret = -ENOMEM;
		goto EXIT;
	}
	cdev_init(g_pdev->dev, &vma_file_ops);
	ret = cdev_add(g_pdev->dev,g_pdev->devid,1);
	if(ret < 0)
	{
		ERR("Add %s to system failed\n", g_pdev->name);
		goto EXIT;
	}

	printk("Succeed in add %s to system. Device ID is Major=%d, Minor=%d\n",
	g_pdev->name, MAJOR(g_pdev->devid), MINOR(g_pdev->devid));
	return 0;
EXIT:
	deinit_device();
	return ret;
}


int __init vma_init(void)
{
	printk("\nKernel moduel: VMA\n");
	printk("Basic idea is observing the data transfered to kernel.\n");
	printk("To understand how to map a BUS space to for user space applications.\n");
	printk("\n");

	init_device();

	return 0;
}

void __exit vma_exit(void)
{
	deinit_device();
}

module_init(vma_init);
module_exit(vma_exit);
	
MODULE_LICENSE("GPL");
MODULE_AUTHOR("David");
MODULE_DESCRIPTION("Linux Kernel Study: VMA");

