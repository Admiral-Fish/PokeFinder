/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2022 by Admiral_Fish, bumba, and EzPzStreamz
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "Encounters3.hpp"
#include <Core/Enum/Encounter.hpp>
#include <Core/Enum/Game.hpp>
#include <Core/Enum/ShadowType.hpp>
#include <Core/Gen3/EncounterArea3.hpp>
#include <Core/Gen3/LockInfo.hpp>
#include <Core/Gen3/Profile3.hpp>
#include <Core/Gen3/ShadowTemplate.hpp>
#include <Core/Parents/PersonalLoader.hpp>
#include <Core/Parents/Slot.hpp>
#include <Core/Parents/StaticTemplate.hpp>
#include <Core/Resources/Encounters.hpp>
#include <Core/Util/Utilities.hpp>
#include <algorithm>
#include <cstring>
#include <iterator>

constexpr std::array<StaticTemplate, 9> starters = {
    StaticTemplate(Game::Emerald, 152, 5), // Chikorita @ Littleroot Town
    StaticTemplate(Game::Emerald, 155, 5), // Cyndaquil
    StaticTemplate(Game::Emerald, 158, 5), // Totodile

    StaticTemplate(Game::RSE, 252, 5), // Treecko @ Route 101
    StaticTemplate(Game::RSE, 255, 5), // Torchic
    StaticTemplate(Game::RSE, 258, 5), // Mudkip

    StaticTemplate(Game::FRLG, 1, 5), // Bulbasaur @ Pallet Town
    StaticTemplate(Game::FRLG, 4, 5), // Squirtle
    StaticTemplate(Game::FRLG, 7, 5), // Charmander
};

constexpr std::array<StaticTemplate, 5> fossils = {
    StaticTemplate(Game::RSE, 345, 20), // Lileep @ Rustboro City
    StaticTemplate(Game::RSE, 347, 20), // Anorith

    StaticTemplate(Game::FRLG, 138, 5), // Omanyte @ Cinnabar Islands
    StaticTemplate(Game::FRLG, 140, 5), // Kabuto
    StaticTemplate(Game::FRLG, 142, 5) // Aerodactyl
};

constexpr std::array<StaticTemplate, 9> gifts = {
    StaticTemplate(Game::RSE, 351, 25), // Castform @ Weather Institute
    StaticTemplate(Game::RSE, 374, 5), // Beldum @ Mossdeep City
    StaticTemplate(Game::RSE, 360, 5), // Wynaut Egg

    StaticTemplate(Game::FRLG, 106, 25), // Hitmonlee @ Saffron City
    StaticTemplate(Game::FRLG, 107, 25), // Hitmonchan @ Saffron City
    StaticTemplate(Game::FRLG, 129, 5), // Magikarp @ Route 4
    StaticTemplate(Game::FRLG, 131, 25), // Lapras @ Silph Co.
    StaticTemplate(Game::FRLG, 133, 25), // Eevee @ Celadon City
    StaticTemplate(Game::FRLG, 175, 5) // Togepi Egg
};

constexpr std::array<StaticTemplate, 10> gameCorner = {
    StaticTemplate(Game::FireRed, 63, 9), // Abra @ Celadon City
    StaticTemplate(Game::FireRed, 35, 8), // Clefairy
    StaticTemplate(Game::FireRed, 123, 25), // Scyther
    StaticTemplate(Game::FireRed, 147, 18), // Dratini
    StaticTemplate(Game::FireRed, 137, 26), // Porygon
    StaticTemplate(Game::LeafGreen, 63, 7), // Abra
    StaticTemplate(Game::LeafGreen, 35, 12), // Clefairy
    StaticTemplate(Game::LeafGreen, 127, 18), // Pinsir
    StaticTemplate(Game::LeafGreen, 147, 24), // Dratini
    StaticTemplate(Game::LeafGreen, 137, 18) // Porygon
};

constexpr std::array<StaticTemplate, 6> stationary = {
    StaticTemplate(Game::RSE, 352, 30), // Kecleon @ Route 119/120
    StaticTemplate(Game::RSE, 101, 30), // Electrode @ Magma/Aqua Hideout
    StaticTemplate(Game::Emerald, 185, 40), // Sudowoodo @ Battle Frontier

    StaticTemplate(Game::FRLG, 143, 30), // Snorlax @ Route 12/16
    StaticTemplate(Game::FRLG, 101, 34), // Electrode @ Power Plant
    StaticTemplate(Game::FRLG, 97, 30) // Hypno @ Berry Forest
};

constexpr std::array<StaticTemplate, 14> legends = {
    StaticTemplate(Game::RSE, 377, 40), // Regirock @ Desert Ruins
    StaticTemplate(Game::RSE, 378, 40), // Regice @ Island Cave
    StaticTemplate(Game::RSE, 379, 40), // Registeel @ Ancient Tomb
    StaticTemplate(Game::Emerald | Game::Ruby, 380, 50), // Latias @ Southern Island
    StaticTemplate(Game::Emerald | Game::Sapphire, 381, 50), // Latios @ Southern Island
    StaticTemplate(Game::Sapphire, 382, 45), // Kyogre @ Cave of Origin
    StaticTemplate(Game::Emerald, 382, 70), // Kyogre @ Marine Cave
    StaticTemplate(Game::Ruby, 383, 45), // Groudon @ Cave of Origin
    StaticTemplate(Game::Emerald, 383, 70), // Groudon @ Terra Cave
    StaticTemplate(Game::RSE, 384, 70), // Rayquaza @ Sky Pillar

    StaticTemplate(Game::FRLG, 144, 50), // Articuno @ Seafoam Islands
    StaticTemplate(Game::FRLG, 145, 50), // Zapdos @ Power Plant
    StaticTemplate(Game::FRLG, 146, 50), // Moltres @ Mt. Ember
    StaticTemplate(Game::FRLG, 150, 70) // Mewtwo @ Cerulean Cave
};

constexpr std::array<StaticTemplate, 6> events = {
    StaticTemplate(Game::Emerald, 151, 30), // Mew @ Faraway Island
    StaticTemplate(Game::Emerald, 386, 30, 3), // Deoxys @ Birth Islands

    StaticTemplate(Game::FireRed, 386, 30, 1), // Deoxys @ Birth Islands
    StaticTemplate(Game::LeafGreen, 386, 30, 2), // Deoxys @ Birth Islands

    StaticTemplate(Game::Emerald | Game::FRLG, 249, 70), // Lugia @ Navel Rock
    StaticTemplate(Game::Emerald | Game::FRLG, 250, 70) // Ho-Oh @ Navel Rock
};

constexpr std::array<StaticTemplate, 69> galesColo = {
    // Non-shadows
    StaticTemplate(Game::Colosseum, 197, 26, Shiny::Never), // Umbreon
    StaticTemplate(Game::Colosseum, 196, 25, Shiny::Never), // Espeon
    StaticTemplate(Game::Colosseum, 250, 70, Shiny::Never), // Mattle Ho-Oh
    StaticTemplate(Game::Colosseum, 251, 10, Shiny::Never), // Ageto Celebi
    StaticTemplate(Game::Colosseum, 25, 10, Shiny::Never), // Ageto Pikachu

    StaticTemplate(Game::Gales, 133, 10), // Eevee
    StaticTemplate(Game::Gales, 152, 5), // Chikorita
    StaticTemplate(Game::Gales, 155, 5), // Cyndaquil
    StaticTemplate(Game::Gales, 158, 5), // Totodile

    // Shadows
    StaticTemplate(Game::Colosseum, 153, 30), // Bayleef
    StaticTemplate(Game::Colosseum, 156, 30), // Quilava
    StaticTemplate(Game::Colosseum, 159, 30), // Croconaw
    StaticTemplate(Game::Colosseum, 164, 30), // Noctowl
    StaticTemplate(Game::Colosseum, 180, 30), // Flaaffy
    StaticTemplate(Game::Colosseum, 188, 30), // Skiploom
    StaticTemplate(Game::Colosseum, 195, 30), // Quagsire
    StaticTemplate(Game::Colosseum, 200, 30), // Misdreavus
    StaticTemplate(Game::Colosseum, 193, 33), // Yanma
    StaticTemplate(Game::Colosseum, 162, 33), // Furret
    StaticTemplate(Game::Colosseum, 218, 30), // Slugma
    StaticTemplate(Game::Colosseum, 223, 20), // Remoraid
    StaticTemplate(Game::Colosseum, 226, 33), // Mantine
    StaticTemplate(Game::Colosseum, 211, 33), // Qwilfish
    StaticTemplate(Game::Colosseum, 307, 33), // Meditite
    StaticTemplate(Game::Colosseum, 206, 33), // Dunsparce
    StaticTemplate(Game::Colosseum, 333, 33), // Swablu
    StaticTemplate(Game::Colosseum, 185, 35), // Sudowoodo
    StaticTemplate(Game::Colosseum, 237, 38), // Hitmontop
    StaticTemplate(Game::Colosseum, 244, 40), // Entei
    StaticTemplate(Game::Colosseum, 166, 40), // Ledian
    StaticTemplate(Game::Colosseum, 245, 40), // Suicune
    StaticTemplate(Game::Colosseum, 234, 43), // Stantler
    StaticTemplate(Game::Colosseum, 221, 43), // Piloswine
    StaticTemplate(Game::Colosseum, 215, 43), // Sneasel
    StaticTemplate(Game::Colosseum, 190, 43), // Aipom
    StaticTemplate(Game::Colosseum, 205, 43), // Forretress
    StaticTemplate(Game::Colosseum, 210, 43), // Granbull
    StaticTemplate(Game::Colosseum, 329, 43), // Vibrava
    StaticTemplate(Game::Colosseum, 168, 43), // Ariados
    StaticTemplate(Game::Colosseum, 243, 40), // Raikou
    StaticTemplate(Game::Colosseum, 192, 45), // Sunflora
    StaticTemplate(Game::Colosseum, 225, 45), // Delibird
    StaticTemplate(Game::Colosseum, 227, 47), // Skarmory
    StaticTemplate(Game::Colosseum, 241, 48), // Miltank
    StaticTemplate(Game::Colosseum, 359, 48), // Absol
    StaticTemplate(Game::Colosseum, 229, 48), // Houndoom
    StaticTemplate(Game::Colosseum, 357, 49), // Tropius
    StaticTemplate(Game::Colosseum, 376, 50), // Metagross
    StaticTemplate(Game::Colosseum, 248, 55), // Tyranitar
    StaticTemplate(Game::Colosseum, 235, 45), // Smeargle
    StaticTemplate(Game::Colosseum, 213, 45), // Shuckle
    StaticTemplate(Game::Colosseum, 176, 20), // Toegtic

    StaticTemplate(Game::Gales, 216, 11, Shiny::Never), // Teddiursa
    StaticTemplate(Game::Gales, 343, 17, Shiny::Never), // Baltoy
    StaticTemplate(Game::Gales, 179, 17, Shiny::Never), // Mareep
    StaticTemplate(Game::Gales, 318, 15, Shiny::Never), // Carvanha
    StaticTemplate(Game::Gales, 228, 17, Shiny::Never), // Houndour
    StaticTemplate(Game::Gales, 335, 28, Shiny::Never), // Zangoose
    StaticTemplate(Game::Gales, 175, 25, Shiny::Never), // Togepi
    StaticTemplate(Game::Gales, 90, 29, Shiny::Never), // Shellder
    StaticTemplate(Game::Gales, 15, 30, Shiny::Never), // Beedrill
    StaticTemplate(Game::Gales, 277, 43, Shiny::Never), // Swellow
    StaticTemplate(Game::Gales, 249, 50, Shiny::Never), // Lugia
    StaticTemplate(Game::Gales, 112, 46, Shiny::Never), // Rhydon
    StaticTemplate(Game::Gales, 146, 50, Shiny::Never), // Moltres
    StaticTemplate(Game::Gales, 103, 46, Shiny::Never), // Exeggutor
    StaticTemplate(Game::Gales, 128, 46, Shiny::Never), // Tauros
    StaticTemplate(Game::Gales, 144, 50, Shiny::Never), // Articuno
    StaticTemplate(Game::Gales, 145, 50, Shiny::Never) // Zapdos
};

constexpr std::array<ShadowTemplate, 77> galesColoShadow = {
    ShadowTemplate(Game::Colosseum, 296, 30,
                   std::array<LockInfo, 5> {
                       LockInfo(24, 0, 127), // Duskull (M) (Quirky)
                       LockInfo(0, 1, 127) // Spinarak (F) (Hardy)
                   },
                   2, ShadowType::FirstShadow), // Makuhita
    ShadowTemplate(Game::Colosseum, 207, 43,
                   std::array<LockInfo, 5> {
                       LockInfo(12, 0, 127), // Teddiursa (M) (Serious)
                       LockInfo(6, 1, 191), // Jigglypuff (F) (Docile)
                       LockInfo(18, 0, 127) // Shroomish (M) (Bashful)
                   },
                   3,
                   ShadowType::FirstShadow), // Gligar
    ShadowTemplate(Game::Colosseum, 198, 43,
                   std::array<LockInfo, 5> {
                       LockInfo(6, 0, 127), // Carvanha (M) (Docile)
                       LockInfo(12, 1, 127), // Nuzleaf (F) (Serious)
                       LockInfo(18, 0, 127) // Houndour (M) (Bashful)
                   },
                   3, ShadowType::FirstShadow), // Murkrow
    ShadowTemplate(Game::Colosseum, 214, 45,
                   std::array<LockInfo, 5> {
                       LockInfo(0, 0, 127), // Masquerain (M) (Hardy)
                       LockInfo(0, 1, 127) // Ariados (F) (Hardy)
                   },
                   2,
                   ShadowType::FirstShadow), // Heracross
    ShadowTemplate(Game::Colosseum, 217, 45,
                   std::array<LockInfo, 5> {
                       LockInfo(20, 1, 63), // Machoke (F) (Calm)
                       LockInfo(16, 0, 31), // Marshtomp (M) (Mild)
                       LockInfo(21, 1, 127) // Shiftry (F) (Gentle)
                   },
                   3,
                   ShadowType::FirstShadow), // Ursaring
    ShadowTemplate(Game::Colosseum, 175, 20,
                   std::array<LockInfo, 5> {
                       LockInfo(23, 0, 127), // Sableye (M) (Careful)
                       LockInfo(8, 0, 127), // Grimer (M) (Impish)
                       LockInfo(24, 0, 127), // Gulpin (M) (Quirky)
                       LockInfo(22, 1, 31) // Togepi (F) (Sassy)
                   },
                   4, ShadowType::EReader), // Toegpi
    ShadowTemplate(Game::Colosseum, 179, 37,
                   std::array<LockInfo, 5> {
                       LockInfo(4, 1, 191), // Skitty (F) (Naughty)
                       LockInfo(10, 1, 127), // Qwilfish (F) (Timid)
                       LockInfo(12, 1, 127), // Duskull (F) (Serious)
                       LockInfo(16, 1, 127) // Mareep (F) (Mild)
                   },
                   4, ShadowType::EReader), // Mareep
    ShadowTemplate(Game::Colosseum, 212, 50,
                   std::array<LockInfo, 5> {
                       LockInfo(13, 1, 191), // Murkow (F) (Jolly)
                       LockInfo(2, 2, 255), // Claydol (-) (Brave)
                       LockInfo(3, 0, 127), // Steelix (M) (Adamant)
                       LockInfo(11, 0, 127) // Scizor (M) (Hasty)
                   },
                   4, ShadowType::EReader), // Scizor

    ShadowTemplate(Game::Gales, 165, 10, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(0, 1, 127) // Taillow (F) (Hardy)
                   },
                   1, ShadowType::SingleLock), // Ledyba
    ShadowTemplate(Game::Gales, 261, 10, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(12, 1, 127), // Zubat (F) (Serious)
                   },
                   1, ShadowType::SingleLock), // Poochyena
    ShadowTemplate(Game::Gales, 363, 17, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(24, 0, 63), // Horsea (M) (Quirky)
                       LockInfo(12, 1, 127), // Goldeen (F) (Serious)
                   },
                   2, ShadowType::FirstShadow), // Spheal @ Lab
    ShadowTemplate(Game::Gales, 363, 17, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(24, 0, 63), // Horsea (M) (Quirky)
                       LockInfo(12, 1, 127), // Goldeen (F) (Serious)
                       LockInfo(0, 2, 255), // Beldum (-) (Hardy)
                   },
                   3, ShadowType::FirstShadow), // Spheal @ Phenac/Citadark
    ShadowTemplate(Game::Gales, 316, 17, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(12, 1, 127), // Koffing (F) (Serious)
                       LockInfo(6, 0, 127), // Grimer (M) (Docile)
                   },
                   2, ShadowType::FirstShadow), // Gulpin
    ShadowTemplate(Game::Gales, 273, 17, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(6, 0, 127), // Oddish (M) (Docile)
                       LockInfo(24, 1, 127), // Cacnea (F) (Quirky)
                       LockInfo(18, 1, 127), // Shroomish (F) (Bashful)
                       LockInfo(0, 0, 127), // Lotad (M) (Hardy)
                       LockInfo(12, 0, 127), // Pineco (M) (Serious)
                   },
                   5, ShadowType::FirstShadow), // Seedot @ Lab
    ShadowTemplate(Game::Gales, 273, 17, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(6, 0, 127), // Oddish (M) (Docile)
                       LockInfo(24, 1, 127), // Cacnea (F) (Quirky)
                       LockInfo(0, 1, 127), // Shroomish (F) (Hardy)
                       LockInfo(0, 1, 127), // Lotad (F) (Hardy)
                       LockInfo(6, 0, 127), // Pineco (M) (Docile)
                   },
                   5, ShadowType::FirstShadow), // Seedot @ Phenac City
    ShadowTemplate(Game::Gales, 273, 17, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(6, 0, 127), // Vileplume (M) (Docile)
                       LockInfo(24, 1, 127), // Cacturne (F) (Quirky)
                       LockInfo(0, 1, 127), // Breloom (F) (Hardy)
                       LockInfo(0, 0, 127), // Lombre (M) (Hardy)
                       LockInfo(12, 0, 127), // Forretress (M) (Serious)
                   },
                   5, ShadowType::FirstShadow), // Seedot @ Citadark
    ShadowTemplate(Game::Gales, 167, 14, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(12, 1, 127), // Swinub (F) (Serious)
                       LockInfo(6, 0, 127), // Shuppet (M) (Docile)
                   },
                   2, ShadowType::FirstShadow), // Spinarak
    ShadowTemplate(Game::Gales, 322, 14, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(6, 0, 127), // Ralts (M) (Docile)
                       LockInfo(0, 2, 255), // Voltorb (-) (Hardy)
                       LockInfo(24, 1, 127), // Bagon (F) (Quirky)
                   },
                   3, ShadowType::FirstShadow), // Numel
    ShadowTemplate(Game::Gales, 285, 14, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(24, 1, 191), // Snubbull (F) (Quirky)
                       LockInfo(0, 1, 127), // Kecleon (F) (Hardy)
                   },
                   2, ShadowType::FirstShadow), // Shroomish
    ShadowTemplate(Game::Gales, 301, 18, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(6, 1, 191), // Luvdisc (F) (Docile)
                       LockInfo(0, 0, 127), // Beautifly (M) (Hardy)
                       LockInfo(24, 0, 127), // Roselia (M) (Quirky)
                   },
                   3, ShadowType::FirstShadow), // Delcatty
    ShadowTemplate(Game::Gales, 100, 19, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(0, 0, 127), // Lombre (M) (Hardy)
                       LockInfo(18, 0, 127), // Lombre (M) (Bashful)
                       LockInfo(12, 1, 127), // Lombre (F) (Serious)
                   },
                   3, ShadowType::FirstShadow), // Voltorb
    ShadowTemplate(Game::Gales, 296, 18, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(6, 0, 127), // Kecleon (M) (Docile)
                       LockInfo(18, 1, 127), // Surskit (F) (Bashful)
                   },
                   2, ShadowType::FirstShadow), // Makuhita
    ShadowTemplate(Game::Gales, 37, 18, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(0, 0, 127), // Spinarak (M) (Hardy)
                       LockInfo(6, 1, 127), // Beautifly (F) (Docile)
                       LockInfo(18, 0, 127), // Dustox (M) (Bashful)
                   },
                   3, ShadowType::FirstShadow), // Vulpix
    ShadowTemplate(Game::Gales, 355, 19, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(12, 0, 127), // Sneasel (M) (Serious)
                       LockInfo(18, 1, 127), // Yanma (F) (Bashful)
                       LockInfo(24, 0, 127), // Misdreavus (M) (Quirky)
                   },
                   3, ShadowType::FirstShadow), // Duskull
    ShadowTemplate(Game::Gales, 280, 20, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(0, 0, 63), // Kadabra (M) (Hardy)
                       LockInfo(6, 1, 127), // Flaaffy (F) (Docile)
                       LockInfo(18, 0, 127) // Vigoroth (M) (Bashful)
                   },
                   3, ShadowType::FirstShadow), // Ralts
    ShadowTemplate(Game::Gales, 303, 22, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(6, 0, 127), // Loudred (M) (Docile)
                       LockInfo(18, 1, 127), // Girafarig (F) (Bashful)
                   },
                   2, ShadowType::FirstShadow), // Mawile
    ShadowTemplate(Game::Gales, 361, 20, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(6, 1, 127), // Seviper (F) (Docile)
                   },
                   1, ShadowType::SingleLock), // Snorunt
    ShadowTemplate(Game::Gales, 361, 20, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(6, 0, 127), // Murkrow (M) (Docile)
                   },
                   1, ShadowType::SingleLock), // Pineco
    ShadowTemplate(Game::Gales, 177, 22, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(0, 0, 127), // Kirlia (M) (Hardy)
                       LockInfo(0, 1, 127), // Linoone (F) (Hardy)
                   },
                   2, ShadowType::FirstShadow), // Natu
    ShadowTemplate(Game::Gales, 177, 22, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(6, 0, 127), // Remoraid (M) (Docile)
                       LockInfo(18, 0, 127), // Golbat (M) (Bashful)
                   },
                   2, ShadowType::FirstShadow), // Roselia
    ShadowTemplate(Game::Gales, 52, 22, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(6, 0, 63), // Kadabra (M) (Docile)
                       LockInfo(0, 1, 127), // Sneasel (F) (Hardy)
                       LockInfo(18, 1, 127), // Misdreavus (F) (Bashful)
                   },
                   3, ShadowType::FirstShadow), // Meowth
    ShadowTemplate(Game::Gales, 220, 22, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(18, 1, 127), // Torkoal (F) (Bashful)
                       LockInfo(0, 0, 127), // Nuzleaf (M) (Hardy)
                   },
                   2, ShadowType::FirstShadow), // Swinub
    ShadowTemplate(Game::Gales, 21, 22, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(18, 0, 127), // Pelipper (M) (Bashful)
                       LockInfo(6, 1, 127), // Electrike (F) (Docile)
                   },
                   2, ShadowType::FirstShadow), // Spearow
    ShadowTemplate(Game::Gales, 88, 23, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(12, 0, 127), // Chimecho (M) (Serious)
                       LockInfo(18, 0, 127), // Stantler (M) (Bashful)
                   },
                   2, ShadowType::FirstShadow), // Grimer
    ShadowTemplate(Game::Gales, 86, 23, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(6, 0, 127), // Hoothoot (M) (Docile)
                       LockInfo(18, 0, 127), // Graveler (M) (Bashful)
                       LockInfo(18, 1, 127), // Gulpin (F) (Bashful)
                   },
                   3, ShadowType::FirstShadow), // Seel
    ShadowTemplate(Game::Gales, 337, 25, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(0, 1, 127), // Lanturn (F) (Hardy)
                       LockInfo(18, 0, 127), // Quagsire (M) (Bashful)
                   },
                   2, ShadowType::FirstShadow), // Lunatone
    ShadowTemplate(Game::Gales, 299, 26, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(0, 0, 127), // Lombre (M) (Hardy)
                       LockInfo(18, 0, 127), // Lombre (M) (Bashful)
                       LockInfo(12, 1, 127), // Lombre (F) (Serious)
                   },
                   3, ShadowType::FirstShadow), // Nosepass
    ShadowTemplate(Game::Gales, 46, 28, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(24, 0, 127), // Seviper (M) (Quirky)
                       LockInfo(6, 1, 127), // Murkrow (F) (Docile)
                   },
                   2, ShadowType::FirstShadow), // Paras
    ShadowTemplate(Game::Gales, 58, 28, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(24, 0, 127), // Seviper (M) (Quirky)
                       LockInfo(6, 1, 127), // Murkrow (F) (Docile)
                       LockInfo(), // Shadow Paras
                   },
                   3, ShadowType::SecondShadow), // Growlithe
    ShadowTemplate(Game::Gales, 17, 30, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(), // Shadow Beedrill
                       LockInfo(12, 0, 127), // Furret (M) (Serious)
                       LockInfo(18, 0, 31), // Togetic (M) (Bashful)
                   },
                   3, ShadowType::FirstShadow), // Pidgeotto
    ShadowTemplate(Game::Gales, 114, 30, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(12, 1, 191), // Ninetales (F) (Serious)
                       LockInfo(6, 0, 127), // Jumpluff (M) (Docile)
                       LockInfo(0, 1, 127), // Azumarill (F) (Hardy)
                   },
                   3, ShadowType::FirstShadow), // Tangela
    ShadowTemplate(Game::Gales, 12, 30, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(12, 1, 191), // Ninetales (F) (Serious)
                       LockInfo(6, 0, 127), // Jumpluff (M) (Docile)
                       LockInfo(0, 1, 127), // Azumarill (F) (Hardy)
                       LockInfo(), // Shadow Tangela
                   },
                   4, ShadowType::SecondShadow), // Butterfree
    ShadowTemplate(Game::Gales, 82, 30, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(18, 2, 255), // Shedinja (-) (Bashful)
                       LockInfo(0, 0, 127), // Wobbuffet (M) (Hardy)
                       LockInfo(12, 1, 127), // Vibrava (F) (Serious)
                   },
                   3, ShadowType::FirstShadow), // Magneton
    ShadowTemplate(Game::Gales, 49, 32, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(18, 1, 127), // Golduck (F) (Bashful)
                       LockInfo(24, 0, 0), // Hitmontop (M) (Quirky)
                       LockInfo(12, 0, 63), // Hariyama (M) (Serious)
                   },
                   3, ShadowType::FirstShadow), // Venomoth
    ShadowTemplate(Game::Gales, 70, 32, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(18, 1, 127), // Golduck (F) (Bashful)
                       LockInfo(24, 0, 0), // Hitmontop (M) (Quirky)
                       LockInfo(12, 0, 063), // Hariyama (M) (Serious)
                       LockInfo(), // Shadow Venomoth
                   },
                   4, ShadowType::SecondShadow), // Weepinbell
    ShadowTemplate(Game::Gales, 24, 33, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(6, 0, 127), // Huntail (M) (Docile)
                       LockInfo(0, 1, 127), // Cacturne (F) (Hardy)
                       LockInfo(12, 1, 127), // Weezing (F) (Serious)
                       LockInfo(18, 1, 127), // Ursaring (F) (Bashful)
                   },
                   4, ShadowType::FirstShadow), // Arbok
    ShadowTemplate(Game::Gales, 57, 34, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(18, 1, 127), // Lairon (F) (Bashful)
                       LockInfo(12, 1, 127), // Sealeo (F) (Serious)
                       LockInfo(6, 1, 127), // Slowking (F) (Docile)
                       LockInfo(24, 0, 127), // Ursaring (M) (Quirky)
                   },
                   4, ShadowType::FirstShadow), // Primeape
    ShadowTemplate(Game::Gales, 97, 34, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(18, 1, 127), // Lairon (F) (Bashful)
                       LockInfo(12, 1, 127), // Sealeo (F) (Serious)
                       LockInfo(6, 1, 127), // Slowking (F) (Docile)
                       LockInfo(24, 0, 127), // Ursaring (M) (Quirky)
                       LockInfo(), // Shadow Primeape
                   },
                   5, ShadowType::SecondShadow), // Hypno
    ShadowTemplate(Game::Gales, 55, 33, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(24, 0, 127), // Crawdaunt (M) (Quirky)
                       LockInfo(6, 1, 127), // Pelipper (F) (Docile)
                       LockInfo(18, 1, 127), // Mantine (F) (Bashful)
                   },
                   3, ShadowType::FirstShadow), // Golduck
    ShadowTemplate(Game::Gales, 302, 33, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(24, 0, 127), // Crawdaunt (M) (Quirky)
                       LockInfo(6, 1, 127), // Pelipper (F) (Docile)
                       LockInfo(18, 1, 127), // Mantine (F) (Bashful)
                       LockInfo(), // Shadow Golduck
                   },
                   4, ShadowType::SecondShadow), // Sableye
    ShadowTemplate(Game::Gales, 85, 34, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(18, 1, 127), // Xatu (F) (Bashful)
                   },
                   1, ShadowType::SingleLock), // Dodrio
    ShadowTemplate(Game::Gales, 20, 34, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(18, 1, 127), // Xatu (F) (Bashful)
                       LockInfo(), // Shadow Dodrio
                       LockInfo(18, 0, 127), // Whiscash (M) (Bashful)
                   },
                   3, ShadowType::FirstShadow), // Raticate
    ShadowTemplate(Game::Gales, 83, 36, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(12, 0, 127), // Gardevoir (M) (Serious)
                       LockInfo(0, 1, 127), // Gorebyss (F) (Hardy)
                       LockInfo(24, 0, 127), // Roselia (M) (Quirky)
                   },
                   3, ShadowType::FirstShadow), // Farfetch'd
    ShadowTemplate(Game::Gales, 334, 36, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(12, 0, 127), // Gardevoir (M) (Serious)
                       LockInfo(0, 1, 127), // Gorebyss (F) (Hardy)
                       LockInfo(24, 0, 127), // Roselia (M) (Quirky)
                       LockInfo(), // Shadow Farfetch'd
                   },
                   4, ShadowType::SecondShadow), // Altaria
    ShadowTemplate(Game::Gales, 115, 35, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(0, 2, 255), // Electrode (-) (Hardy)
                       LockInfo(18, 1, 127), // Misdreavus (F) (Bashful)
                       LockInfo(12, 2, 255), // Claydol (-) (Serious)
                   },
                   3, ShadowType::FirstShadow), // Kangaskhan
    ShadowTemplate(Game::Gales, 354, 37, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(0, 2, 255), // Electrode (-) (Hardy)
                       LockInfo(18, 1, 127), // Misdreavus (F) (Bashful)
                       LockInfo(12, 2, 255), // Claydol (-) (Serious)
                       LockInfo(), // Shadow Kangaskhan
                   },
                   4, ShadowType::SecondShadow), // Banette
    ShadowTemplate(Game::Gales, 126, 36, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(18, 0, 127), // Houndoom (M) (Bashful)
                       LockInfo(18, 0, 191), // Ninetales (M) (Bashful)
                       LockInfo(0, 1, 127), // Vileplume (F) (Hardy)
                   },
                   3, ShadowType::FirstShadow), // Magmar
    ShadowTemplate(Game::Gales, 127, 35, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(18, 0, 127), // Houndoom (M) (Bashful)
                       LockInfo(18, 0, 191), // Ninetales (M) (Bashful)
                       LockInfo(0, 1, 127), // Vileplume (F) (Hardy)
                       LockInfo(), // Shadow Magmar
                   },
                   4, ShadowType::SecondShadow), // Pinsir
    ShadowTemplate(Game::Gales, 78, 40, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(24, 0, 127), // Camerupt (M) (Quirky)
                       LockInfo(6, 0, 127), // Weezing (M) (Docile)
                       LockInfo(12, 1, 127), // Muk (F) (Serious)
                   },
                   3, ShadowType::FirstShadow), // Rapidash
    ShadowTemplate(Game::Gales, 219, 38, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(24, 0, 127), // Camerupt (M) (Quirky)
                       LockInfo(6, 0, 127), // Weezing (M) (Docile)
                       LockInfo(12, 1, 127), // Muk (F) (Serious)
                       LockInfo(), // Shadow Rapidash
                   },
                   4, ShadowType::SecondShadow), // Magcargo
    ShadowTemplate(Game::Gales, 107, 38, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(24, 0, 127), // Medicham (M) (Quirky)
                       LockInfo(6, 1, 127), // Golem (F) (Docile)
                       LockInfo(18, 1, 127), // Xatu (F) (Bashful)
                   },
                   3, ShadowType::FirstShadow), // Hitmonchan
    ShadowTemplate(Game::Gales, 106, 38, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(18, 0, 127), // Grumpig (M) (Bashful)
                       LockInfo(12, 1, 127), // Skarmory (F) (Serious)
                       LockInfo(6, 2, 255), // Metang (-) (Docile)
                       LockInfo(24, 1, 063), // Hariyama (F) (Quirky)
                   },
                   4, ShadowType::FirstShadow), // Hitmonlee
    ShadowTemplate(Game::Gales, 108, 38, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(24, 0, 127), // Lanturn (M) (Quirky)
                       LockInfo(6, 2, 255), // Magneton (-) (Docile)
                   },
                   2, ShadowType::FirstShadow), // Lickitung
    ShadowTemplate(Game::Gales, 123, 40, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(6, 1, 127), // Stantler (F) (Docile)
                       LockInfo(24, 0, 127), // Exploud (M) (Quirky)
                   },
                   2, ShadowType::FirstShadow), // Scyther
    ShadowTemplate(Game::Gales, 113, 39, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(6, 1, 127), // Stantler (F) (Docile)
                       LockInfo(24, 0, 127), // Exploud (M) (Quirky)
                       LockInfo(), // Shadow Scyther
                   },
                   3, ShadowType::SecondShadow), // Chansey
    ShadowTemplate(Game::Gales, 338, 41, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(24, 2, 255), // Metang (-) (Quirky)
                       LockInfo(6, 0, 127), // Quagsire (M) (Docile)
                       LockInfo(0, 1, 127), // Scizor (F) (Hardy)
                   },
                   3, ShadowType::FirstShadow), // Solrock
    ShadowTemplate(Game::Gales, 121, 41, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(24, 2, 255), // Metang (-) (Quirky)
                       LockInfo(6, 0, 127), // Quagsire (M) (Docile)
                       LockInfo(0, 1, 127), // Scizor (F) (Hardy)
                       LockInfo(), // Shadow Solrock
                       LockInfo(18, 0, 127), // Castform (M) (Bashful)
                   },
                   5, ShadowType::FirstShadow), // Starmie
    ShadowTemplate(Game::Gales, 125, 43, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(), // Shadow Swellow
                       LockInfo(24, 0, 63), // Alakazam (M) (Quirky)
                       LockInfo(6, 1, 127), // Kingdra (F) (Docile)
                       LockInfo(18, 1, 127), // Heracross (F) (Bashful)
                   },
                   4, ShadowType::FirstShadow), // Electabuzz
    ShadowTemplate(Game::Gales, 143, 43, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(), // Shadow Swellow
                       LockInfo(24, 0, 63), // Alakazam (M) (Quirky)
                       LockInfo(6, 1, 127), // Kingdra (F) (Docile)
                       LockInfo(18, 1, 127), // Heracross (F) (Bashful)
                       LockInfo(), // Shadow Electabuzz
                   },
                   5, ShadowType::SecondShadow), // Snorlax
    ShadowTemplate(Game::Gales, 62, 42, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(18, 0, 127), // Slowking (M) (Bashful)
                       LockInfo(18, 0, 127), // Ursaring (M) (Bashful)
                       LockInfo(24, 0, 127), // Aggron (M) (Quirky)
                       LockInfo(6, 1, 127), // Walrein (F) (Docile)
                   },
                   4, ShadowType::FirstShadow), // Poliwrath
    ShadowTemplate(Game::Gales, 122, 42, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(18, 0, 127), // Slowking (M) (Bashful)
                       LockInfo(18, 0, 127), // Ursaring (M) (Bashful)
                       LockInfo(24, 0, 127), // Aggron (M) (Quirky)
                       LockInfo(6, 1, 127), // Walrein (F) (Docile)
                       LockInfo(), // Shadow Poliwrath
                   },
                   5, ShadowType::SecondShadow), // Mr. Mime
    ShadowTemplate(Game::Gales, 51, 40, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(0, 0, 127), // Glalie (M) (Hardy)
                       LockInfo(18, 0, 127), // Ampharos (M) (Bashful)
                       LockInfo(06, 1, 127), // Breloom (F) (Docile)
                       LockInfo(12, 0, 127), // Donphan (M) (Serious)
                   },
                   4, ShadowType::FirstShadow), // Dugtrio
    ShadowTemplate(Game::Gales, 310, 44, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(6, 1, 127), // Ninjask (F) (Docile)
                   },
                   1, ShadowType::FirstShadow), // Manetric
    ShadowTemplate(Game::Gales, 373, 50, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(6, 1, 127), // Ninjask (F) (Docile)
                       LockInfo(), // Shadow Manetric
                   },
                   2, ShadowType::Salamence), // Salamence
    ShadowTemplate(Game::Gales, 105, 44, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(6, 1, 127), // Ninjask (F) (Docile)
                       LockInfo(), // Shadow Manectric
                       LockInfo(), // Shadow Salamence
                       LockInfo(24, 0, 127), // Flygon (M) (Quirky)
                   },
                   4, ShadowType::FirstShadow), // Marowak
    ShadowTemplate(Game::Gales, 131, 44, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(6, 1, 127), // Ninjask (F) (Docile)
                       LockInfo(), // Shadow Manectric
                       LockInfo(), // Shadow Salamence
                       LockInfo(24, 0, 127), // Flygon (M) (Quirky)
                       LockInfo(), // Shadow Marowak
                   },
                   5, ShadowType::SecondShadow), // Lapras
    ShadowTemplate(Game::Gales, 149, 55, Shiny::Never,
                   std::array<LockInfo, 5> {
                       LockInfo(0, 0, 127), // Ludicolo (M) (Hardy)
                       LockInfo(18, 0, 127), // Ludicolo (M) (Bashful)
                       LockInfo(12, 1, 127), // Ludicolo (F) (Serious)
                       LockInfo(12, 1, 127), // Ludicolo (F) (Serious)
                       LockInfo(0, 0, 127), // Ludicolo (M) (Hardy)
                   },
                   5, ShadowType::FirstShadow) // Dragonite
};

constexpr std::array<StaticTemplate, 1> channel = {
    StaticTemplate(Game::GC, 385, 5) // Jirachi
};

namespace
{
    /**
     * @brief Gets wild encounters for the \p encouner and \p game
     *
     * @param encounter Encounter type
     * @param game Game version
     * @param info Pointer to PersonalInfo array
     *
     * @return Vector of wild encounters
     */
    std::vector<EncounterArea3> getAreas(Encounter encounter, Game game, const PersonalInfo *info)
    {
        const u8 *compressedData;
        size_t compressedSize;

        if (game == Game::Emerald)
        {
            compressedData = emerald.data();
            compressedSize = emerald.size();
        }
        else if (game == Game::FireRed)
        {
            compressedData = firered.data();
            compressedSize = firered.size();
        }
        else if (game == Game::LeafGreen)
        {
            compressedData = leafgreen.data();
            compressedSize = leafgreen.size();
        }
        else if (game == Game::Ruby)
        {
            compressedData = ruby.data();
            compressedSize = ruby.size();
        }
        else
        {
            compressedData = sapphire.data();
            compressedSize = sapphire.size();
        }

        u32 size;
        u8 *data = Utilities::decompress(compressedData, compressedSize, size);

        std::vector<EncounterArea3> encounters;
        for (size_t offset = 0; offset < size; offset += 121)
        {
            const u8 *entry = data + offset;

            u8 location = entry[0];
            u8 grass = entry[1];
            u8 water = entry[2];
            u8 rock = entry[3];
            u8 fish = entry[4];

            switch (encounter)
            {
            case Encounter::Grass:
                if (grass != 0)
                {
                    std::vector<Slot> slots;
                    slots.reserve(12);
                    for (size_t i = 0; i < 12; i++)
                    {
                        u8 level = entry[5 + (i * 3)];
                        u16 specie = *reinterpret_cast<const u16 *>(entry + 6 + (i * 3));
                        slots.emplace_back(specie, level, &info[specie]);
                    }
                    encounters.emplace_back(location, grass, encounter, slots);
                }
                break;
            case Encounter::Surfing:
                if (water != 0)
                {
                    std::vector<Slot> slots;
                    slots.reserve(5);
                    for (size_t i = 0; i < 5; i++)
                    {
                        u8 min = entry[41 + (i * 4)];
                        u8 max = entry[42 + (i * 4)];
                        u16 specie = *reinterpret_cast<const u16 *>(entry + 43 + (i * 4));
                        slots.emplace_back(specie, min, max, &info[specie]);
                    }
                    encounters.emplace_back(location, water, encounter, slots);
                }
                break;
            case Encounter::RockSmash:
                if (rock != 0)
                {
                    std::vector<Slot> slots;
                    slots.reserve(5);
                    for (size_t i = 0; i < 5; i++)
                    {
                        u8 min = entry[61 + (i * 4)];
                        u8 max = entry[62 + (i * 4)];
                        u16 specie = *reinterpret_cast<const u16 *>(entry + 63 + (i * 4));
                        slots.emplace_back(specie, min, max, &info[specie]);
                    }
                    encounters.emplace_back(location, rock, encounter, slots);
                }
                break;
            case Encounter::OldRod:
                if (fish != 0)
                {
                    std::vector<Slot> slots;
                    slots.reserve(2);
                    for (size_t i = 0; i < 2; i++)
                    {
                        u8 min = entry[81 + (i * 4)];
                        u8 max = entry[82 + (i * 4)];
                        u16 specie = *reinterpret_cast<const u16 *>(entry + 83 + (i * 4));
                        slots.emplace_back(specie, min, max, &info[specie]);
                    }
                    encounters.emplace_back(location, fish, encounter, slots);
                }
                break;
            case Encounter::GoodRod:
                if (fish != 0)
                {
                    std::vector<Slot> slots;
                    slots.reserve(3);
                    for (size_t i = 0; i < 3; i++)
                    {
                        u8 min = entry[89 + (i * 4)];
                        u8 max = entry[90 + (i * 4)];
                        u16 specie = *reinterpret_cast<const u16 *>(entry + 91 + (i * 4));
                        slots.emplace_back(specie, min, max, &info[specie]);
                    }
                    encounters.emplace_back(location, fish, encounter, slots);
                }
                break;
            case Encounter::SuperRod:
                if (fish != 0)
                {
                    std::vector<Slot> slots;
                    slots.reserve(5);
                    for (size_t i = 0; i < 5; i++)
                    {
                        u8 min = entry[101 + (i * 4)];
                        u8 max = entry[102 + (i * 4)];
                        u16 specie = *reinterpret_cast<const u16 *>(entry + 103 + (i * 4));
                        slots.emplace_back(specie, min, max, &info[specie]);
                    }
                    encounters.emplace_back(location, fish, encounter, slots);
                }
                break;
            default:
                break;
            }
        }
        delete[] data;
        return encounters;
    }
}

namespace Encounters3
{
    std::vector<EncounterArea3> getEncounters(Encounter encounter, Game version)
    {
        return getAreas(encounter, version, PersonalLoader::getPersonal(version));
    }

    const ShadowTemplate *getShadowTeams(size_t *size)
    {
        if (size)
        {
            *size = galesColoShadow.size();
        }
        return galesColoShadow.data();
    }

    const ShadowTemplate *getShadowTeam(int index)
    {
        const ShadowTemplate *templates = getShadowTeams();
        return &templates[index];
    }

    const StaticTemplate *getStaticEncounters(int type, size_t *size)
    {
        if (type == 0)
        {
            if (size)
            {
                *size = starters.size();
            }
            return starters.data();
        }
        else if (type == 1)
        {
            if (size)
            {
                *size = fossils.size();
            }
            return fossils.data();
        }
        else if (type == 2)
        {
            if (size)
            {
                *size = gifts.size();
            }
            return gifts.data();
        }
        else if (type == 3)
        {
            if (size)
            {
                *size = gameCorner.size();
            }
            return gameCorner.data();
        }
        else if (type == 4)
        {
            if (size)
            {
                *size = stationary.size();
            }
            return stationary.data();
        }
        else if (type == 5)
        {
            if (size)
            {
                *size = legends.size();
            }
            return legends.data();
        }
        else if (type == 6)
        {
            if (size)
            {
                *size = events.size();
            }
            return events.data();
        }
        else if (type == 7)
        {
            if (size)
            {
                *size = galesColo.size();
            }
            return galesColo.data();
        }
        else
        {
            if (size)
            {
                *size = channel.size();
            }
            return channel.data();
        }
    }

    const StaticTemplate *getStaticEncounter(int type, int index)
    {
        const StaticTemplate *templates = getStaticEncounters(type);
        return &templates[index];
    }
}
