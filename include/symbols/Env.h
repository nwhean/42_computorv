#ifndef ENV_H
# define ENV_H

# include "Object.h"

extern const void	*Env;	/* new(Env, Env_prev); */

extern const void	*EnvClass;

void	initEnv(void);
void	Env_put(void *self, void* id, void *expr);
void	*Env_get(const void *self, void *id);

#endif