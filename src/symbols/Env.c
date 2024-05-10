#include "Str.h"
#include "UnorderedMap.h"
#include "Vec.h"

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

/* Return '\n' delimited string of all key-value pairs in the environment */
static char	*Env_str(const void *_self)
{
	const struct s_Env	*self = _self;
	void				*keys = UnorderedMap_keys(self->table);
	void				*values = UnorderedMap_values(self->table);
	void				*s = new(Str, "");
	char				*append;
	size_t				size = Vec_size(keys);
	size_t				i;

	for (i = 0; i < size; ++i)
	{
		append = str(Vec_at(keys, i));
		Str_append(s, append);
		free(append);
		Str_append(s, " = ");

		append = str(Vec_at(values, i));
		Str_append(s, append);
		free(append);
		Str_push_back(s, '\n');
	}

	append = str(s);
	delete(s);
	return (append);
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
				str, Env_str,
				0);
}
