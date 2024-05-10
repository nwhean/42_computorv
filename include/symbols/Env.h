#ifndef ENV_H
# define ENV_H

# include "Object.h"

extern const void	*Env;	/* new(Env, Env_prev); */

extern const void	*EnvClass;

void	initEnv(void);
void	Env_put(void *self, const void *id, void *expr);
void	*Env_get(const void *self, const void *id);

#endif
