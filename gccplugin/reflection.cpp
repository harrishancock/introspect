#include <gcc-plugin.h>

#include <cp/cp-tree.h>

#include <diagnostic-core.h>
#include <tree.h>
#include <plugin.h>
#include <plugin-version.h>

#include <cassert>
#include <cstdio>

#define FOREACH(item, chain) for (tree item = chain; item; item = TREE_CHAIN(item))

//////////////////////////////////////////////////////////////////////////////

namespace {

bool debug = true;
bool no_asm = false;
const char* const attribute_name = "reflection";

struct plugin_info reflection_info = {
    .version = "0.0.1",
    .help = "C/C++ reflection plugin",
};

const char* tree_code_string (tree t) {
    if (!t) {
        return "(null tree)";
    }

    if (TREE_CODE(t) >= 0 && TREE_CODE(t) < MAX_TREE_CODES) {
        return tree_code_name[TREE_CODE(t)];
    }

    return "(unknown code)";
}

bool is_attributable (tree node) {
    switch (TREE_CODE(node)) {
        case RECORD_TYPE:
            return !TYPE_PTRMEMFUNC_P(node);
        case UNION_TYPE:    /* fall-through */
        case ENUMERAL_TYPE:
            return true;
        default:
            return false;
    }
}

void process_attributed_enum (tree type) {
    printf("processing attributed enum ...\n");

    printf("TYPE_SIZE<%s> TYPE_SIZE_UNIT<%s>\n", tree_code_string(TYPE_SIZE(type)), tree_code_string(TYPE_SIZE_UNIT(type)));
    printf("TYPE_ALIGN_UNIT<%d>\n", TYPE_ALIGN_UNIT(type));
    printf("ENUM_IS_SCOPED<%d> ENUM_IS_OPAQUE<%d>\n", ENUM_IS_SCOPED(type), ENUM_IS_OPAQUE(type));
    printf("%s:\n", IDENTIFIER_POINTER(DECL_NAME(TYPE_NAME(type))));

    FOREACH(x, TYPE_VALUES(type)) {
        /* enums have tree_lists */
        assert(TREE_LIST == TREE_CODE(x));

        tree valuenode = DECL_INITIAL(TREE_VALUE(x));
        int64_t value = TREE_INT_CST_HIGH(x) << 32;
        value |= TREE_INT_CST_LOW(x);
        printf("\t%s = %ld\n", IDENTIFIER_POINTER(TREE_PURPOSE(x)), value);
    }
}

void process_attributed_class (tree type) {
    printf("processing attributed class ...\n");

    printf("fields:\n");
    //for (tree x = TYPE_FIELDS(type); x; x = TREE_CHAIN(x)) {
    FOREACH(x, TYPE_FIELDS(type)) {
        printf("\t%s ", IDENTIFIER_POINTER(DECL_NAME(x)));
        if (TYPE_DECL == TREE_CODE(x) && DECL_SELF_REFERENCE_P(x)) {
            printf("(self-reference)");
        }
        //printf(" %s\n", get_type_identifier(x));
        printf("\n");
    }

    printf("methods:\n");
    //for (tree x = TYPE_METHODS(type); x; x = TREE_CHAIN(x)) {
    FOREACH(x, TYPE_METHODS(type)) {
        printf("\t%s\n", IDENTIFIER_POINTER(DECL_NAME(x)));
    }
}

tree handle_reflection_attr (tree* node, tree name, tree args, int flags, bool* no_add_attrs) {
    assert(!strcmp(IDENTIFIER_POINTER(name), attribute_name));

    if (!is_attributable(*node)) {
        *no_add_attrs = true;
        error("%qE attribute applies to struct, union, and enum types only", name);
        return NULL_TREE;
    }

#if 0
    if (ENUMERAL_TYPE == TREE_CODE(*node)) {
        process_attributed_enum(*node);
    }
#endif

#if 0
    printf("args:\n");
    for (tree x = args; x != NULL_TREE; x = TREE_CHAIN(x)) {
        printf("\t%s : %s\n", tree_code_string(TREE_PURPOSE(x)),
                tree_code_string(TREE_VALUE(x)));
    }

    printf("type name : %s\n", IDENTIFIER_POINTER(DECL_NAME(TYPE_NAME(*node))));

    printf("TYPE_FIELDS(*node) : %s\n", tree_code_string(TYPE_FIELDS(*node)));
    printf("flags<%08x>\n", flags);
#endif

    return NULL_TREE;
}

struct attribute_spec reflection_attr = {
    attribute_name,
    1, 1, // min length, max length
    false, false, false,
    handle_reflection_attr,
    false,
};

void register_attributes (void* event_data, void* data) {
    register_attribute(&reflection_attr);
}

void print_decl (tree decl) {
    int tc = TREE_CODE(decl);
    tree id = DECL_NAME(decl);
    const char* name = id ? IDENTIFIER_POINTER(id) : "<anonymous>";

    printf("%s %s at %s:%d\n", tree_code_name[tc], name,
            DECL_SOURCE_FILE(decl),
            DECL_SOURCE_LINE(decl));

    if (TYPE_DECL == tc) {
        tree type = TREE_TYPE(decl);
        if (ENUMERAL_TYPE == TREE_CODE(type)) {
            process_attributed_enum(type);
        }
        else {
            //process_attributed_class(type);
        }
    }
}

void traverse (tree ns) {
    cp_binding_level* level = NAMESPACE_LEVEL(ns);

    FOREACH(decl, level->names) {
        if (!DECL_IS_BUILTIN(decl)) {
            print_decl(decl);
        }
    }

    FOREACH(decl, level->namespaces) {
        if (!DECL_IS_BUILTIN(decl)) {
            print_decl(decl);
            traverse(decl);
        }
    }
}

void gate_callback (void* event_data, void* data) {
    if (errorcount || sorrycount) {
        return;
    }

    int ret = 0;

    traverse(global_namespace);

    if (no_asm) {
        exit(ret);
    }
}

void finish_type (void* event_data, void* data) {
    tree type_spec = (tree)event_data;

    if (!is_attributable(type_spec) ||
        !lookup_attribute(attribute_name, TYPE_ATTRIBUTES(type_spec))) {
        return;
    }

    process_attributed_class(type_spec);
}

} // file-scope namespace

//////////////////////////////////////////////////////////////////////////////

int plugin_is_GPL_compatible = 1;

int plugin_init (struct plugin_name_args* info, struct plugin_gcc_version* ver) {
    if (!plugin_default_version_check(ver, &gcc_version)) {
        return 1;
    }

    for (int i = 0; i < info->argc; ++i) {
        if (!strcmp(info->argv[i].key, "debug")) {
            debug = true;
            printf("(reflection) debugging on, bitch!\n");
        }
        else if (!strcmp(info->argv[i].key, "no-asm")) {
            no_asm = true;
            asm_file_name = HOST_BIT_BUCKET;
        }
    }

    register_callback(info->base_name, PLUGIN_INFO, NULL, &reflection_info);
    register_callback(info->base_name, PLUGIN_ATTRIBUTES, register_attributes, NULL);
    //register_callback(info->base_name, PLUGIN_FINISH_TYPE, finish_type, NULL);
    register_callback(info->base_name, PLUGIN_OVERRIDE_GATE, gate_callback, NULL);


    return 0;
}
