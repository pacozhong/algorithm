/* 
   hash table: use array to construct bucket 
 */

typedef int xl_ht_array_compare_fun_t(const void *node1, const void *node2);

typedef uint32_t xl_ht_array_hash_fun_t(const void *node);

typedef int xl_ht_array_eliminate_fun_t(const void *node, const void *arg);

typedef struct xl_ht_array {
	void *mem;
	void *zero;
	size_t unit_size;
	size_t unit_num;
	size_t bukcet_num;
	size_t *mods;

	xl_ht_array_compare_fun_t *compare_fun;
	xl_ht_array_hash_fun_t *hash_fun;
	xl_ht_array_eliminate_fun_t *eliminate_fun;	
} xl_ht_array_t;


static int _is_prime(size_t x){
	if(x <= 1) return 0;
	size_t i = 2;	
	for(;i * i <= x; i ++){
		if(x % i == 0) return 0;
	}
	return 1;
}

static size_t _get_primes(size_t base, size_t *primes, size_t num){
	size_t total_num = 0;
	size_t i = base;
	int index = 0;
	while(1){
		if(_is_prime(i)) {
			*(primes + index) = i;
			total_num += i;
			index ++;
			if(index == num) break;
		}
		i ++;
	}	
}


xl_ht_array_t* xl_ht_array_init(size_t unit_size, size_t unit_num, size_t bucket_num, 
		xl_ht_array_compare_fun_t *compare_fun, xl_ht_array_hash_fun_t *hash_fun,
		xl_ht_array_eliminate_fun_t *eliminate_fun){
	
	xl_ht_array_t *ht = (xl_ht_array_t*)calloc(sizeof(xl_ht_array_t), 1);	
	if(ht == NULL) return NULL;

	ht->unit_size = unit_size;
	ht->bucket_num = bucket_num;
	ht->mods = (size_t*)calloc(sizeof(size_t), ht->bucket_num); 

	ht->unit_num = _get_primes((unit_num + bucket_num - 1) / bucket_num, ht->mods, bucket_num);

	ht->mem = calloc(ht->unit_size, ht->unit_num + 1);
	ht->zero = ht->mem + ht->unit_size * ht->unit_num;	

	ht->compare_fun = compare_fun;
	ht->hash_fun = hash_fun;
	ht->eliminate_fun = eliminate_fun;
	
	return ht;	
}

int xl_ht_array_destroy(xl_ht_array_t *ht){
	
	if(ht == NULL) return 0;
	if(ht->mem != NULL) free(ht->mem);
	if(ht->mods != NULL) free(ht->mods);
	free(ht);
	return 0;
}

void* _xl_ht_array_find(xl_ht_array_t *ht, uint32_t key, const void *data){
	
	void *base = ht->mem, *target;
	size_t i;
	for(i = 0; i < ht->bucket_num; i ++){
		target = base + ((key % ht->mods[i]) * ht->unit_size);
		if(0 == ht->compare_fun(data, target)){
			return target;
		}
		base += ht->mods[i] * ht->unit_size;	
	}
	return NULL;
}

void* xl_ht_array_get(xl_ht_array_t *ht, const void *data){
	return _xl_ht_array_find(ht, ht->hash_fun(data), data);	
}

void* _eliminate(xl_ht_array_t *ht, uint32_t key, void *data){
	
	void *base = ht->mem, *target, *node;
	size_t i;
	int x = 0;
	for(i = 0; i < ht->bucket_num; i ++){
		node = base + ((key % ht->mods[i]) * ht->unit_size);
		int ret = ht->eliminate(target, data);
		if(ret > x){
			x = ret;
			target = node;
		}
		base += ht->mods[i] * ht->unit_size;
	}
	return target;
}

void* xl_ht_array_put(xl_ht_array_t *ht, void *data, void *eliminate_arg){
	
	uint32_t key = ht->hash_fun(data);
	void* node = _xl_ht_array_find(ht, key, ht->zero);
	if(node){
		memcpy(node, data, ht->unit_size);
		return node;
	}
	if(ht->eliminate_fun){
		node = ht->eliminate_fun(key, eliminate_arg);	
		memcpy(node, data, ht->unit_size);
		return node;
	}
	return NULL;
} 

void* xl_ht_array_add(xl_ht_array_t *ht, void *data, int *exist, void *eliminate_arg){
	
	uint32_t key = ht->hash_fun(data);
	void* node = _xl_ht_array_find(ht, key, data);
	if(node){
		if(exist)
			*exist = 1;
		return node;
	}
	if(exist)
		*exist = 0;
	return	xl_ht_array_put(ht, data, eliminate_arg);
}

int xl_ht_array_delete(xl_ht_array_t *ht, void *data, int *exist){

	uint32_t key = ht->hash_fun(data);
	void *node = _xl_ht_array_find(ht, key, data);
	if(node){
		memcpy(node, ht->zero, ht->unit_size);
		if(exist) *exist = 1;
		return 0;
	}
	if(exist) *exist = 0;
	return 0;
}

typedef int xl_ht_array_travel_fun_t(void *node, void *arg);


size_t xl_ht_array_travel(xl_ht_array_t *ht, xl_ht_array_travel_fun_t *travel_fun, void *arg){
	
	size_t success_node = 0;
	void *node = ht->mem;
	size_t i = 0;
	for( ; i < ht->unit_num; i ++){
		if(0 != ht->comapre_fun(node, ht->zero)){
			int travel_ret = travel_fun(node, arg);
			if(travel_ret == 0) success_node ++;
		}
		node += ht->unit_size;
	}
	return success_node;
}

static int _xl_ht_array_is_empty(void *node, void *arg){
	
	return 0;
}

size_t xl_ht_array_node_used(xl_ht_array_t *ht){
	return xl_ht_array_travel(ht, _xl_ht_array_is_empty, NULL);
}
