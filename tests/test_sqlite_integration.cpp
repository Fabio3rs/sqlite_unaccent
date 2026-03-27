#include <gtest/gtest.h>
#include <sqlite3.h>
#include <string>

#ifndef UNACCENT_EXT_PATH
#  error "Defina UNACCENT_EXT_PATH no CMake apontando para a .so/.dylib/.dll"
#endif

class SqliteMem : public ::testing::Test {
protected:
    sqlite3* db = nullptr;

    void SetUp() override {
        // 1) abre banco em memória
        ASSERT_EQ(SQLITE_OK, sqlite3_open(":memory:", &db)) << sqlite3_errmsg(db);

        // 2) habilita carregamento de extensões
        //    (você pode usar sqlite3_enable_load_extension(db,1) ou db_config)
        int rc = sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_LOAD_EXTENSION, 1, nullptr);
        ASSERT_EQ(SQLITE_OK, rc) << sqlite3_errmsg(db);

        // 3) carrega a extensão
        char* err = nullptr;
        rc = sqlite3_load_extension(db, UNACCENT_EXT_PATH, nullptr, &err);
        if (rc != SQLITE_OK) {
            std::string msg = err ? err : "(no message)";
            sqlite3_free(err);
            FAIL() << "load_extension failed: " << msg;
        }
    }

    void TearDown() override {
        if (db) sqlite3_close(db);
    }

    // helper: executa 1 SELECT que retorna 1 coluna/1 linha como string
    std::string scalarText(const char* sql) {
        sqlite3_stmt* st = nullptr;
        int rc = sqlite3_prepare_v2(db, sql, -1, &st, nullptr);
        if (rc != SQLITE_OK) throw std::runtime_error(sqlite3_errmsg(db));
        rc = sqlite3_step(st);
        std::string out;
        if (rc == SQLITE_ROW) {
            const unsigned char* txt = sqlite3_column_text(st, 0);
            if (txt) out.assign(reinterpret_cast<const char*>(txt));
        }
        sqlite3_finalize(st);
        return out;
    }
};

TEST_F(SqliteMem, UnaccentLower_Works) {
    auto res = scalarText("SELECT UNACCENT_LOWER('Olá, Açúcar! ÉÇÃO — Straße, Café, π');");
    EXPECT_EQ(res, "ola, acucar! ecao — strasse, cafe, π");
}

TEST_F(SqliteMem, IndexOnExpression_Demo) {
    // só pra garantir que a função roda dentro de DDL/queries normais
    ASSERT_NO_FATAL_FAILURE(scalarText("CREATE TABLE t(x TEXT);"));
    ASSERT_NO_FATAL_FAILURE(scalarText("CREATE INDEX idx ON t(UNACCENT_LOWER(x));"));
    ASSERT_NO_FATAL_FAILURE(scalarText("INSERT INTO t VALUES('Coração'),('coracao'),('CORACAO');"));
    auto res = scalarText(
        "SELECT group_concat(x,'|') "
        "FROM t WHERE UNACCENT_LOWER(x) = UNACCENT_LOWER('coração');");
    EXPECT_EQ(res, "Coração|coracao|CORACAO");
}
