#ifdef PLUGINS_H_INC
#error "plugins.h included more than once"
#endif
#define PLUGINS_H_INC

typedef struct Plugin {
    struct list     entry;

    size_t          plugin_id;
    void           *module;
    bool            loaded;

    const char     *path;
    PluginObject    plugin_object;
} Plugin;

#define PLUGINS_MAX         (32)
#define PLUGIN_INVALID_ID   ((uint32_t)-1)

bool plugin_load(const char *path);
void plugin_unload(Plugin *plugin);

extern struct list plugins;

#define plugin_first() list_first_entry(&plugins, Plugin, entry)
#define plugin_end(p)  list_end(&plugins, &(p)->entry)
#define plugin_next(p) list_entry(list_next(&(p)->entry), Plugin, entry)

#define plugin_foreach(it)          \
    for (it = plugin_first();       \
        !plugin_end(it);            \
        it = plugin_next(it))
