#define HPRIME 97

int hashfunc(long key)
{
    char *str = (char *)key;
    int ret = 0;
    while (*str) {
        ret |= *str;
        str ++;
    }
    return ret % HPRIME;
}


struct item {
    long key;
    long value;
    struct item *next;
};

struct hashmap {
    struct item **set;
    long size;
};

struct hashmap *hashmap_new()
{
    struct hashmap *obj = malloc(sizeof(struct hashmap));
    if (!obj)
        return NULL;
    obj->set = calloc(HPRIME, sizeof(struct item*));
    if (!obj->set) {
        free(obj);
        return NULL;
    }
    obj->size = 0;
    return obj;
}

void hashmap_free(struct hashmap *obj)
{
    if (!obj) {
        if (obj->set) {
            for (int i = 0; i < HPRIME; i ++) {
                struct item *cur = obj->set[i];
                while (cur) {
                    struct item *next = cur->next;
                    free(cur);
                    cur = next;
                }
            }
            free(obj->set);
        }
        free(obj);
    }
}

void hashmap_put(struct hashmap *obj, long key, long value) 
{
    if (!obj)
        return;
    int idx = hashfunc(key);
    struct item *cur = obj->set[idx];
    while (cur && strcmp((char*)cur->key, (char*)key)) cur = cur->next;
    if (!cur) {
        struct item *new = malloc(sizeof(struct item));
        if (!new)
            return;
        new->key = key;
        new->value = value;
        new->next = obj->set[idx];
        obj->set[idx] = new;
        obj->size ++;
    } else {
        cur->value = value;
    }
}

long hashmap_get(struct hashmap *obj, long key)
{
    if (!obj)
        return -1;
    int idx = hashfunc(key);
    struct item *cur = obj->set[idx];
    while (cur && strcmp((char*)cur->key, (char*)key))  cur = cur->next;
    if (cur)
        return cur->value;
    else
        return -1;
}

void hashmap_del(struct hashmap *obj, long key)
{
    if (!obj)
        return;
    int idx = hashfunc(key);
    struct item *cur = obj->set[idx];
    struct item *parent = NULL;
    while (cur && strcmp((char*)cur->key, (char*)key)) {
        parent = cur;
        cur = cur->next;
    }
    if (cur) {
        if (parent)
            parent->next = cur->next;
        else
            obj->set[idx] = cur->next;
        free(cur);
        obj->size --;
    } 
}

long hashmap_size(struct hashmap *obj)
{
    if (obj)
        return obj->size;
    else
        return 0;
}
