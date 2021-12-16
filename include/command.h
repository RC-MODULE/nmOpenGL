#pragma once

#define COMMAND_PARAM_SIZE 7

enum StoredTypes {
	TYPE_NULL,
	TYPE_POINTER,
	TYPE_INT,
	TYPE_UNSIGNED_INT,
	TYPE_FLOAT,
	TYPE_BOOL
};

struct CommandArgument {
	union {
		void* p;
		int i;
		unsigned int ui;	
		float f;
		bool b; 
	};
	StoredTypes storedType = TYPE_NULL;

	CommandArgument() {
		storedType = TYPE_NULL;
	}
	CommandArgument(void* value) {
		p = value;
		storedType = TYPE_POINTER;
	}
	CommandArgument(unsigned int value) {
		ui = value;
		storedType = TYPE_UNSIGNED_INT;
	}
	CommandArgument(int value) {
		i = value;
		storedType = TYPE_INT;
	}
	CommandArgument(float value) {
		f = value;
		storedType = TYPE_FLOAT;
	}
	CommandArgument(bool value) {
		b = value;
		storedType = TYPE_BOOL;
	}
};

struct NM_Command{
	int instr;
	CommandArgument params[COMMAND_PARAM_SIZE];
};

