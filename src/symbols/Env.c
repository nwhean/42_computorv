#include "UnorderedMap.h"

#include "Env.r"
#include "Env.h"

const void	*Env;
const void	*EnvClass;

/* Env constructor method. */
static void	*Env_ctor(void *_self, va_list *app)
{
	struct s_Env	*self;

	self = super_ctor(Env, _self, app);
	self->table = new(UnorderedMap);
	self->prev = va_arg(*app, struct s_Env *);
	return (self);
}

/* Env destructor method. */
static void	*Env_dtor(void *_self)
{
	struct s_Env	*self = _self;

	delete(self->table);
	return (super_dtor(Env, _self));
}

/* put id and expr as key-value pair in the hash table */
void	Env_put(void *_self, void* id, void *expr)
{
	const struct s_Env	*self = _self;

	UnorderedMap_erase(self->table, id);
	UnorderedMap_insert(self->table, id, expr);
}

/* get the expr from the hash table */
void	*Env_get(const void *_self, const void *id)
{
	const struct s_Env	*self = _self;
	struct s_Env		*env;

	for (env = (struct s_Env *)self; env != NULL; env = env->prev)
	{
		void	*found = UnorderedMap_find(env->table, id);

		if (found)
			return (found);
	}
	return (NULL);
}

/* EnvClass constructor method. */
static void	*EnvClass_ctor(void *_self, va_list *app)
{
	typedef void		(*voidf)();
	struct s_EnvClass	*self;
	voidf				selector;
	va_list				ap;

	self = super_ctor(EnvClass, _self, app);
	#ifdef va_copy
		va_copy(ap, *app);
	#else
		*ap = **app;
	#endif
	while ((selector = va_arg(ap, voidf)))
	{
		voidf	method;

		#pragma GCC diagnostic ignored "-Wcast-function-type"
		(void)method;
		#pragma GCC diagnostic pop
	}
	return (self);
}

void	initEnv(void)
{
	if (!EnvClass)
		EnvClass = new(Class, "EnvClass",
				Class, sizeof(struct s_EnvClass),
				ctor, EnvClass_ctor,
				0);
	if (!Env)
		Env = new(EnvClass, "Env",
				Object, sizeof(struct s_Env),
				ctor, Env_ctor,
				dtor, Env_dtor,
				0);
}
