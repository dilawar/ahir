#ifndef INC_vcParserTokenTypes_hpp_
#define INC_vcParserTokenTypes_hpp_

/* $ANTLR 2.7.7 (2006-11-01): "vc.g" -> "vcParserTokenTypes.hpp"$ */

#ifndef CUSTOM_API
# define CUSTOM_API
#endif

#ifdef __cplusplus
struct CUSTOM_API vcParserTokenTypes {
#endif
	enum {
		EOF_ = 1,
		LIFO = 4,
		PIPE = 5,
		UINTEGER = 6,
		DEPTH = 7,
		MEMORYSPACE = 8,
		UNORDERED = 9,
		LBRACE = 10,
		RBRACE = 11,
		CAPACITY = 12,
		DATAWIDTH = 13,
		ADDRWIDTH = 14,
		MAXACCESSWIDTH = 15,
		OBJECT = 16,
		COLON = 17,
		FOREIGN = 18,
		MODULE = 19,
		SIMPLE_IDENTIFIER = 20,
		EQUIVALENT = 21,
		LPAREN = 22,
		RPAREN = 23,
		OPEN = 24,
		DIV_OP = 25,
		ENTRY = 26,
		EXIT = 27,
		CONTROLPATH = 28,
		PLACE = 29,
		TRANSITION = 30,
		DEAD = 31,
		SERIESBLOCK = 32,
		PARALLELBLOCK = 33,
		BRANCHBLOCK = 34,
		LOOPBLOCK = 35,
		TERMINATE = 36,
		BIND = 37,
		MERGE = 38,
		BRANCH = 39,
		FORKBLOCK = 40,
		PIPELINE = 41,
		JOIN = 42,
		MARKEDJOIN = 43,
		FORK = 44,
		DATAPATH = 45,
		GUARD = 46,
		NOT_OP = 47,
		PLUS_OP = 48,
		MINUS_OP = 49,
		MUL_OP = 50,
		SHL_OP = 51,
		SHR_OP = 52,
		UGT_OP = 53,
		UGE_OP = 54,
		EQ_OP = 55,
		ULT_OP = 56,
		ULE_OP = 57,
		NEQ_OP = 58,
		BITSEL_OP = 59,
		CONCAT_OP = 60,
		OR_OP = 61,
		AND_OP = 62,
		XOR_OP = 63,
		NOR_OP = 64,
		NAND_OP = 65,
		XNOR_OP = 66,
		SHRA_OP = 67,
		SGT_OP = 68,
		SGE_OP = 69,
		SLT_OP = 70,
		SLE_OP = 71,
		StoS_ASSIGN_OP = 72,
		StoU_ASSIGN_OP = 73,
		UtoS_ASSIGN_OP = 74,
		FtoS_ASSIGN_OP = 75,
		FtoU_ASSIGN_OP = 76,
		StoF_ASSIGN_OP = 77,
		UtoF_ASSIGN_OP = 78,
		FtoF_ASSIGN_OP = 79,
		BRANCH_OP = 80,
		SELECT_OP = 81,
		SLICE_OP = 82,
		ASSIGN_OP = 83,
		EQUIVALENCE_OP = 84,
		CALL = 85,
		INLINE = 86,
		IOPORT = 87,
		IN = 88,
		OUT = 89,
		LOAD = 90,
		FROM = 91,
		STORE = 92,
		TO = 93,
		PHI = 94,
		LBRACKET = 95,
		RBRACKET = 96,
		CONSTANT = 97,
		INTERMEDIATE = 98,
		WIRE = 99,
		BINARYSTRING = 100,
		HEXSTRING = 101,
		COMMA = 102,
		INT = 103,
		FLOAT = 104,
		POINTER = 105,
		ARRAY = 106,
		OF = 107,
		RECORD = 108,
		ATTRIBUTE = 109,
		IMPLIES = 110,
		QUOTED_STRING = 111,
		DPE = 112,
		LIBRARY = 113,
		REQS = 114,
		ACKS = 115,
		HIDDEN = 116,
		PARAMETER = 117,
		PORT = 118,
		MAP = 119,
		MIN = 120,
		MAX = 121,
		DPEINSTANCE = 122,
		LINK = 123,
		AT = 124,
		ORDERED_OP = 125,
		UNORDERED_OP = 126,
		WHITESPACE = 127,
		SINGLELINECOMMENT = 128,
		HIERARCHICAL_IDENTIFIER = 129,
		ALPHA = 130,
		DIGIT = 131,
		NULL_TREE_LOOKAHEAD = 3
	};
#ifdef __cplusplus
};
#endif
#endif /*INC_vcParserTokenTypes_hpp_*/
