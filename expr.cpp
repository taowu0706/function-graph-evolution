#include "expr.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>

int expr_strlen(expr* e){
	switch(e->nt){
	case MONO:
		switch(e->m){
		case NEG:
		case COS:
		case EXP:
		case ABS:
		case SGN:
			return 6+expr_strlen(e->lt);
			break;
		case SLOG:
		case ATAN:
		case SQRT:
		case SINH:
			return 7+expr_strlen(e->lt);
			break;
		default:
			return 0;
		}
		break;
	case LEAF:
		switch(e->l){
		case X:
		case Y:
			return 3;
			break;
		case C:
			return 7; // we fix +1.2f
			break;
		default:
			return 0;
		}
		break;
	case DUAL:
		switch(e->d){
		case PLUS:
		case MINUS:
		case TIMES:
		case DIV:
		case POW:
		case FMOD:
			return 5+expr_strlen(e->lt)+expr_strlen(e->rt);
			break;
		default:
			return 0;
		}
		break;
	default:
		return 0;
	}
}

int expr2str(expr* e, char* c){
	int ptr=0;
	switch(e->nt){
	case MONO:
		switch(e->m){
		case NEG:
			strcpy(&c[0],"(neg ");
			ptr=5;
			ptr+=expr2str(e->lt,&c[5]);
			c[ptr]=')';
			ptr++;
			return ptr;
			break;
		case SINH:
			strcpy(&c[0],"(sinh ");
			ptr=6;
			ptr+=expr2str(e->lt,&c[6]);
			c[ptr]=')';
			ptr++;
			return ptr;
			break;
		case COS:
			strcpy(&c[0],"(cos ");
			ptr=5;
			ptr+=expr2str(e->lt,&c[5]);
			c[ptr]=')';
			ptr++;
			return ptr;
			break;
		case EXP:
			strcpy(&c[0],"(exp ");
			ptr=5;
			ptr+=expr2str(e->lt,&c[5]);
			c[ptr]=')';
			ptr++;
			return ptr;
			break;
		case ABS:
			strcpy(&c[0],"(abs ");
			ptr=5;
			ptr+=expr2str(e->lt,&c[5]);
			c[ptr]=')';
			ptr++;
			return ptr;
			break;
		case SGN:
			strcpy(&c[0],"(sgn ");
			ptr=5;
			ptr+=expr2str(e->lt,&c[5]);
			c[ptr]=')';
			ptr++;
			return ptr;
			break;
		case SLOG:
			strcpy(&c[0],"(slog ");
			ptr=6;
			ptr+=expr2str(e->lt,&c[6]);
			c[ptr]=')';
			ptr++;
			return ptr;
			break;
		case ATAN:
			strcpy(&c[0],"(atan ");
			ptr=6;
			ptr+=expr2str(e->lt,&c[6]);
			c[ptr]=')';
			ptr++;
			return ptr;
			break;
		case SQRT:
			strcpy(&c[0],"(sqrt ");
			ptr=6;
			ptr+=expr2str(e->lt,&c[6]);
			c[ptr]=')';
			ptr++;
			return ptr;
			break;
		default:
			return 0;
		}
		break;
	case LEAF:
		switch(e->l){
		case X:
			strcpy(c,"(x)");
			return 3;
			break;
		case Y:
			strcpy(c,"(y)");
			return 3;
			break;
		case C:
			sprintf(c,"(%+1.2f)", e->value);
			return 7; // we fix +1.2f
			break;
		default:
			return 0;
		}
		break;
	case DUAL:
		switch(e->d){
		case PLUS:
			strcpy(c,"(+ ");
			ptr=3;
			ptr+=expr2str(e->lt, &c[ptr]);
			c[ptr]=' ';
			ptr++;
			ptr+=expr2str(e->rt, &c[ptr]);
			c[ptr]=')';
			ptr++;
			return ptr;
			break;
		case MINUS:
			strcpy(c,"(- ");
			ptr=3;
			ptr+=expr2str(e->lt, &c[ptr]);
			c[ptr]=' ';
			ptr++;
			ptr+=expr2str(e->rt, &c[ptr]);
			c[ptr]=')';
			ptr++;
			return ptr;
			break;
		case TIMES:
			strcpy(c,"(* ");
			ptr=3;
			ptr+=expr2str(e->lt, &c[ptr]);
			c[ptr]=' ';
			ptr++;
			ptr+=expr2str(e->rt, &c[ptr]);
			c[ptr]=')';
			ptr++;
			return ptr;
			break;
		case DIV:
			strcpy(c,"(/ ");
			ptr=3;
			ptr+=expr2str(e->lt, &c[ptr]);
			c[ptr]=' ';
			ptr++;
			ptr+=expr2str(e->rt, &c[ptr]);
			c[ptr]=')';
			ptr++;
			return ptr;
			break;
		case POW:
			strcpy(c,"(^ ");
			ptr=3;
			ptr+=expr2str(e->lt, &c[ptr]);
			c[ptr]=' ';
			ptr++;
			ptr+=expr2str(e->rt, &c[ptr]);
			c[ptr]=')';
			ptr++;
			return ptr;
			break;
		case FMOD:
			strcpy(c,"(\\ ");
			ptr=3;
			ptr+=expr2str(e->lt, &c[ptr]);
			c[ptr]=' ';
			ptr++;
			ptr+=expr2str(e->rt, &c[ptr]);
			c[ptr]=')';
			ptr++;
			return ptr;
			break;
		default:
			return 0;
		}
		break;
	default:
		return 0;
	}
}

expr* str2expr(char* c){
	int d=1, ptr=1, m=0, bp=-1;
	expr* e = (expr*)malloc(sizeof(expr));
	while(d>0){
		switch(c[ptr]){
		case '(':
			d++;
			break;
		case ')':
			d--;
			break;
		case ' ':
			if(d==1){
				m++;
				if(m==2) bp=ptr+1;
			}
			break;
		default:
			break;
		}
		ptr++;
	}
	switch(m){
	case 0:
		e->nt=LEAF;
		switch(c[1]){
		case 'x':
			e->l=X;
			break;
		case 'y':
			e->l=Y;
			break;
		default:
			e->l=C;
			sscanf(&c[1],"%f",&(e->value));
		}
		return e;
		break;
	case 1:
		e->nt=MONO;
		switch(c[2]){
		case 'i':
			e->m=SINH;
			e->lt=str2expr(&c[6]);
			break;
		case 'o':
			e->m=COS;
			e->lt=str2expr(&c[5]);
			break;
		case 'l':
			e->m=SLOG;
			e->lt=str2expr(&c[6]);
			break;
		case 'x':
			e->m=EXP;
			e->lt=str2expr(&c[5]);
			break;
		case 'e':
			e->m=NEG;
			e->lt=str2expr(&c[5]);
			break;
		case 't':
			e->m=ATAN;
			e->lt=str2expr(&c[6]);
			break;
		case 'q':
			e->m=SQRT;
			e->lt=str2expr(&c[6]);
			break;
		case 'b':
			e->m=ABS;
			e->lt=str2expr(&c[5]);
			break;
		case 'g':
			e->m=SGN;
			e->lt=str2expr(&c[5]);
			break;
		default:
			e->lt=0;
			break;
		}
		return e;
		break;
	case 2:
		e->nt=DUAL;
		e->lt=str2expr(&c[3]);
		e->rt=str2expr(&c[bp]);
		switch(c[1]){
		case '+':
			e->d=PLUS;
			break;
		case '-':
			e->d=MINUS;
			break;
		case '*':
			e->d=TIMES;
			break;
		case '/':
			e->d=DIV;
			break;
		case '\\':
			e->d=FMOD;
			break;
		case '^':
			e->d=POW;
			break;
		default:
			e->lt=0;
			break;
		}
		return e;
		break;
	default:
		return e;
	}
}

void free_expr(expr* e){
	switch(e->nt){
	case MONO:
		if(e->lt!=0){
			free_expr(e->lt);
		}
		break;
	case DUAL:
		if(e->lt!=0){
			free_expr(e->lt);
		}
		if(e->rt!=0){
			free_expr(e->rt);
		}
		break;
	case LEAF:
	default:;
	}
	free(e);
	return;
}

float eval_expr(expr* e, float x, float y){
	switch(e->nt){
	case LEAF:
		switch(e->l){
		case X:
			return x;
			break;
		case Y:
			return y;
			break;
		case C:
		default:
			return e->value;
			break;
		}
		break;
	case MONO:
		switch(e->m){
		case SINH:
			return sinh(eval_expr(e->lt,x,y));
			break;
		case COS:
			return cos(eval_expr(e->lt,x,y));
			break;
		case SLOG:
			return log(1+abs(eval_expr(e->lt,x,y)));
			break;
		case EXP:
			return exp(eval_expr(e->lt,x,y));
			break;
		case NEG:
			return -(eval_expr(e->lt,x,y));
			break;
		case ATAN:
			return atan(eval_expr(e->lt,x,y));
			break;
		case SQRT:
			return sqrt(abs(eval_expr(e->lt,x,y)));
			break;
		case ABS:
			return abs(eval_expr(e->lt,x,y));
			break;
		case SGN:
		default:
			if(eval_expr(e->lt,x,y)>0) return 1.0f; else return -1.0f;
			break;
		}
		break;
	case DUAL:
		switch(e->d){
		case PLUS:
			return eval_expr(e->lt,x,y) + eval_expr(e->rt,x,y);
			break;
		case MINUS:
			return eval_expr(e->lt,x,y) - eval_expr(e->rt,x,y);
			break;
		case TIMES:
			return eval_expr(e->lt,x,y) * eval_expr(e->rt,x,y);
			break;
		case DIV:
			if(abs(eval_expr(e->rt,x,y)) > 1e-7) return eval_expr(e->lt,x,y) / eval_expr(e->rt,x,y); else return 0.0f;
			break;
		case POW:
			if(pow(abs(eval_expr(e->lt,x,y))+1e-7f, eval_expr(e->rt,x,y))==HUGE_VAL) return 0.0f; else return pow(abs(eval_expr(e->lt,x,y))+1e-7f, eval_expr(e->rt,x,y));
			break;
		case FMOD:
			if(eval_expr(e->rt,x,y) > 1e-7) return fmod(eval_expr(e->lt,x,y) ,eval_expr(e->rt,x,y)); else return 0.0f;
			break;
		default:
			return 0.0f;
		}
		break;
	default:
		return 0.0f;
	}
}

color_RGB eval_expr_color(expr* e, float x, float y){
	return float2rgb(eval_expr(e,x,y));
}

void opt_expr(expr* e){
	switch(e->nt){
	case MONO:
		opt_expr(e->lt);
		switch(e->m){
		case ABS:
			if((e->lt->nt==LEAF)&&(e->lt->l==C)){
				e->nt=LEAF;
				e->value=abs(e->lt->value);
				free(e->lt);
				return;
			}
			if((e->lt->nt==MONO)&&((e->lt->m==EXP)||(e->lt->m==SQRT)||(e->lt->m==ABS))){
				expr* tmp=e->lt;
				e->m=tmp->m;
				e->lt=tmp->lt;
				free(tmp);
				return;
			}
			if((e->lt->nt==MONO)&&((e->lt->m==POW))){
				expr* tmp=e->lt;
				e->d=tmp->d;
				e->lt=tmp->lt;
				e->rt=tmp->rt;
				free(tmp);
				return;
			}
			if((e->lt->nt==MONO)&&((e->lt->m==NEG))){
				expr* tmp=e->lt;
				e->lt=tmp->lt;
				free(tmp);
				return;
			}
			if((e->lt->nt==MONO)&&((e->lt->m==SGN))){
				free_expr(e->lt);
				e->nt=LEAF;
				e->l=C;
				e->value=1.0f;
				return;
			}
			break;
		case SLOG:
			if((e->lt->nt==MONO)&&((e->lt->m==ABS)||(e->lt->m==NEG))){
				expr* tmp=e->lt;
				e->lt=tmp->lt;
				free(tmp);
				return;
			}
			break;
		case SQRT:
			if((e->lt->nt==MONO)&&((e->lt->m==ABS)||(e->lt->m==NEG))){
				expr* tmp=e->lt;
				e->lt=tmp->lt;
				free(tmp);
				return;
			}
			if((e->lt->nt==MONO)&&(e->lt->m==SGN)){
				free_expr(e->lt);
				e->nt=LEAF;
				e->l=C;
				e->value=1.0f;
				return;
			}
			break;
		case SGN:
			if((e->lt->nt==MONO)&&((e->lt->m==EXP)||(e->lt->m==ABS)||(e->lt->m==SQRT))){
				free_expr(e->lt);
				e->nt=LEAF;
				e->l=C;
				e->value=1.0f;
				return;
			}
			if((e->lt->nt==POW)&&(e->lt->d==POW)){
				free_expr(e->lt);
				e->nt=LEAF;
				e->l=C;
				e->value=1.0f;
				return;
			}
			if((e->lt->nt==SGN)&&(e->lt->m==SGN)){
				expr* tmp=e->lt;
				e->lt=tmp->lt;
				free(tmp);
				return;
			}
		default:
			return;
		}
		break;
	case DUAL:
		opt_expr(e->lt);
		opt_expr(e->rt);
		if((e->d==POW)&&(e->rt->nt==MONO)&&(e->rt->m==ABS)){
			expr* tmp=e->rt;
			e->rt=tmp->lt;
			free(tmp);
		}
	case LEAF:
	default:
		return;
	}
}

expr* dup_expr(expr* e){
	expr* newe=(expr*)malloc(sizeof(expr));
	newe->nt=e->nt;
	switch(e->nt){
	case LEAF:
		newe->l=e->l;
		newe->value=e->value;
		break;
	case MONO:
		newe->m=e->m;
		newe->lt=dup_expr(e->lt);
		break;
	case DUAL:
	default:
		newe->d=e->d;
		newe->lt=dup_expr(e->lt);
		newe->rt=dup_expr(e->rt);
		break;
	}
	return newe;
}

int nodecnt_expr(expr* e){
	switch(e->nt){
	case DUAL:
		return 1+nodecnt_expr(e->lt)+nodecnt_expr(e->rt);
		break;
	case MONO:
		return 1+nodecnt_expr(e->lt);
		break;
	case LEAF:
	default:
		return 1;
	}
}

expr* nthnode_expr(expr* e, int n){
	if(n==0) return e;
	switch(e->nt){
	case DUAL:
		{
			int nl=nodecnt_expr(e->lt);
			if(n<=nl){
				return nthnode_expr(e->lt,n-1);
			}else{
				return nthnode_expr(e->rt,n-1-nl);
			}
		}
		break;
	case MONO:
		return nthnode_expr(e->lt,n-1);
		break;
	case LEAF:
	default:
		return e;
	}
}

expr** nthnode_ptr_expr(expr* e, int n){
	if(n==0) return 0;
	switch(e->nt){
	case DUAL:
		{
			int nl=nodecnt_expr(e->lt);
			if(n==1) return &(e->lt);
			else if(n==nl+1) return &(e->lt);
			else if(n>nl+1) return nthnode_ptr_expr(e->rt,n-nl-1);
			else return nthnode_ptr_expr(e->lt,n-1);
		}
		break;
	case MONO:
		if(n==1) return &(e->lt);
		else return nthnode_ptr_expr(e->lt,n-1);
		break;
	case LEAF:
	default:
		return 0;
	}
}