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
	size_t str_len = sizeof(msg_to_ubuffer);	// this includes the null char

	pr_debug("read_op: entry\n str_len: %ld, size: %ld", str_len, size);

	if (*offset >= str_len)	return 0;

	copy_to_user(user_buffer, msg_to_ubuffer, str_len);
	*offset += str_len;

	return str_len;
}

static ssize_t write_op(struct file *f, const char __user *user_buffer, size_t size, loff_t *offset){
	size_t str_len = sizeof(user_buffer);	// this includes the null char
	char kernel_buffer[str_len];

	copy_from_user(kernel_buffer, user_buffer, str_len);
	pr_debug("write_op: kernel_buffer: %s, size: %lln\n", kernel_buffer, offset);
	pr_debug("write_op: exit\n");

	*offset = str_len;
	return str_len;
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
