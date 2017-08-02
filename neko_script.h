/************************************************
 *       The Neko-Script implementation         *
 *       By: Tyrerexus                          *
 *       Date: 31 July 2017                     *
 ************************************************/

#include "lib.h"

//=======================================
// Define the Neko-Script environment.
//=======================================

/** The ScriptRuntime, defines an environment which runs NekoScript code. */
typedef struct t_ScriptRuntime ScriptRuntime;
typedef void (*Instruction)(ScriptRuntime *runtime);
struct t_ScriptRuntime{

	/** A pointer to all instructions. */
	Instruction *instructions;

	/** A pointer to all instructions' arguments. */
	size_t *arguments;

	/** The begining of the stack. */
	size_t *stack;

	/** The current stack pointer. */
	size_t *stack_pointer;
	
	/** Top stack value. */
	size_t stack_top;

	/** What instruction to execute next. */
	size_t program_counter;

	/** If this runtime has been stopped. */
	bool stopped;	
};

/** Executes one NekoScript instruction. */
void runtime_step(ScriptRuntime *runtime);

/** Pops a value from the stack of a runtime. */
size_t runtime_pop(ScriptRuntime *runtime);

/** Pushes a value to the stack of a runtime. */
void runtime_push(ScriptRuntime *runtime, size_t val);

/** Loads a script! */
bool runtime_load_script(ScriptRuntime *runtime);

/** Runtime insert instruction and argument. */
void runtime_insert_instruction (ScriptRuntime *runtime,
								 Instruction instruction,
								 size_t argument);

//=======================================
// Define the instructions for Neko-Script.
//=======================================

/** Takes in a string and finds a instruction with that name. */
Instruction string_to_instruction(const char *str);

/// These macros can be used for defining new instructions. 
#define instruction(INST_NAME) script_instruction_ ## INST_NAME
#define define_instruction(INST_NAME) void instruction(INST_NAME) (__attribute__((unused)) ScriptRuntime *runtime) 

/// Define the headers.
define_instruction(add);
define_instruction(sub);
define_instruction(jump);
define_instruction(push);
define_instruction(stop);
define_instruction(cond);
define_instruction(noop);