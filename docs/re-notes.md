# Notas de engenharia reversa: SCRANTIC.SCR

Cada achado precisa ter evidência: endereço (`seg:off`) ou offset no arquivo, pseudocódigo/assembly obtido via Ghidra/REA e a hipótese testada.

## Alvo

| Arquivo | Tamanho | MD5 | Observações |
|---|---|---|---|
| `SCRANTIC.SCR` | 295952 | `b2e9ccad1d46b31e532ebdb3887dfd02` | MZ + **NE** (Win16), Borland C++ 1991. Armazenado no instalador como `SCRANTIC.EXE` |
| `RESOURCE.MAP` | 1453 | `cf6b8e845a5235b2294fba0da1174f09` | 179 entradas (41 TTM, 116 BMP, 10 SCR, 10 ADS, 1 VIN, 1 PAL) |
| `RESOURCE.001` | 1175278 | `e71fadd320497026861c3f4a2a3332a2` | Validado: 100% dos 179 recursos decodificados com sucesso |

---

## Achados e Evidências

### F1: Tabela de caminhada idêntica
- **Offset:** `0x188ea`
- **Tamanho:** 489 linhas (2934 bytes)
- **Evidência:** `scripts/find-walk-table.mjs` confirmou 2934/2934 bytes idênticos ao `walk_data.h` do `jc_reborn`.

### F2: Carregamento do `RESOURCE.MAP`
- **Função:** `FUN_1018_1771` (no segmento `1018:1771`)
- **Evidência:** descompilada em `re/decompiled_functions.c`.
- **Comportamento:**
  - Abre `RESOURCE.MAP` via handle;
  - Lê 4 bytes de cabeçalho, depois 2 bytes de contagem de arquivos;
  - Para cada arquivo: lê 13 bytes de nome (`RESOURCE.001`), 2 bytes de número de entradas;
  - Aloca `(numEntries + 1) * 8` bytes de memória (`FUN_1030_199a`);
  - Lê pares de 4 bytes (tag ID) + 4 bytes (offset no `RESOURCE.001`).

### F3: Temporizador original e taxa de quadros (Tick Rate)
- **Funções:**
  - `FUN_1040_076a` (Init Timer): calcula `DAT_1068_45de = 1000 / (param_1 * 18)` (baseado no timer INT 08h do BIOS MS-DOS a **18.2 Hz**, ou seja, ~55ms por tick).
  - `FUN_1040_07e6` (Add Timer Slot): cadastra tarefas em 5 slots de timer, calculando `target = GetCurrentTime() + (ticks * interval)`.
  - `FUN_1040_089a` (Tick Dispatcher): disparado a cada `WM_TIMER` do Windows (`0x32` = 50ms = 20 fps no `SCREENSAVERPROC`), consultando `GetCurrentTime()` e chamando o callback do slot quando o tempo expira.
- **Impacto no Engine:** o `jc_reborn` usava `delay *= 20` ms presumido; a fórmula real do Screen Antics multiplica pelo quantum derivado da frequência de 18.2 Hz do PC.

### F4: Esquema original do `SCRANTIC.INI`
- **Função:** `FUN_1008_0c5c` (lê de `[ScreenSaver.ScreenAntics]`)
- **Evidência:** strings em `1068:0x17ecc` a `0x17fcf`.
- **Chaves suportadas pelo binário original:**
  1. `SourceDir`: diretório alternativo onde estão `RESOURCE.MAP` e `RESOURCE.001` (permite instalar os assets fora da pasta do executável).
  2. `Background`: ativa/desativa desenho da ilha (1/0).
  3. `Clouds`: ativa/desativa nuvens dinâmicas (1/0).
  4. `Waves`: ativa/desativa ondas do mar (1/0).
  5. `Sounds`: ativa/desativa efeitos sonoros (1/0).
  6. `Password`: hash de senha para proteção de tela.
  7. `NumDays`: contador persistente de dias em que Johnny está náufrago na ilha (incrementa conforme o tempo de uso).
  8. `Introduction`: flag que indica se a cena inicial de introdução já foi exibida.
  9. `CurrentYear`, `CurrentMonth`, `CurrentDay`: persistência da última data em que o screensaver rodou (usado para disparar feriados: Natal, Ano Novo, Halloween, etc.).
  10. `StartTime`: horário de início da sessão.

---

## Próximos passos de RE

- [x] Mapear carregador de `RESOURCE.MAP`
- [x] Mapear temporizador e taxa de quadros
- [x] Mapear esquema de configuração do `SCRANTIC.INI`
- [ ] Mapear algoritmo de sorteio de cenas e transição do náufrago (`story.c`)
- [ ] Verificar eventos sazonais de datas (feriados e aniversários)
