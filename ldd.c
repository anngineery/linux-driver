#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ann Yun");
MODULE_DESCRIPTION("A toy loadable kernel driver");

static struct proc_dir_entry *my_file_node;
static ssize_t read_op(struct file *f, char __user *user_buffer, size_t size, loff_t *offset);
static ssize_t write_op(struct file *f, const char __user *user_buffer, size_t size, loff_t *offset);
struct proc_ops fn_proc_ops = {
	.proc_read = read_op,
	.proc_write = write_op
};

static ssize_t read_op(struct file *f, char __user *user_buffer, size_t size, loff_t *offset){
    char msg_to_ubuffer[] = "my_file_node is read\n";
    size_t len = strlen(msg_to_ubuffer);

    printk("inside read_op: offset=%lld, len=%ld\n", *offset, len);

    if (*offset >= len)	return 0;

    copy_to_user(user_buffer, msg_to_ubuffer, len);
    *offset += len;

    return len;
}

static ssize_t write_op(struct file *f, const char __user *user_buffer, size_t size, loff_t *offset){
	printk("inside write_op function");
	return 0;
}

static int my_module_init(void){
    printk("my module init: entry\n");	
    my_file_node = proc_create("my_file_node", 0, NULL, &fn_proc_ops);
    printk("my module init: exit\n");	
    return 0;
}

static void my_module_exit(void){
    printk("my module exit: entry\n");	
    proc_remove(my_file_node);
    printk("my module exit: exit\n");	
}

module_init(my_module_init);
module_exit(my_module_exit);
