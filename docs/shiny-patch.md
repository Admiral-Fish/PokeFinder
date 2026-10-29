# DPPt Shiny Patch

Open **Gen 4 Tools → Shiny Patch (DPPt)**. A Gen 4 wild-search result also has
**Find Shiny Patches (DPPt)** in its context menu, which copies the initial seed
and uses the encounter's advances as the end of the search range.

1. Enter the initial seed and an inclusive start/end advance range. An advance
   is one main LCRNG call, not a video frame. The activation advance is the
   state **before** the radar draws the first patch position. To check a current
   RNG state directly, enter that state as the seed and use advances 0–0.
2. Enter the current chain count. Zero cannot produce a shiny patch; the odds
   stop improving at 40. Counts through the in-game cap of 999 are accepted.
3. Select manual reset / after knockout, or immediately after capture. The
   capture bonus applies only to automatic post-capture patches. Resetting the
   radar manually uses normal rates even if the previous battle was a capture.
4. Edit the 9×9 terrain grid around the player. Pressed green tiles mean eligible
   tall grass at the player's elevation and in the same map. The default assumes
   a completely valid grass area; deselect ineligible tiles before relying on
   results. The center is the player. Positive X is right and positive Y is down.
5. Generate. Results show the activation advance and state, ring (4 is outermost),
   relative coordinates, continuation roll, raw shiny roll and the absolute
   advance of the shiny check. Uncheck **Show shiny patches only** to inspect all
   four rings, including invalid tiles and failed chain checks. A dash means no
   such RNG check was made. Results can be copied or exported via the table menu.

Generation can be cancelled. To bound memory use, it stops at 100,000 displayed
rows with an explicit notice; narrow the range to continue. The range includes
its end even at `4294967295`, and check-advance columns use 64-bit values.

This tool predicts grass when the radar activates. The existing **Poke Radar
Shiny** encounter generator/searcher predicts the Pokémon obtained by entering
an already shiny patch. Those are separate RNG stages. The new tool does not
claim that a shiny grass result will produce any previously searched IV/PID
spread. NPC advances, walking to a patch, and battle startup still need to be
accounted for. Terrain is supplied by the user; it is not read from a game or
automatically inferred from encounter tables.

## Algorithm and sources

The shared D/P/Pt calculation was checked against these pinned game sources:

- [Platinum radar routines](https://github.com/pret/pokeplatinum/blob/f78e5012b9de85fe62997766ab02862c0aeadccf/src/pokeradar.c):
  `RadarSpawnPatches`, `SetupGrassPatches`, `CheckTileIsGrass`,
  `CheckPatchContinueChain`, `CheckPatchShiny`, and `RefreshRadarChain`.
- [Platinum random-number scaling](https://github.com/pret/pokeplatinum/blob/f78e5012b9de85fe62997766ab02862c0aeadccf/include/inlines.h):
  `LCRNG_RandMod` uses integer division despite its name.
- [Diamond/Pearl radar assembly](https://github.com/pret/pokediamond/blob/b623abef1b4dfe204ca50aab42447ac00c134290/arm9/asm/unk_0205DD18.s):
  `sub_0205DD80` (positions), `sub_0205DED4` (patch setup), `sub_0205E12C`
  (terrain), `sub_0205E2A0` (continuation), `sub_0205E30C` (manual reset),
  `sub_0205E40C` (shiny check), and the `UNK_020F7374` rate table.

The implementation is a standalone simulation of these mechanics:

- Draw all four positions, outermost first, from rings of 32, 24, 16 and 8 tiles.
- Skip subsequent checks entirely for invalid tiles.
- For each active ring, draw the continuation roll as `high16 / 656`. It must
  be strictly below 88/68/48/28 (98/78/58/38 after a capture), outermost first.
- A failed continuation consumes one further call to select the shake type.
  That random value cannot make the patch shiny.
- A successful continuation with chain zero consumes no shiny call. Otherwise,
  draw the shiny roll and test
  `high16 < floor(65535 / max(200, 8200 - 200 * chain)) + 1`.
- Continue checking inner rings even when an outer ring fails or is shiny.

There is no additional version offset inside this routine: the corresponding
DP assembly and Platinum source agree on the operations and order. This is
source-level verification, not a claim of emulator or hardware testing.

## Regression examples

For initial seed `1D0B3634`, activation advance **312**, chain 1, the state is
`D726AFEC`. The first continuation roll is 98, which fails both the normal and
capture-boosted outer-ring rates. The subsequent raw `0006` is a shake-type
draw. No shiny patch exists for any of the 16 active-ring combinations under
either activation type. With all grass valid, the continuation rolls are
98, 74, 80 and 52.

For initial seed `A01513D6`, activation advance **466**, chain 1, normal rates,
the state is `11BAE100`. The outer patch is at **(+3, +4)**, continuation roll
84, and raw shiny roll 8 at advance **472**. It is shiny only if that tile is
eligible. This reproduces the candidate discussed in the supplied chat; it
remains a calculated result rather than a hardware-confirmed observation.

The Qt test suite also covers inner-ring shinies after outer-ring failure,
capture bonuses, skipped invalid rings, chain-zero call consumption, strict
shiny thresholds, the cap above chain 40, and all 80 ring positions.

The separate `ShinyPatchForm` CTest runs with Qt's offscreen platform plugin.
It exercises both reported examples through the form, terrain toggling,
detailed results, the inclusive maximum-u32 advance, cancellation and the
100,000-row limit without requiring a display server.
