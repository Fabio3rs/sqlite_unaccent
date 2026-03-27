// unaccent_lower_utf8proc.cpp
extern "C" {
#include "sqlite3ext.h"
SQLITE_EXTENSION_INIT1
}

extern "C" {
#include <utf8proc.h>
}

#include "stdafx.hpp"

static void unaccent_lower_sqlite(sqlite3_context *ctx, int argc,
                                  sqlite3_value **argv) {
    if (argc != 1 || sqlite3_value_type(argv[0]) == SQLITE_NULL) {
        sqlite3_result_null(ctx);
        return;
    }
    const unsigned char *in = sqlite3_value_text(argv[0]);
    int inlen = sqlite3_value_bytes(argv[0]);
    try {
        auto [view, memown] = unaccent_lower_impl(in, inlen);
        sqlite3_result_text(ctx, view.data(), static_cast<int>(view.size()),
                            SQLITE_TRANSIENT);
    } catch (const std::exception &e) {
        sqlite3_result_error(ctx, e.what(), -1); // <— mostra a msg
    } catch (...) {
        sqlite3_result_error(ctx, "unaccent_lower: unknown error", -1);
    }
}

extern "C"
#ifdef _WIN32
    __declspec(dllexport)
#endif
    int
    sqlite3_unaccentlower_init(sqlite3 *db, char **pzErrMsg,
                               const sqlite3_api_routines *pApi);

extern "C"
#ifdef _WIN32
    __declspec(dllexport)
#endif
    int
    sqlite3_unaccentlower_init(sqlite3 *db, char **pzErrMsg,
                               const sqlite3_api_routines *pApi) {
    SQLITE_EXTENSION_INIT2(pApi)
    int flags = SQLITE_UTF8 | SQLITE_DETERMINISTIC;
    if (sqlite3_create_function(db, "UNACCENT_LOWER", 1, flags, nullptr,
                                &unaccent_lower_sqlite, nullptr,
                                nullptr) != SQLITE_OK) {
        if (pzErrMsg)
            *pzErrMsg =
                sqlite3_mprintf("failed to create function UNACCENT_LOWER");
        return SQLITE_ERROR;
    }
    return SQLITE_OK;
}
