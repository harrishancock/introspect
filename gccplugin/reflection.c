#include <gcc-plugin.h>

#include <cp/cp-tree.h>

#include <diagnostic-core.h>
#include <tree.h>
#include <plugin.h>
#include <plugin-version.h>

#include <assert.h>
#include <stdio.h>

static const char* const plugin_name = "reflection";
static const char* const attribute_name = plugin_name;

int plugin_is_GPL_compatible = 1;

static struct plugin_info reflection_info = {
    .version = "0.0.1",
    .help = "Good luck!",
};

static const char* tree_code_string (tree t) {
    if (!t) {
        return "(null tree)";
    }

    if (TREE_CODE(t) >= 0 && TREE_CODE(t) < MAX_TREE_CODES) {
        return tree_code_name[TREE_CODE(t)];
    }

    return "(unknown code)";
}

static bool is_attributable (tree node) {
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

static void process_attributed_enum (tree type) {
    printf("processing attributed enum ...\n");

    for (tree x = TYPE_VALUES(type); x; x = TREE_CHAIN(x)) {
        /* enums have tree_lists */
        assert(TREE_LIST == TREE_CODE(x));

        printf("\t%s = %s\n", IDENTIFIER_POINTER(TREE_PURPOSE(x)), tree_code_string(TREE_VALUE(x)));
    }
}

static void process_attributed_class (tree type) {
    printf("processing attributed class ...\n");

    printf("fields:\n");
    for (tree x = TYPE_FIELDS(type); x; x = TREE_CHAIN(x)) {
        printf("\t%s ", IDENTIFIER_POINTER(DECL_NAME(x)));
        if (TYPE_DECL == TREE_CODE(x) && DECL_SELF_REFERENCE_P(x)) {
            printf("(self-reference)");
        }
        printf("\n");
    }

    printf("methods:\n");
    for (tree x = TYPE_METHODS(type); x; x = TREE_CHAIN(x)) {
        printf("\t%s\n", IDENTIFIER_POINTER(DECL_NAME(x)));
    }
}

static tree handle_reflection_attr (tree* node, tree name, tree args, int flags, bool* no_add_attrs) {
    assert(!strcmp(IDENTIFIER_POINTER(name), attribute_name));

    if (!is_attributable(*node)) {
        *no_add_attrs = true;
        error("%qE attribute applies to struct, union, and enum types only", name);
        return NULL_TREE;
    }

    if (ENUMERAL_TYPE == TREE_CODE(*node)) {
        process_attributed_enum(*node);
    }

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

static struct attribute_spec reflection_attr = {
    attribute_name,
    1, 1, // min length, max length
    false, false, false,
    handle_reflection_attr,
    false,
};

static void register_attributes (void* event_data, void* data) {
    register_attribute(&reflection_attr);
}

static void finish_type (void* event_data, void* data) {
    tree type_spec = (tree)event_data;

    if (!is_attributable(type_spec) ||
        !lookup_attribute(attribute_name, TYPE_ATTRIBUTES(type_spec))) {
        return;
    }

    process_attributed_class(type_spec);
#if 0
    printf("TYPE_FIELDS(type_spec) : %s\n", tree_code_string(TYPE_FIELDS(type_spec)));
#endif
}

int plugin_init (struct plugin_name_args* info, struct plugin_gcc_version* ver) {
    if (!plugin_default_version_check(ver, &gcc_version)) {
        return 1;
    }

    register_callback(plugin_name, PLUGIN_INFO, NULL, &reflection_info);
    register_callback(plugin_name, PLUGIN_ATTRIBUTES, register_attributes, NULL);
    register_callback(plugin_name, PLUGIN_FINISH_TYPE, finish_type, NULL);

    return 0;
}
