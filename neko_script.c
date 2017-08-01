#include "neko_script.h"

#define get_argument() runtime->arguments[runtime->program_counter]

//=======================================
// Implement ScriptRuntime functions.
//=======================================

void runtime_step(ScriptRuntime *runtime) {
	runtime->instructions[runtime->program_counter](runtime);
	++runtime->program_counter;
}


size_t runtime_pop(ScriptRuntime *runtime) {
	++runtime->stack_pointer;
	size_t val = runtime->stack_top;
	runtime->stack_top = *runtime->stack_pointer;
	return val;
}


void runtime_push(ScriptRuntime *runtime, size_t val) {
	*runtime->stack_pointer = runtime->stack_top;
	--runtime->stack_pointer;
	runtime->stack_top = val;
}

bool runtime_load_script(ScriptRuntime *runtime) {

	// Loop as many times as possible.
	while (true) {
		char c = getchar();

		// Check if no script was loaded. //
		if (c == EOF) return false;
	
		// Read name of the instruction. //
		char instruction_name_buffer [85];
		int instruction_name_index = 0;
		while (c != ' ' && c != '\n' && c != EOF) { // TODO: Add array out-of-bounds check!
			instruction_name_buffer[instruction_name_index] = c;
			c = getchar();
			instruction_name_index++;
		}
		instruction_name_buffer[instruction_name_index] = 0;
	
		// Convert instruction name to instruction. //
		Instruction instruction = string_to_instruction(instruction_name_buffer);
	
		// Skip spaces. //
		if (c == ' ') {
			c = getchar();
		}
	
		// Read argument of the instruction. //
		char instruction_argument_buffer [85];
		int instruction_argument_index = 0;
		while (c != '\n' && c != EOF) { // TODO: Add array out-of-bounds check!
			instruction_argument_buffer[instruction_argument_index] = c;
			c = getchar();
			instruction_argument_index++;
		}
		instruction_argument_buffer[instruction_argument_index] = 0;
		
		long argument = 0;
		
		if (instruction_argument_index != 0) {
			const char f = instruction_argument_buffer[0];
			if (f >= '0' && f <= '9') {
				argument = atol(instruction_argument_buffer);
			}
		}
		
		runtime_insert_instruction(runtime, instruction, argument);
		++runtime->program_counter;
	}
}

void runtime_insert_instruction (ScriptRuntime *runtime,
								 Instruction instruction,
								 size_t argument) {
	runtime->instructions[runtime->program_counter] = instruction;
	runtime->arguments[runtime->program_counter] = argument;
}

//=======================================
// Instruction implementations.
//=======================================

Instruction string_to_instruction(const char *str) {
	if (strlen(str) == 0 || str[0] == '#') {
		return instruction(noop);
	}
	
	#define try_conv_return(inst) if (strcmp(#inst, str) == 0)\
										return script_instruction_ ## inst
	try_conv_return(add) ;
	try_conv_return(sub) ;
	try_conv_return(push);
	try_conv_return(jump);
	try_conv_return(stop);
	try_conv_return(cond);
	try_conv_return(noop);
	#undef try_conv_return
	
	puts ("ERROR: Unknown instruction: ");
	puts (str);
	
	// TODO: Return error instead!
	return instruction(noop);
}

define_instruction(add) {
	size_t a = runtime->stack_top;
	size_t b = *++runtime->stack_pointer;
	runtime->stack_top =  a + b;	
}


define_instruction(sub) {
	puts("Trying to sub!\n");
}


define_instruction(push) {
	runtime_push(runtime, get_argument());
}


define_instruction(jump) {
	runtime->program_counter = get_argument();
}


define_instruction(stop) {
	runtime->stopped = true;	
}


define_instruction(cond) {
	if (runtime_pop(runtime)) {
		runtime->program_counter = get_argument();
	}
}

define_instruction(noop){}
