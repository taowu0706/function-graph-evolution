#include <stdlib.h>
#include "evolve.h"

int rand_expr_cnt;

float rand_proba(){
	return (float)rand()/RAND_MAX;
}

crab* expr2crab(expr* e){
	crab* cr=(crab*)malloc(sizeof(crab));
	cr->e=e;
	cr->expanded=false;
	float x, y=EVO_YSTART;
	color_RGB c;
	Uint8* ptr=cr->smallbuf;
	int i, j;
	for(i = 0; i < (EVO_SQLEN>>2); i++){
		x=EVO_XSTART;
		for(j = 0; j < (EVO_SQLEN>>2); j++){
			c = eval_expr_color(e,x,y);
			ptr[0]=c.b;
			ptr[1]=c.g;
			ptr[2]=c.r;
			ptr+=3;
			x += EVO_STEPXY * 4;
		}
		y -= EVO_STEPXY * 4;
	}
	return cr;
}

void update_crab(crab* cr){
	float x, y=EVO_YSTART;
	color_RGB c;
	cr->expanded=false;
	Uint8* ptr=cr->smallbuf;
	int i, j;
	for(i = 0; i < (EVO_SQLEN>>2); i++){
		x=EVO_XSTART;
		for(j = 0; j < (EVO_SQLEN>>2); j++){
			c = eval_expr_color(cr->e,x,y);
			ptr[0]=c.b;
			ptr[1]=c.g;
			ptr[2]=c.r;
			ptr+=3;
			x += EVO_STEPXY * 4;
		}
		y -= EVO_STEPXY * 4;
	}
	return;
}

void expand_crab(crab* cr){
	int i, j;
	float x, y=EVO_YSTART;
	cr->expanded=true;
	color_RGB c;
	Uint8* ptr=cr->fullbuf;
	for(i = 0; i < EVO_SQLEN; i++){
		x=EVO_XSTART;
		for(j = 0; j < EVO_SQLEN; j++){
			c = eval_expr_color(cr->e,x,y);
			ptr[0]=c.b;
			ptr[1]=c.g;
			ptr[2]=c.r;
			ptr+=3;
			x += EVO_STEPXY;
		}
		y -= EVO_STEPXY;
	}
	return;
}

expr* randexpr(){
	expr* e=(expr*)malloc(sizeof(expr));
	rand_expr_cnt++;
	if(rand_expr_cnt>35){
		//LEAF
		e->nt=LEAF;
		switch((int)(rand_proba()*3)){
		case 0:
			e->l=X;
			break;
		case 1:
			e->l=Y;
			break;
		default:
			e->l=C;
			e->value=((int)((10.0f+rand_proba()*(-20.0f))*100))/100.0f;
		}
		return e;
	}
	switch((int)(rand_proba()*10)){
	case 0:
		//LEAF
		e->nt=LEAF;
		switch((int)(rand_proba()*3)){
		case 0:
			e->l=X;
			break;
		case 1:
			e->l=Y;
			break;
		default:
			e->l=C;
			e->value=10.0f+rand_proba()*(-20.0f);
		}
		break;
	case 1:
	case 2:
	case 3:
		//MONO
		e->nt=MONO;
		e->lt=randexpr();
		e->m=(M_op)((int)(rand_proba()*9));
		break;
	default:
		//DUAL
		e->nt=DUAL;
		e->lt=randexpr();
		e->rt=randexpr();
		e->d=(D_op)((int)(rand_proba()*6));
	}
	return e;
}

expr* randexpr_s(){
	rand_expr_cnt=0;
	expr* e=randexpr();
	opt_expr(e);
	return e;
}

bool mutate(expr* e){
	bool flag=false;
	switch(e->nt){
	case LEAF:
		if(rand_proba()>MUTATION_RATE) break;
		switch((int)(5*rand_proba())){
		case 0:
			if(e->l!=X) flag=true;
			e->l=X;
			break;
		case 1:
			if(e->l!=Y) flag=true;
			e->l=Y;
			break;
		case 2:
			flag=true;
			e->l=C;
			e->value=((int)((10.0f+rand_proba()*(-20.0f))*100))/100.0f;
			break;
		case 3:
			if(e->l==C){
				flag=true;
				e->value*=1.3f;
				if(e->value>=10.0f) e->value*=0.7f;
				e->value=((int)(e->value*100))/100.0f;
			}
			break;
		case 4:
			if(e->l==C){
				flag=true;
				e->value*=0.83f;
				e->value=((int)(e->value*100))/100.0f;
			}
			break;
		default:;
		}
		break;
	case MONO:
		flag|=mutate(e->lt);
		if(rand_proba()<MUTATION_RATE){
			e->m=(M_op)((int)(rand_proba()*9));
			flag=true;
		}
		break;
	case DUAL:
	default:
		flag|=mutate(e->lt);
		flag|=mutate(e->rt);
		if(rand_proba()<MUTATION_RATE){
			e->d=(D_op)((int)(rand_proba()*6));
			flag=true;
		}
	}
	return flag;
}

expr* randnode_expr(expr* e){
	return nthnode_expr(e,(int)(rand_proba()*nodecnt_expr(e)));
}

expr** randnode_ptr_expr(expr* e){
	return nthnode_ptr_expr(e,(int)(rand_proba()*nodecnt_expr(e)));
}

expr* crossover(expr* c, expr* p){
	expr* child=dup_expr(c);
	expr* branch=dup_expr(randnode_expr(p));
	expr** ins_ptr=randnode_ptr_expr(child);
	if(ins_ptr==0){
		return branch;
	}
	expr* cutting=*ins_ptr;
	*ins_ptr=branch;
	free_expr(cutting);
	mutate(child);
	opt_expr(child);
	return child;
}

void free_crab(crab* cr){
	free(cr->e);
	free(cr);
	return;
}

bool natural_selection(crab** ppl, bool* sel){
	int i, j, k, winners[WINNERCNT], losers[POPULATION-WINNERCNT];
	j=0;
	for(i=0;i<POPULATION;i++) if(sel[i]) j++;
	if(j!=WINNERCNT) return false;
	j=0; k=0;
	for(i=0;i<POPULATION;i++){
		if(sel[i]){
			winners[j]=i;
			j++;
		}else{
			losers[k]=i;
			free_crab(ppl[i]);
			k++;
		}
	}
	ppl[losers[0]]=expr2crab(randexpr_s());
	ppl[losers[1]]=expr2crab(randexpr_s());
	ppl[losers[2]]=expr2crab(randexpr_s());
	ppl[losers[3]]=expr2crab(randexpr_s());
	for(i=4;i<POPULATION-WINNERCNT;i++){
		ppl[losers[i]]=expr2crab(crossover(ppl[winners[i-4]]->e,ppl[winners[(int)(rand_proba()*WINNERCNT)]]->e));
	}
	for(i=0;i<WINNERCNT;i++){
		if(mutate(ppl[winners[i]]->e)) update_crab(ppl[winners[i]]);
	}
	return true;
}

void init_pool(crab** ppl){
	for(int i=0;i<POPULATION;i++){
		ppl[i]=expr2crab(randexpr_s());
	}
	return;
}