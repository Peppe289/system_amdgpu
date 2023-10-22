#ifndef _RKS_DATA_STRUCT_H__
#define _RKS_DATA_STRUCT_H__

// standard list for save @p_gpu
typedef struct node_t
{
    void *data;
    struct node_t *next;
} node_t;

/**
 * Save the paths in this structure which will then in turn be saved
 * in the list @node_t.
 * 
 * @root: Contains the GPU attachment point.
 * For example card1 or card0.
 * 
 * @hwmon: Useful for understanding the "version" of hwmon.
 * For example hwmon5 or hwmon2
 */
typedef struct p_gpu {
    char *root;
    char *hwmon;
} p_gpu;

/**
 * Possible input data type. this helps to understand how
 * to manage the input data in the args reading phase.
 */
typedef enum argparse_option_type {
	ARG_PARSE_ERR = -1,
	ARG_PARSE_INT = 0,
	ARG_PARSE_STRING,
	ARG_PARSE_FLOAT,
	ARG_PARSE_INT_ARRAY,
	ARG_PARSE_END,
} argparse_option_type;

/* Not properly used, will be dropped when the code is stable. */
typedef struct argparse {
	enum argparse_option_type type;	
	// internal context
	int argc;
	char **argv;
	char *optvalue; // current option value
} argparse;

/* List of unique ids of all possible flags */
typedef enum opt_id {
	OPT_SET_PWM_MODE = 0,
	OPT_SET_FAN = 1,
} opt_id;

/**
 * All possible arguments.
 * 
 * @type: I need to save the possible data type in input
 * to understand how to manage the string.
 * 
 * @id: This is a reference to what he will actually do.
 * For this reference there is a function pointer in __int_args().
 * 
 * @long_name: The name of the flag (without '--' and '=').
 * 
 * @help: Description of what the flag will do.
 */
typedef struct argparse_option {
	enum argparse_option_type type;
	enum opt_id id;
	const char *long_name;
	const char *help;
} argparse_option;

typedef int (*fptr)(struct node_t *, const char *);

#define OPT_STRING(x, ...)	[x] =	{ARG_PARSE_STRING, x, __VA_ARGS__}
#define OPT_INT(x, ...)		[x] = {ARG_PARSE_INT, x, __VA_ARGS__}
#define OPT_INT_ARR(x, ...)	[x] = {ARG_PARSE_INT_ARRAY, x, __VA_ARGS__}
#define OPT_END()			{ARG_PARSE_END, -1, NULL, NULL}

void push(struct node_t **_node, void *data);
void *pop(struct node_t **_node);
void destroy_node(struct node_t **_node);

const char *get_root(struct node_t *_node);
const char *get_hwmon(struct node_t *_node);
void free_pgpu(struct p_gpu *data);
int pwm_init(struct node_t *_node);
int pwm_control(struct node_t *_node);

/* These functions help manage input arguments. */
int __int_args(struct argparse_option *option,
				struct node_t *amdgpu, int argc, char *argv[]);
int pwm_set(struct node_t *_node, const char *mode);

#endif // _RKS_DATA_STRUCT_H__