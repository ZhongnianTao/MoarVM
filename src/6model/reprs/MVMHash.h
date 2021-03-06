/* Representation used by VM-level hashes. */

struct MVMHashEntry {
    /* value object */
    MVMObject *value;

    /* the uthash hash handle inline struct, including the key. */
    UT_hash_handle hash_handle;
};

struct MVMHashBody {
    /* uthash updates this pointer directly. */
    MVMHashEntry *hash_head;
};
struct MVMHash {
    MVMObject common;
    MVMHashBody body;
};

/* Function for REPR setup. */
const MVMREPROps * MVMHash_initialize(MVMThreadContext *tc);

#define MVM_HASH_BIND(tc, hash, key, value) \
    do { \
        if (MVM_LIKELY(!MVM_is_null(tc, (MVMObject *)key) && REPR(key)->ID == MVM_REPR_ID_MVMString \
                && IS_CONCRETE(key))) { \
            HASH_ADD_KEYPTR_VM_STR(tc, hash_handle, hash, key, value); \
        } \
        else { \
            MVM_exception_throw_adhoc(tc, "Hash keys must be concrete strings (got %s)", MVM_6model_get_debug_name(tc, (MVMObject *)key)); \
        } \
    } while (0);

#define MVM_HASH_GET(tc, hash, key, value) \
    do { \
        if (MVM_LIKELY(!MVM_is_null(tc, (MVMObject *)key) && REPR(key)->ID == MVM_REPR_ID_MVMString \
                && IS_CONCRETE(key))) { \
            HASH_FIND_VM_STR(tc, hash_handle, hash, key, value); \
        } \
        else { \
            MVM_exception_throw_adhoc(tc, "Hash keys must be concrete strings (got %s)", MVM_6model_get_debug_name(tc, (MVMObject *)key)); \
        } \
    } while (0);

#define MVM_HASH_KEY(entry) ((MVMString *)(entry)->hash_handle.key)

#define MVM_HASH_DESTROY(tc, hash_handle, hashentry_type, head_node) do { \
    hashentry_type *current, *tmp; \
    HASH_ITER_FAST(tc, hash_handle, head_node, current, { \
        if (current != head_node) \
            MVM_free(current); \
    }); \
    tmp = head_node; \
    HASH_CLEAR(tc, hash_handle, head_node); \
    MVM_free(tmp); \
} while (0)

void MVMHash_at_key(MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data, MVMObject *key_obj, MVMRegister *result, MVMuint16 kind);
void MVMHash_bind_key(MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data, MVMObject *key_obj, MVMRegister value, MVMuint16 kind);
