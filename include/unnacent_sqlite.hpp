#pragma once

extern "C" {
#include "sqlite3ext.h"
SQLITE_EXTENSION_INIT3
}

extern "C" void unaccent_lower_sqlite(sqlite3_context *ctx, int argc,
                                      sqlite3_value **argv);

extern "C" void unaccent_sqlite(sqlite3_context *ctx, int argc,
                                sqlite3_value **argv);
