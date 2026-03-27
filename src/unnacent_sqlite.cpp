#include "unnacent_sqlite.hpp"

extern "C" {
#include <utf8proc.h>
}

#include "stdafx.hpp"

extern "C" void unaccent_lower_sqlite(sqlite3_context *ctx, int argc,
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

extern "C" void unaccent_sqlite(sqlite3_context *ctx, int argc,
                                sqlite3_value **argv) {
    if (argc != 1 || sqlite3_value_type(argv[0]) == SQLITE_NULL) {
        sqlite3_result_null(ctx);
        return;
    }
    const unsigned char *in = sqlite3_value_text(argv[0]);
    int inlen = sqlite3_value_bytes(argv[0]);
    try {
        auto [view, memown] = unaccent_impl(in, inlen);
        sqlite3_result_text(ctx, view.data(), static_cast<int>(view.size()),
                            SQLITE_TRANSIENT);
    } catch (const std::exception &e) {
        sqlite3_result_error(ctx, e.what(), -1); // <— mostra a msg
    } catch (...) {
        sqlite3_result_error(ctx, "unaccent_lower: unknown error", -1);
    }
}
