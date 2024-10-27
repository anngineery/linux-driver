/*
 * A linux driver that creates a file in /proc at initialization,
 * and allow the user to write to it and read back from it.
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>

#define MAX_BUFFER_SIZE 1024

/* some module info */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ann Yun");
MODULE_DESCRIPTION("A toy loadable kernel driver");

/* variables and functions */
static char driver_buffer[MAX_BUFFER_SIZE];
static struct proc_dir_entry *my_file_node;
static ssize_t read_op(struct file *f, char __user *user_buffer, size_t size, loff_t *offset);
static ssize_t write_op(struct file *f, const char __user *user_buffer, size_t size, loff_t *offset);
struct proc_ops fn_proc_ops = {
	.proc_read = read_op,
	.proc_write = write_op
};

static ssize_t read_op(struct file *f, char __user *user_buffer, size_t size, loff_t *offset){
	size_t str_len = sizeof(driver_buffer);	// this includes the null char
	unsigned long nbytes_not_copied;

	pr_debug("read_op: entry\n");
	pr_debug("read_op: str_len: %lu, size: %lu, offset: %llu\n", str_len, size, *offset);

	nbytes_not_copied = copy_to_user(user_buffer, driver_buffer, str_len);
	if (*offset >= str_len || nbytes_not_copied != 0)	return 0;
	*offset += str_len;

	pr_debug("read_op: exit\n");

	return str_len;
}

static ssize_t write_op(struct file *f, const char __user *user_buffer, size_t size, loff_t *offset){
	/*
	 * Get a user input and write it into the buffer.
	 * If the input is too long for the buffer, raise an error.
	 */ 
	unsigned long nbytes_not_copied;

	pr_debug("write_op: entry\n");
	pr_debug("write_op: size: %lu, offset: %llu\n", size, *offset);

	// size includes the null char at the end of user_buffer's string
	if (size > MAX_BUFFER_SIZE){
		pr_warn("Input longer than max buffer size is not allowed");
		return -ENOMEM;
	}

	nbytes_not_copied = copy_from_user(driver_buffer, user_buffer, size);
	if (*offset >= size || nbytes_not_copied != 0)	return 0;
	*offset += size;

	pr_debug("write_op: exit\n");

	return size;
}

static int my_module_init(void){
	/*
	 * Create a file in /proc with rw permission for all users.
	 * Note that if 'others' don't have write permission,
	 * writing to this file fails with Permission Denied.
	 *
	 * Return 0 on success or a non-zero number on failure.
	 */
	pr_debug("my module init: entry\n");	
	umode_t mode = 0666;	
	my_file_node = proc_create("my_file_node", mode, NULL, &fn_proc_ops);

	if (!my_file_node){
		return -ENOMEM;
	}

	pr_debug("my module init: exit\n");	
	return 0;
}

static void my_module_exit(void){
	pr_debug("my module exit: entry\n");	
	proc_remove(my_file_node);
	pr_debug("my module exit: exit\n");	
}

module_init(my_module_init);
module_exit(my_module_exit);
