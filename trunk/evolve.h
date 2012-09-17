#include "SDL.h"
#include "expr.h"

#define EVO_SQLEN (600)
#define EVO_XSTART (-1.5f)
#define EVO_YSTART (1.5f)
#define EVO_STEPXY (0.005f)

#define MUTATION_RATE 0.01f
#define POPULATION 16
#define WINNERCNT 6

struct crab{
	expr* e;
	Uint8 fullbuf[EVO_SQLEN*EVO_SQLEN*3];
	Uint8 smallbuf[EVO_SQLEN*EVO_SQLEN*3/16];
	bool expanded;
};

crab* expr2crab(expr*);
void expand_crab(crab*);
expr* randexpr_s(void);
bool mutate(expr*);
expr* randnode_expr(expr*);
expr* crossover(expr*, expr*);
void free_crab(crab*);
bool natural_selection(crab**, bool*);
void init_pool(crab**);