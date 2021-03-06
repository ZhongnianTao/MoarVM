#include "moar.h"

/* Choose the threshold for a given static frame before we start applying
 * specialization to it. */
MVMuint32 MVM_spesh_threshold(MVMThreadContext *tc, MVMStaticFrame *sf) {
    MVMuint32 bs = sf->body.bytecode_size;
    if (tc->instance->spesh_nodelay)
        return 1;
    if (bs <= 2048)
        return 150;
    else if (bs <= 8192)
        return 200;
    else
        return 300;
}
