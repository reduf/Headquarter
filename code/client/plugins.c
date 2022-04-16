#ifdef PLUGINS_C_INC
#error "plugins.c included more than once"
#endif
#define PLUGINS_C_INC

#define PLUGIN_MAX_PATH (256)

struct list   plugins = LIST_INITIALIZER(plugins);
static size_t plugins_count = 0;
static Plugin plugins_table[PLUGINS_MAX * 2];
static size_t array_find_plugin(Plugin *table, size_t size, void *handle);

bool plugin_load(const char *path)
{
    if (plugins_count >= PLUGINS_MAX) {
        LogError("Can't load more plugin, the maximum '%u' is reached", PLUGINS_MAX);
        return false;
    }

    size_t plugin_id = array_find_plugin(plugins_table, PLUGINS_MAX, NULL);
    assert(plugin_id != PLUGIN_INVALID_ID);

    char temp_path[PLUGIN_MAX_PATH];
    Plugin *plugin = &plugins_table[plugin_id];
    memset(plugin, 0, sizeof(*plugin));

    plugin->plugin_id = plugin_id;

#if defined(OS_WINDOWS) && defined(NDEBUG)
    char temp_dir[PLUGIN_MAX_PATH];

    if (GetTempPathA(PLUGIN_MAX_PATH, temp_dir) == 0) {
        LogError("Couldn't get temporary directory (%d)", os_errno);
        free(plugin);
        return false;
    }

    if (GetTempFileNameA(temp_dir, "cc", 0, temp_path) == 0) {
        LogError("Couldn't create a temporary file name (%d)", os_errno);
        free(plugin);
        return false;
    }

    if (!CopyFileA(path, temp_path, FALSE)) {
        LogError("Couldn't duplicate the file '%s' into '%s' (%d)",
            path, temp_path, os_errno);
        free(plugin);
        return false;
    }
#else
    safe_strcpy(temp_path, PLUGIN_MAX_PATH, path);
#endif

    plugin->module = dllopen(temp_path);
    if (plugin->module == NULL) {
        LogError("Couldn't dllopen the plugin '%s'", temp_path);
        return false;
    }

    plugin->path = path;

    PluginEntry_pt PluginInit;
    *(void **)&PluginInit = dllsym(plugin->module, "PluginEntry");
    if (PluginInit == NULL) {
        LogError("Couldn't dllsym plugin '%s'", path);
        plugin_unload(plugin);
        return false;
    }

    plugin->plugin_object.module = plugin->module;
    plugin->plugin_object.PluginInit = PluginInit;

    list_node_init(&plugin->entry);
    list_insert_tail(&plugins, &plugin->entry);

    if (!PluginInit(&plugin->plugin_object)) {
        plugin_unload(plugin);
        return false;
    }

    plugins_count++;
    plugin->loaded = true;
    return true;
}

void plugin_unload(Plugin *plugin)
{
    if (!plugin) return;
    if (plugin->loaded && plugin->plugin_object.PluginUnload)
        plugin->plugin_object.PluginUnload(&plugin->plugin_object);
    if (!list_node_unlinked(&plugin->entry))
        list_remove(&plugin->entry);
    if (plugin->module) {
        dllclose(plugin->module);
        plugin->module = NULL;
    }
    plugin->plugin_id = 0;
    plugins_count--;
}

static size_t array_find_plugin(Plugin *table, size_t size, void *handle)
{
    size_t try_count = 0;
    uint32_t hash  = hash_ptr(handle) & 0xFFFFFFFF;
    uint32_t index = hash % size;
    while (table[index].module && (table[index].module != handle)) {
        if (try_count++ >= PLUGINS_MAX)
            return PLUGIN_INVALID_ID;
        hash = hash_int32(hash);
        index = hash % size;
    }
    return index;
}
