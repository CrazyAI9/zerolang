#include "program_graph_source_map.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char *path;
  size_t node_count;
} GraphSourceMapFile;

static bool source_map_text_eq(const char *left, const char *right) {
  return strcmp(left ? left : "", right ? right : "") == 0;
}

static uint64_t source_map_hash_text(const char *text) {
  uint64_t hash = 1469598103934665603ull;
  for (const unsigned char *cursor = (const unsigned char *)(text ? text : ""); *cursor; cursor++) {
    hash ^= (uint64_t)*cursor;
    hash *= 1099511628211ull;
  }
  return hash;
}

static size_t source_map_line_count(const char *text) {
  if (!text || !text[0]) return 0;
  size_t lines = 1;
  for (const char *cursor = text; *cursor; cursor++) {
    if (*cursor == '\n' && cursor[1]) lines++;
  }
  return lines;
}

static void source_map_json_string(ZBuf *buf, const char *value) {
  zbuf_append_char(buf, '"');
  for (const unsigned char *cursor = (const unsigned char *)(value ? value : ""); *cursor; cursor++) {
    unsigned char ch = *cursor;
    switch (ch) {
      case '"': zbuf_append(buf, "\\\""); break;
      case '\\': zbuf_append(buf, "\\\\"); break;
      case '\n': zbuf_append(buf, "\\n"); break;
      case '\r': zbuf_append(buf, "\\r"); break;
      case '\t': zbuf_append(buf, "\\t"); break;
      default:
        if (ch < 0x20) zbuf_appendf(buf, "\\u%04x", (unsigned)ch);
        else zbuf_append_char(buf, (char)ch);
        break;
    }
  }
  zbuf_append_char(buf, '"');
}

static int source_map_start_line(const ZProgramGraphNode *node) {
  return node && node->line > 0 ? node->line : 1;
}

static int source_map_start_column(const ZProgramGraphNode *node) {
  return node && node->column > 0 ? node->column : 1;
}

static int source_map_range_length(const ZProgramGraphNode *node) {
  const char *payload = NULL;
  if (node) {
    if (node->name && node->name[0]) payload = node->name;
    else if (node->value && node->value[0]) payload = node->value;
    else if (node->type && node->type[0]) payload = node->type;
    else payload = z_program_graph_node_kind_name(node->kind);
  }
  size_t len = strlen(payload ? payload : "");
  if (len == 0) len = 1;
  if (len > 120) len = 120;
  return (int)len;
}

static void source_map_append_range_json(ZBuf *buf, const ZProgramGraphNode *node, const char *fallback_path) {
  const char *path = node && node->path && node->path[0] ? node->path : (fallback_path ? fallback_path : "");
  int start_line = source_map_start_line(node);
  int start_column = source_map_start_column(node);
  int end_column = start_column + source_map_range_length(node);
  zbuf_append(buf, "{\"path\":");
  source_map_json_string(buf, path);
  zbuf_appendf(buf,
               ",\"start\":{\"line\":%d,\"column\":%d},\"end\":{\"line\":%d,\"column\":%d},\"columnUnit\":\"utf8-byte\"}",
               start_line,
               start_column,
               start_line,
               end_column);
}

static size_t source_map_file_index(GraphSourceMapFile *files, size_t len, const char *path) {
  for (size_t i = 0; i < len; i++) {
    if (source_map_text_eq(files[i].path, path)) return i;
  }
  return (size_t)-1;
}

static void source_map_collect_file(GraphSourceMapFile **files, size_t *len, size_t *cap, const char *path) {
  if (!path || !path[0]) return;
  size_t index = source_map_file_index(*files, *len, path);
  if (index != (size_t)-1) {
    (*files)[index].node_count++;
    return;
  }
  if (*len == *cap) {
    size_t next = *cap ? *cap * 2 : 4;
    *files = z_checked_reallocarray(*files, next, sizeof(GraphSourceMapFile));
    for (size_t i = *cap; i < next; i++) (*files)[i] = (GraphSourceMapFile){0};
    *cap = next;
  }
  (*files)[*len].path = z_strdup(path);
  (*files)[*len].node_count = 1;
  (*len)++;
}

static void source_map_append_file_json(ZBuf *buf, const GraphSourceMapFile *file) {
  char *text = NULL;
  ZDiag diag = {0};
  if (file && file->path && file->path[0]) text = z_read_file(file->path, &diag);
  zbuf_append(buf, "{\"path\":");
  source_map_json_string(buf, file ? file->path : "");
  zbuf_appendf(buf,
               ",\"sourceHash\":\"%016llx\",\"lineCount\":%zu,\"nodeCount\":%zu,\"columnUnit\":\"utf8-byte\",\"readable\":%s}",
               (unsigned long long)source_map_hash_text(text ? text : ""),
               source_map_line_count(text ? text : ""),
               file ? file->node_count : 0,
               text ? "true" : "false");
  free(text);
}

static void source_map_append_node_json(ZBuf *buf, const ZProgramGraphNode *node, const char *input_path) {
  zbuf_append(buf, "{\"nodeId\":");
  source_map_json_string(buf, node ? node->id : "");
  zbuf_append(buf, ",\"kind\":");
  source_map_json_string(buf, node ? z_program_graph_node_kind_name(node->kind) : "");
  zbuf_append(buf, ",\"name\":");
  source_map_json_string(buf, node ? node->name : "");
  zbuf_append(buf, ",\"type\":");
  source_map_json_string(buf, node ? node->type : "");
  zbuf_append(buf, ",\"value\":");
  source_map_json_string(buf, node ? node->value : "");
  zbuf_append(buf, ",\"nodeHash\":");
  source_map_json_string(buf, node ? node->node_hash : "");
  zbuf_append(buf, ",\"symbolId\":");
  source_map_json_string(buf, node ? node->symbol_id : "");
  zbuf_append(buf, ",\"typeId\":");
  source_map_json_string(buf, node ? node->type_id : "");
  zbuf_append(buf, ",\"effectId\":");
  source_map_json_string(buf, node ? node->effect_id : "");
  zbuf_append(buf, ",\"sourceAvailable\":");
  zbuf_append(buf, node && node->path && node->path[0] ? "true" : "false");
  zbuf_append(buf, ",\"sourceRange\":");
  source_map_append_range_json(buf, node, node && node->path && node->path[0] ? node->path : input_path);
  zbuf_append(buf, "}");
}

size_t z_program_graph_source_map_count(const ZProgramGraph *graph) {
  return graph ? graph->node_len : 0;
}

void z_program_graph_append_source_map_json(ZBuf *buf, const ZProgramGraph *graph, const char *input_path) {
  GraphSourceMapFile *files = NULL;
  size_t file_len = 0;
  size_t file_cap = 0;
  for (size_t i = 0; graph && i < graph->node_len; i++) {
    source_map_collect_file(&files, &file_len, &file_cap, graph->nodes[i].path);
  }

  zbuf_append(buf, "{\n  \"schemaVersion\": 1,\n  \"ok\": true,\n  \"artifact\": ");
  source_map_json_string(buf, input_path ? input_path : "");
  zbuf_appendf(buf, ",\n  \"canonicalSource\": %s,\n  \"moduleIdentity\": ", graph && graph->canonical_source ? "true" : "false");
  source_map_json_string(buf, graph ? graph->module_identity : "");
  zbuf_append(buf, ",\n  \"graphHash\": ");
  source_map_json_string(buf, graph ? graph->graph_hash : "");
  zbuf_appendf(buf, ",\n  \"counts\": {\"files\": %zu, \"mappings\": %zu},\n  \"files\": [", file_len, graph ? graph->node_len : 0);
  for (size_t i = 0; i < file_len; i++) {
    if (i > 0) zbuf_append(buf, ", ");
    source_map_append_file_json(buf, &files[i]);
  }
  zbuf_append(buf, "],\n  \"mappings\": [");
  for (size_t i = 0; graph && i < graph->node_len; i++) {
    if (i > 0) zbuf_append(buf, ", ");
    source_map_append_node_json(buf, &graph->nodes[i], input_path);
  }
  zbuf_append(buf, "],\n  \"diagnostics\": []\n}\n");

  for (size_t i = 0; i < file_len; i++) free(files[i].path);
  free(files);
}
