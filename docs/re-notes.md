# Notas de engenharia reversa: SCRANTIC.SCR

Cada achado precisa ter evidência: endereço (`seg:off`) ou offset no arquivo, pseudocódigo/assembly obtido via REA/Ghidra e a hipótese testada.

## Alvo

| Arquivo | Tamanho | MD5 | Observações |
|---|---|---|---|
| `SCRANTIC.SCR` | 295952 | `b2e9ccad1d46b31e532ebdb3887dfd02` | MZ + **NE** (Win16), Borland C++ 1991. Armazenado no instalador como `SCRANTIC.EXE` |
| `RESOURCE.MAP` | 1453 | `cf6b8e845a5235b2294fba0da1174f09` | 179 entradas (41 TTM, 116 BMP, 10 SCR, 10 ADS, 1 VIN, 1 PAL) |
| `RESOURCE.001` | 1175278 | `e71fadd320497026861c3f4a2a3332a2` | ≠ da versão de referência do jc_reborn (1175645 / `374e6d05…`) |

Origem: instalador de 1992 (InstallShield 3 + PKWARE DCL implode), extraído com `scripts/unpack-installer.mjs`.

## Achados

### F1: a tabela de caminhada é idêntica à do jc_reborn
- `scripts/find-walk-table.mjs engine/walk_data.h SCRANTIC.SCR` → offset de arquivo `0x188ea`, 489 linhas, 2934/2934 bytes idênticos.
- Conclusão: o executável é o mesmo build (ou equivalente) usado pelo jc_reborn. Só o `RESOURCE.001` difere e precisa de validação (ver Q1).

## Perguntas em aberto (backlog de RE)

- **Q1:** onde o `RESOURCE.001` desta versão difere do de referência? Fazer diff por recurso (nome, tamanho, conteúdo descomprimido).
- **Q2:** o NE tem recursos WAVE embutidos? O jc_reborn pega os `soundN.wav` do JCOS.
- **Q3:** algoritmo de escolha de cenas (`story.c`): pesos, histórico, regra de "FINAL"/"FIRST", feriados (datas → Halloween, Natal, etc.).
- **Q4:** temporização: unidade de `SET_DELAY` (o jc_reborn assume 20 ms por tick), ritmo do `UPDATE`.
- **Q5:** opcodes TTM/ADS que o jc_reborn marca como desconhecidos/TODO (`grep -n TODO engine/ttm.c engine/ads.c`).
- **Q6:** RNG original (Borland `rand()` LCG: `seed = seed*0x015A4E35 + 1`) e de onde vem a semente.
- **Q7:** posicionamento da ilha, nuvens, maré e noite/dia (`island.c`).
- **Q8:** configurações do `ScrAntic.ini` e diálogo de configuração (`SCREENSAVERCONFIGUREDIALOG`).

## Fluxo de trabalho com REA

```bash
docker compose exec jc-dev bash
rea doctor --json          # valida GHIDRA_INSTALL_DIR / JAVA_HOME
rea providers --json
# depois, via agente (MCP): abrir /assets/SCRANTIC.SCR com o provider Ghidra
```

> Atenção: segundo a documentação do REA, assinaturas NE são *reconhecidas*, mas o suporte ao layout depende do provider. Se o REA recusar o NE, a saída é usar Ghidra headless (`$GHIDRA_INSTALL_DIR/support/analyzeHeadless`) com scripts próprios em `re/ghidra-scripts/` e registrar a limitação aqui.
