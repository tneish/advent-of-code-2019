/*  dict is struct intIntKey: struct coord_val */
dictType int_int_dict = {
    intIntHash,
    intIntKeyDup,
    coordValDup,
    intIntKeyCompare,
    intIntKeyDestructor,
    coordValDestructor
};

static dictType string_string_dict = {
    stringHash,
    stringKeyDup,
    stringValDup,
    stringKeyCompare,
    stringKeyDestructor,
    stringValDestructor
};

static dictType string_listptr_dict = {
    stringHash,
    stringKeyDup,
    listPtrValDup,
    stringKeyCompare,
    stringKeyDestructor,
    listPtrValDestructor
};
