/*
 * A linux driver that creates a file in /proc at initialization,
 * and allow the user to write to it and read back from it.
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>

#define MAX_BUFFER_SIZE 1024
#ifdef DEBUG
#define FUNC_START() pr_debug("%s: start\n", __func__)
#define FUNC_END() pr_debug("%s: end\n", __func__)
#define FUNC_DEBUG_INFO(...) pr_debug(__VA_ARGS__)
#else
#define FUNC_START()
#define FUNC_END()
#define FUNC_DEBUG_INFO(...)
#endif

/* some module info */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ann Yun");
MODULE_DESCRIPTION("A toy loadable kernel driver");

/* variables and functions */
static char driver_buffer[MAX_BUFFER_SIZE];
static struct proc_dir_entry *my_file_node;
struct proc_ops fn_proc_ops;

/* all the proc operations */
static ssize_t read_op(struct file *f, char __user *user_buffer, size_t size, loff_t *offset);
static ssize_t write_op(struct file *f, const char __user *user_buffer, size_t size, loff_t *offset);


static ssize_t read_op(struct file *f, char __user *user_buffer, size_t size, loff_t *offset){
	/*
	 * Send data from kernel buffer to user space.
	 * Return the number of bytes successfully read.
	 * If an error occurs, a negative number is returned.
	 * Return value of 0 means, EOF was reached.
	 * Only when 0 is returned, the application program (for example, cat) will stop reading.
	 *
	 * @f: a file pointer to the proc file?
	 * @user_buffer: user buffer to copy to
	 * @size: size of user_buffer
	 * @offset: current file position 
	 */
	FUNC_START();

	unsigned long nbytes_not_copied;
	size_t str_len = strlen(driver_buffer) - *offset;
	ssize_t nbytes_read = 0;

	FUNC_DEBUG_INFO("size: %lu, offset: %llu\n", size, *offset);

	if (*offset < strlen(driver_buffer)){
		nbytes_not_copied = copy_to_user(user_buffer + *offset, driver_buffer, str_len);
		nbytes_read = str_len - nbytes_not_copied;
		*offset += nbytes_read;
	}

	FUNC_END();
	return nbytes_read;
}

static ssize_t write_op(struct file *f, const char __user *user_buffer, size_t size, loff_t *offset){
	/*
	 * Get user input and send it to the proc file.
	 * Return the number of bytes successfully written.
	 * If the input is too long for the buffer, raise an error.
	 * 
	 * Note: when the return value != size, the application program retries.
	 * 
	 * @f: a file pointer to the proc file?
	 * @user_buffer: user buffer to copy from (in user space)
	 * @size: size of data in user_buffer
	 * @offset: current file position 
	 */ 
	FUNC_START();

	unsigned long nbytes_not_copied;
	ssize_t nbytes_written;

	FUNC_DEBUG_INFO("size: %lu, offset: %llu\n", size, *offset);

	if (size >= MAX_BUFFER_SIZE){
		pr_warn("Input longer than max buffer size is not allowed\n");
		FUNC_END();
		return -ENOMEM;
	}

	nbytes_not_copied = copy_from_user(driver_buffer+*offset, user_buffer, size);
	nbytes_written = size - nbytes_not_copied;
	*offset += nbytes_written;

	FUNC_END();
	return nbytes_written;
}

static int my_module_init(void){
	/*
	 * Create a file in /proc with rw permission for all users.
	 * Note that if 'others' don't have write permission,
	 * writing to this file fails with Permission Denied.
	 *
	 * Return 0 on success or a non-zero number on failure.
	 */
	FUNC_START();
	umode_t mode = 0666;	
	fn_proc_ops.proc_read = read_op;
	fn_proc_ops.proc_write = write_op;
	my_file_node = proc_create("my_file_node", mode, NULL, &fn_proc_ops);

	if (!my_file_node){
		return -ENOMEM;
	}

	FUNC_END();
	return 0;
}

static void my_module_exit(void){
	FUNC_START();
	proc_remove(my_file_node);
	FUNC_END();
}

module_init(my_module_init);
module_exit(my_module_exit);
