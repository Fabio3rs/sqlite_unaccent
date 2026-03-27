# sqlite_unnacent_lower


**OBSERVAÇÃO**
Esta extensão foi feita para uso pessoal e pode não ser adequada para produção.


Uma extensão para SQLite que fornece a função `UNACCENT_LOWER(text)`, permitindo remover acentos (diacríticos) e converter textos para minúsculas utilizando regras de *Unicode Case Folding*.

Esta extensão é ideal para buscas textuais onde a diferenciação de acentos e capitalização não é desejada, garantindo alta compatibilidade com caracteres internacionais (UTF-8).

## Características

- **Remoção de Acentos:** Remove diacríticos de caracteres como `á`, `é`, `ç`, `ñ`, etc.
- **Case Folding:** Converte para minúsculas seguindo o padrão Unicode (ex: `ß` torna-se `ss`).
- **Preservação de Símbolos:** Mantém caracteres que não possuem representação sem acento (ex: emojis, letras gregas como `π`).
- **Suporte a Combining Marks:** Lida corretamente com acentos representados como caracteres combinantes (U+0301, etc).
- **Integração com SQLite:** Pode ser utilizada em consultas `SELECT`, cláusulas `WHERE` e até na criação de índices baseados em expressões.

## Dependências

- **CMake** (3.12 ou superior)
- **Compilador C++23** (GCC, Clang ou MSVC)
- **utf8proc** (incluído no diretório `vendor/`)
- **SQLite3** (headers de desenvolvimento)
- **GTest** (opcional, para execução dos testes)

## Como Compilar

Para compilar a extensão e gerar a biblioteca compartilhada (`.so`, `.dylib` ou `.dll`):

```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
```

O arquivo da extensão será gerado como `libunaccent_lower.so` (no Linux).

## Como Usar no SQLite

### Carregando a Extensão

No console do SQLite:

```sql
.load ./libunaccent_lower.so
```

Ou via API C/C++:

```c
sqlite3_load_extension(db, "./libunaccent_lower.so", NULL, &err);
```

### Exemplos de Uso

```sql
-- Consulta simples
SELECT UNACCENT_LOWER('Olá, Açúcar! ÉÇÃO — Straße, Café, π');
-- Resultado: 'ola, acucar! ecao — strasse, cafe, π'

-- Uso em filtros
SELECT * FROM usuarios
WHERE UNACCENT_LOWER(nome) = UNACCENT_LOWER('João');

-- Criação de índice para buscas rápidas
CREATE INDEX idx_usuario_nome_raw ON usuarios(UNACCENT_LOWER(nome));
```

## Testes

Para compilar e rodar os testes unitários e de integração:

```bash
cd build
cmake .. -DENABLE_TESTS=ON
make
ctest --output-on-failure
```

Os testes cobrem:
- Casos básicos de acentuação.
- Regras especiais como o Eszett alemão (`ß`).
- Tratamento de emojis e caracteres especiais.
- Integração real com banco de dados SQLite em memória.

## Licença

Este projeto utiliza a biblioteca `utf8proc`. Verifique os respectivos arquivos de licença no diretório `vendor/`.
