// unaccent_lower_utf8proc.cpp
#include "unnacent_sqlite.hpp"

extern "C" {
SQLITE_EXTENSION_INIT1
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
        if (pzErrMsg) {
            *pzErrMsg =
                sqlite3_mprintf("failed to create function UNACCENT_LOWER");
        }
        return SQLITE_ERROR;
    }

    if (sqlite3_create_function(db, "UNACCENT", 1, flags, nullptr,
                                &unaccent_sqlite, nullptr,
                                nullptr) != SQLITE_OK) {
        if (pzErrMsg) {
            *pzErrMsg = sqlite3_mprintf("failed to create function UNACCENT");
        }
        return SQLITE_ERROR;
    }
    return SQLITE_OK;
}
