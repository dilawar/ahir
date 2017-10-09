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
		NOBLOCK = 5,
		SHIFTREG = 6,
		PIPE = 7,
		UINTEGER = 8,
		DEPTH = 9,
		IN = 10,
		OUT = 11,
		SIGNAL = 12,
		P2P = 13,
		FULLRATE = 14,
		BYPASS = 15,
		MEMORYSPACE = 16,
		UNORDERED = 17,
		LBRACE = 18,
		RBRACE = 19,
		CAPACITY = 20,
		DATAWIDTH = 21,
		ADDRWIDTH = 22,
		MAXACCESSWIDTH = 23,
		OBJECT = 24,
		COLON = 25,
		FOREIGN = 26,
		PIPELINE = 27,
		BUFFERING = 28,
		OPERATOR = 29,
		VOLATILE = 30,
		MODULE = 31,
		SIMPLE_IDENTIFIER = 32,
		EQUIVALENT = 33,
		LPAREN = 34,
		RPAREN = 35,
		OPEN = 36,
		DIV_OP = 37,
		ENTRY = 38,
		EXIT = 39,
		CONTROLPATH = 40,
		PLACE = 41,
		LEFT_OPEN = 42,
		TRANSITION = 43,
		DEAD = 44,
		TIED_HIGH = 45,
		DELAY = 46,
		SERIESBLOCK = 47,
		PARALLELBLOCK = 48,
		BRANCHBLOCK = 49,
		LOOPBLOCK = 50,
		TERMINATE = 51,
		PHISEQUENCER = 52,
		TRANSITIONMERGE = 53,
		BIND = 54,
		IMPLIES = 55,
		ULE_OP = 56,
		MERGE = 57,
		BRANCH = 58,
		FORKBLOCK = 59,
		PIPELINEDFORKBLOCK = 60,
		N_ULL = 61,
		JOIN = 62,
		MARKEDJOIN = 63,
		FORK = 64,
		DATAPATH = 65,
		GUARD = 66,
		NOT_OP = 67,
		FLOWTHROUGH = 68,
		PLUS_OP = 69,
		MINUS_OP = 70,
		MUL_OP = 71,
		SHL_OP = 72,
		SHR_OP = 73,
		ROL_OP = 74,
		ROR_OP = 75,
		UGT_OP = 76,
		UGE_OP = 77,
		EQ_OP = 78,
		ULT_OP = 79,
		NEQ_OP = 80,
		BITSEL_OP = 81,
		CONCAT_OP = 82,
		UNORDERED_OP = 83,
		OR_OP = 84,
		AND_OP = 85,
		XOR_OP = 86,
		NOR_OP = 87,
		NAND_OP = 88,
		XNOR_OP = 89,
		SHRA_OP = 90,
		SGT_OP = 91,
		SGE_OP = 92,
		SLT_OP = 93,
		SLE_OP = 94,
		StoS_ASSIGN_OP = 95,
		StoU_ASSIGN_OP = 96,
		UtoS_ASSIGN_OP = 97,
		FtoS_ASSIGN_OP = 98,
		FtoU_ASSIGN_OP = 99,
		StoF_ASSIGN_OP = 100,
		UtoF_ASSIGN_OP = 101,
		FtoF_ASSIGN_OP = 102,
		DECODE_OP = 103,
		ENCODE_OP = 104,
		P_ENCODE_OP = 105,
		BITREDUCE_OR_OP = 106,
		BITREDUCE_AND_OP = 107,
		BITREDUCE_XOR_OP = 108,
		BRANCH_OP = 109,
		SELECT_OP = 110,
		SLICE_OP = 111,
		PERMUTE_OP = 112,
		ASSIGN_OP = 113,
		HASH = 114,
		EQUIVALENCE_OP = 115,
		CALL = 116,
		INLINE = 117,
		IOPORT = 118,
		LOAD = 119,
		FROM = 120,
		STORE = 121,
		TO = 122,
		PHI = 123,
		LBRACKET = 124,
		RBRACKET = 125,
		CONSTANT = 126,
		INTERMEDIATE = 127,
		WIRE = 128,
		BINARYSTRING = 129,
		HEXSTRING = 130,
		COMMA = 131,
		INT = 132,
		FLOAT = 133,
		POINTER = 134,
		ARRAY = 135,
		OF = 136,
		RECORD = 137,
		SIMPLE_IDENTIFER = 138,
		ATTRIBUTE = 139,
		QUOTED_STRING = 140,
		DPE = 141,
		LIBRARY = 142,
		REQS = 143,
		ACKS = 144,
		HIDDEN = 145,
		PARAMETER = 146,
		PORT = 147,
		MAP = 148,
		MIN = 149,
		MAX = 150,
		DPEINSTANCE = 151,
		LINK = 152,
		AT = 153,
		ORDERED_OP = 154,
		WHITESPACE = 155,
		SINGLELINECOMMENT = 156,
		ALPHA = 157,
		DIGIT = 158,
		NULL_TREE_LOOKAHEAD = 3
	};
#ifdef __cplusplus
};
#endif
#endif /*INC_vcParserTokenTypes_hpp_*/
