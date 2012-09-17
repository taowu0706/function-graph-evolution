// Project EvoForm
// Licensed under GPLv3
// Author: Wenjie Fang (a.k.a fwjmath)

// expr.h
// Treatment of expression construction, evaluation, etc.

#include "cnvt.h"

enum M_op { SINH, COS, SLOG, EXP, NEG, ATAN, SQRT, ABS, SGN };
enum D_op { PLUS, MINUS, TIMES, DIV, POW, FMOD };
enum Leaf { X, Y, C };
enum NodeType { MONO, DUAL, LEAF };

struct expr{
	NodeType nt;
	union{
		M_op m;
		D_op d;
		Leaf l;
	};
	expr* lt;
	union{
		expr* rt;
		float value;
	};
};

int expr_strlen(expr*);
int expr2str(expr*, char*);
expr* str2expr(char*);
void free_expr(expr*);
float eval_expr(expr*, float, float);
color_RGB eval_expr_color(expr*, float, float);
void opt_expr(expr*);
expr* dup_expr(expr*);
int nodecnt_expr(expr* e);
expr* nthnode_expr(expr*, int);
expr** nthnode_ptr_expr(expr*, int);