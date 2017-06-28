#include "NatureLock.hpp"
#include <cstdint>
#include "LCRNG.hpp"
#include "LockInfo.hpp"

// Constructor for NatureLock
NatureLock::NatureLock(int lockNum)
{
    rng = LCRNG(0, "xdRNG");
    natureLockSetup(lockNum);
    count2 = count == 1 ? 0 : count - 2;
    x = 0;
}

// Sets up rest of nature lock data
void NatureLock::natureLockSetup(int lockNum)
{
    switch(lockNum)
    {
        // Altaria
        case 0:
            lockInfo[0] = LockInfo(24, 127, 255);
            lockInfo[1] = LockInfo(0, 0, 126);
            lockInfo[2] = LockInfo(12, 127, 255);
            count = 3;
            type = 6;
            break;
        // Arbok
        case 1:
            lockInfo[0] = LockInfo(18, 0, 126);
            lockInfo[1] = LockInfo(12, 0, 126);
            lockInfo[2] = LockInfo(0, 0, 126);
            lockInfo[3] = LockInfo(6, 127, 255);
            count = 4;
            type = 1;
            break;
        //Baltoy 2
        case 5:
            lockInfo[0] = LockInfo(0, 127, 255);
            lockInfo[1] = LockInfo(24, 127, 255);
            count = 2;
            type = 1;
            break;
        // Banette
        case 6:
            lockInfo[0] = LockInfo(12, 0, 255);
            lockInfo[1] = LockInfo(18, 0, 126);
            lockInfo[2] = LockInfo(0, 0, 255);
            count = 3;
            type = 6;
            break;
        // Butterfree
        case 8:
            lockInfo[0] = LockInfo(0, 0, 126);
            lockInfo[1] = LockInfo(6, 127, 255);
            lockInfo[2] = LockInfo(12, 0, 190);
            count = 3;
            type = 6;
            break;
        // Chansey
        case 10:
            lockInfo[0] = LockInfo(24, 127, 255);
            lockInfo[1] = LockInfo(6, 0, 126);
            count = 2;
            type = 6;
            break;
        // Delcatty
        case 11:
            lockInfo[0] = LockInfo(24, 127, 255);
            lockInfo[1] = LockInfo(0, 127, 255);
            lockInfo[2] = LockInfo(6, 0, 190);
            count = 3;
            type = 1;
            break;
        // Dodrio
        case 12:
            lockInfo[0] = LockInfo(18, 0, 126);
            count = 1;
            getCurrLock();
            type = 2;
            break;
        // Dragonite
        case 13:
            lockInfo[0] = LockInfo(0, 127, 255);
            lockInfo[1] = LockInfo(12, 0, 126);
            lockInfo[2] = LockInfo(12, 0, 126);
            lockInfo[3] = LockInfo(18, 127, 255);
            lockInfo[4] = LockInfo(0, 127, 255);
            count = 5;
            type = 1;
            break;
        // Dugtrio
        case 14:
            lockInfo[0] = LockInfo(12, 127, 255);
            lockInfo[1] = LockInfo(18, 127, 255);
            lockInfo[2] = LockInfo(6, 0, 126);
            lockInfo[3] = LockInfo(0, 127, 255);
            count = 4;
            type = 1;
            break;
        // Duskull
        case 15:
            lockInfo[0] = LockInfo(24, 127, 255);
            lockInfo[1] = LockInfo(18, 0, 126);
            lockInfo[2] = LockInfo(12, 127, 255);
            count = 3;
            type = 1;
            break;
        // Electabuzz
        case 16:
            lockInfo[0] = LockInfo(18, 0, 126);
            lockInfo[1] = LockInfo(6, 0, 126);
            lockInfo[2] = LockInfo(24, 63, 255);
            count = 3;
            type = 1;
            break;
        // Farfetch'd  
        case 18:
            lockInfo[0] = LockInfo(24, 127, 255);
            lockInfo[1] = LockInfo(0, 0, 126);
            lockInfo[2] = LockInfo(12, 127, 255);
            count = 3;
            type = 1;
            break;
        // Golduck
        case 19:
            lockInfo[0] = LockInfo(18, 0, 126);
            lockInfo[1] = LockInfo(6, 0, 126);
            lockInfo[2] = LockInfo(24, 127, 255);
            count = 3;
            type = 1;
            break;
        // Grimer
        case 20:
            lockInfo[0] = LockInfo(18, 127, 255);
            lockInfo[1] = LockInfo(12, 127, 255);
            count = 2;
            type = 1;
            break;
        // Growlithe
        case 21:
            lockInfo[0] = LockInfo(6, 0, 126);
            lockInfo[1] = LockInfo(24, 127, 255);
            count = 2;
            type = 6;
            break;
        // Gulpin 3
        case 22:
        // Gulpin 1
        case 23:
            lockInfo[0] = LockInfo(6, 127, 255);
            lockInfo[1] = LockInfo(12, 0, 126);
            count = 2;
            type = 1;
            break;
        // Gulpin 2
        case 24:
            lockInfo[0] = LockInfo(0, 0, 126);
            lockInfo[1] = LockInfo(0, 0, 126);
            lockInfo[2] = LockInfo(6, 127, 255);
            lockInfo[3] = LockInfo(12, 0, 126);
            count = 4;
            type = 1;
            break;
        // Hitmonchan
        case 25:
            lockInfo[0] = LockInfo(18, 0, 126);
            lockInfo[1] = LockInfo(6, 0, 126);
            lockInfo[2] = LockInfo(24, 127, 255);
            count = 3;
            type = 1;
            break;
        // Hitmonlee
        case 26:
            lockInfo[0] = LockInfo(24, 0, 126);
            lockInfo[1] = LockInfo(6, 0, 255);
            lockInfo[2] = LockInfo(12, 0, 126);
            lockInfo[3] = LockInfo(18, 127, 255);
            count = 4;
            type = 1;
            break;
        // Hypno
        case 30:
            lockInfo[0] = LockInfo(24, 127, 255);
            lockInfo[1] = LockInfo(6, 0, 126);
            lockInfo[2] = LockInfo(12, 0, 126);
            lockInfo[3] = LockInfo(18, 0, 126);
            count = 4;
            type = 6;
            break;
        // Kangaskhan
        case 31:
            lockInfo[0] = LockInfo(12, 0, 255);
            lockInfo[1] = LockInfo(18, 0, 126);
            lockInfo[2] = LockInfo(0, 0, 255);
            count = 3;
            type = 1;
            break;
        // Lapras
        case 32:
            lockInfo[0] = LockInfo(24, 127, 255);
            lockInfo[1] = LockInfo(500, 500, 500);
            lockInfo[2] = LockInfo(500, 500, 500);
            lockInfo[3] = LockInfo(6, 0, 126);
            count = 4;
            type = 6;
            break;
        // Ledyba
        case 33:
            lockInfo[0] = LockInfo(0, 0, 126);
            count = 1;
            getCurrLock();
            type = 2;
            break;
        // Lickitung
        case 34:
            lockInfo[0] = LockInfo(6, 0, 255);
            lockInfo[1] = LockInfo(24, 127, 255);
            count = 2;
            type = 1;
            break;
        // Lunatone
        case 36:
            lockInfo[0] = LockInfo(18, 127, 255);
            lockInfo[1] = LockInfo(0, 0, 126);
            count = 2;
            type = 1;
            break;
        // Marcargo
        case 37:
            lockInfo[0] = LockInfo(12, 0, 126);
            lockInfo[1] = LockInfo(6, 127, 255);
            lockInfo[2] = LockInfo(24, 127, 255);
            count = 3;
            type = 6;
            break;
        // Magmar 
        case 38:
            lockInfo[0] = LockInfo(0, 0, 126);
            lockInfo[1] = LockInfo(18, 191, 255);
            lockInfo[2] = LockInfo(18, 127, 255);
            count = 3;
            type = 1;
            break;
        // Magneton
        case 39:
            lockInfo[0] = LockInfo(12, 0, 126);
            lockInfo[1] = LockInfo(0, 127, 255);
            lockInfo[2] = LockInfo(18, 0, 255);
            count = 3;
            type = 1;
            break;
        // Makuhita
        case 40:
            lockInfo[0] = LockInfo(18, 0, 126);
            lockInfo[1] = LockInfo(6, 127, 255);
            count = 2;
            type = 1;
            break;
        // Makuhita Colo
        case 41:
            lockInfo[0] = LockInfo(0, 0, 126);
            lockInfo[1] = LockInfo(24, 127, 255);
            count = 2;
            type = 1;
            break;
        // Manectric
        case 42:
            lockInfo[0] = LockInfo(6, 0, 126);
            count = 1;
            getCurrLock();
            type = 2;
            break;
        // Mareep 1
        case 44:
            lockInfo[0] = LockInfo(12, 0, 126);
            lockInfo[1] = LockInfo(24, 127, 255);
            count = 2;
            type = 1;
            break;
        // Mareep 2
        case 45:
            lockInfo[0] = LockInfo(0, 0, 255);
            lockInfo[1] = LockInfo(12, 0, 126);
            lockInfo[2] = LockInfo(24, 127, 255);
            count = 3;
            type = 1;
            break;
        // Marowak
        case 46:
            lockInfo[0] = LockInfo(24, 127, 255);
            lockInfo[1] = LockInfo(500, 500, 500);
            lockInfo[2] = LockInfo(500, 500, 500);
            lockInfo[3] = LockInfo(6, 0, 126);
            count = 4;
            type = 1;
            break;
        // Mawile
        case 47:
            lockInfo[0] = LockInfo(18, 0, 126);
            lockInfo[1] = LockInfo(6, 127, 255);
            count = 2;
            type = 1;
            break;
        // Meowth
        case 48:
            lockInfo[0] = LockInfo(18, 0, 126);
            lockInfo[1] = LockInfo(0, 0, 126);
            lockInfo[2] = LockInfo(6, 63, 255);
            count = 3;
            type = 1;
            break;
        // Mr. Mime
        case 50:
            lockInfo[0] = LockInfo(6, 0, 126);
            lockInfo[1] = LockInfo(24, 127, 255);
            lockInfo[2] = LockInfo(18, 127, 255);
            lockInfo[3] = LockInfo(18, 127, 255);
            count = 4;
            type = 6;
            break;
        // Natu
        case 51:
            lockInfo[0] = LockInfo(0, 0, 126);
            lockInfo[1] = LockInfo(24, 127, 255);
            count = 2;
            type = 1;
            break;
        // Nosepass
        case 52:
            lockInfo[0] = LockInfo(12, 0, 126);
            lockInfo[1] = LockInfo(18, 127, 255);
            lockInfo[2] = LockInfo(0, 127, 255);
            count = 3;
            type = 1;
            break;
        // Numel
        case 53:
            lockInfo[0] = LockInfo(24, 0, 126);
            lockInfo[1] = LockInfo(0, 0, 255);
            lockInfo[2] = LockInfo(6, 127, 255);
            count = 3;
            type = 1;
            break;
        // Paras
        case 54:
            lockInfo[0] = LockInfo(6, 0, 126);
            lockInfo[1] = LockInfo(24, 127, 255);
            count = 2;
            type = 1;
            break;
        // Pidgeotto
        case 55:
            lockInfo[0] = LockInfo(18, 32, 255);
            lockInfo[1] = LockInfo(12, 127, 255);
            count = 2;
            type = 1;
            break;
        // Pineco
        case 56:
            lockInfo[0] = LockInfo(6, 127, 255);
            count = 1;
            getCurrLock();
            type = 2;
            break;
        // Pinsir
        case 57:
            lockInfo[0] = LockInfo(0, 0, 126);
            lockInfo[1] = LockInfo(18, 191, 255);
            lockInfo[2] = LockInfo(18, 127, 255);
            count = 3;
            type = 6;
            break;
        // Poliwrath
        case 58:
            lockInfo[0] = LockInfo(6, 0, 126);
            lockInfo[1] = LockInfo(24, 127, 255);
            lockInfo[2] = LockInfo(18, 127, 255);
            lockInfo[3] = LockInfo(18, 127, 255);
            count = 4;
            type = 1;
            break;
        // Poochyena
        case 59:
            lockInfo[0] = LockInfo(12, 0, 126);
            count = 1;
            getCurrLock();
            type = 2;
            break;
        // Primeape
        case 60:
            lockInfo[0] = LockInfo(24, 127, 255);
            lockInfo[1] = LockInfo(6, 0, 126);
            lockInfo[2] = LockInfo(12, 0, 126);
            lockInfo[3] = LockInfo(18, 0, 126);
            count = 4;
            type = 1;
            break;
        // Ralts
        case 61:
            lockInfo[0] = LockInfo(18, 127, 255);
            lockInfo[1] = LockInfo(6, 0, 126);
            lockInfo[2] = LockInfo(0, 63, 255);
            count = 3;
            type = 1;
            break;
        // Rapidash
        case 62:
            lockInfo[0] = LockInfo(12, 0, 126);
            lockInfo[1] = LockInfo(6, 127, 255);
            lockInfo[2] = LockInfo(24, 127, 255);
            count = 3;
            type = 1;
            break;
        // Raticate
        case 63:
            lockInfo[0] = LockInfo(18, 127, 255);
            lockInfo[1] = LockInfo(500, 500, 500);
            lockInfo[2] = LockInfo(18, 0, 126);
            count = 3;
            type = 1;
            break;
        // Roselia
        case 65:
            lockInfo[0] = LockInfo(18, 127, 255);
            lockInfo[1] = LockInfo(6, 127, 255);
            count = 2;
            type = 1;
            break;
        // Sableye
        case 66:
            lockInfo[0] = LockInfo(18, 0, 126);
            lockInfo[1] = LockInfo(6, 0, 126);
            lockInfo[2] = LockInfo(24, 127, 255);
            count = 3;
            type = 6;
            break;
        // Salamence
        case 67:
            lockInfo[0] = LockInfo(6, 0, 126);
            count = 1;
            getCurrLock();
            type = 3;
            break;
        // Scyther
        case 68:
            lockInfo[0] = LockInfo(24, 127, 255);
            lockInfo[1] = LockInfo(6, 0, 126);
            count = 2;
            type = 1;
            break;
        // Seedot 1
        case 70:
            lockInfo[0] = LockInfo(12, 127, 255);
            lockInfo[1] = LockInfo(0, 127, 255);
            lockInfo[2] = LockInfo(12, 0, 126);
            lockInfo[3] = LockInfo(24, 0, 126);
            lockInfo[4] = LockInfo(6, 127, 255);
            count = 5;
            type = 1;
            break;
        // Seedot 2
        case 71:
            lockInfo[0] = LockInfo(6, 127, 255);
            lockInfo[1] = LockInfo(0, 0, 126);
            lockInfo[2] = LockInfo(0, 0, 126);
            lockInfo[3] = LockInfo(24, 0, 126);
            lockInfo[4] = LockInfo(6, 127, 255);
            count = 5;
            type = 1;
            break;
        // Seel
        case 72:
            lockInfo[0] = LockInfo(18, 0, 126);
            lockInfo[1] = LockInfo(12, 127, 255);
            lockInfo[2] = LockInfo(6, 127, 255);
            count = 3;
            type = 1;
            break;
        // Shroomish
        case 74:
            lockInfo[0] = LockInfo(0, 0, 126);
            lockInfo[1] = LockInfo(24, 0, 126);
            count = 2;
            type = 1;
            break;
        // Snorlax
        case 75:
            lockInfo[0] = LockInfo(18, 0, 126);
            lockInfo[1] = LockInfo(6, 0, 126);
            lockInfo[2] = LockInfo(24, 63, 255);
            count = 3;
            type = 6;
            break;
        // Snorunt
        case 76:
            lockInfo[0] = LockInfo(6, 0, 126);
            count = 1;
            getCurrLock();
            type = 2;
            break;
        // Solrock
        case 77:
            lockInfo[0] = LockInfo(0, 0, 126);
            lockInfo[1] = LockInfo(6, 127, 255);
            lockInfo[2] = LockInfo(24, 0, 255);
            count = 3;
            type = 1;
            break;
        // Spearow
        case 78:
            lockInfo[0] = LockInfo(6, 0, 126);
            lockInfo[1] = LockInfo(18, 127, 255);
            count = 2;
            type = 1;
            break;
        // Spheal 3
        case 79:
        // Spheal 2
        case 81:
            lockInfo[0] = LockInfo(0, 0, 255);
            lockInfo[1] = LockInfo(12, 0, 126);
            lockInfo[2] = LockInfo(24, 127, 255);
            count = 3;
            type = 1;
            break;
        // Spheal 1
        case 80:
            lockInfo[0] = LockInfo(12, 0, 126);
            lockInfo[1] = LockInfo(24, 127, 255);
            count = 2;
            type = 1;
            break;
        // Spinarak
        case 82:
            lockInfo[0] = LockInfo(6, 127, 255);
            lockInfo[1] = LockInfo(12, 0, 126);
            count = 2;
            type = 1;
            break;
        // Starmie
        case 83:
            lockInfo[0] = LockInfo(18, 127, 255);
            lockInfo[1] = LockInfo(500, 500, 500);
            lockInfo[2] = LockInfo(0, 0, 126);
            lockInfo[3] = LockInfo(6, 127, 255);
            lockInfo[4] = LockInfo(24, 0, 255);
            count = 5;
            type = 1;
            break;
        // Swinub
        case 85:
            lockInfo[0] = LockInfo(0, 127, 255);
            lockInfo[1] = LockInfo(18, 0, 126);
            count = 2;
            type = 1;
            break;
        // Tangela
        case 86:
            lockInfo[0] = LockInfo(0, 0, 126);
            lockInfo[1] = LockInfo(6, 127, 255);
            lockInfo[2] = LockInfo(12, 0, 190);
            count = 3;
            type = 1;
            break;
        // Venomoth
        case 90:
            lockInfo[0] = LockInfo(12, 127, 255);
            lockInfo[1] = LockInfo(24, 0, 255);
            lockInfo[2] = LockInfo(18, 0, 126);
            count = 3;
            type = 1;
            break;
        // Voltorb
        case 91:
            lockInfo[0] = LockInfo(12, 0, 126);
            lockInfo[1] = LockInfo(12, 127, 255);
            lockInfo[2] = LockInfo(0, 127, 255);
            count = 3;
            type = 1;
            break;
        // Vulpix
        case 92:
            lockInfo[0] = LockInfo(18, 127, 255);
            lockInfo[1] = LockInfo(6, 0, 126);
            lockInfo[2] = LockInfo(0, 127, 255);
            count = 3;
            type = 1;
            break;
        // Weepinbell
        case 93:
            lockInfo[0] = LockInfo(12, 127, 255);
            lockInfo[1] = LockInfo(24, 0, 255);
            lockInfo[2] = LockInfo(18, 0, 126);
            count = 3;
            type = 6;
            break;
        // Articuno
        case 2:
        // Baltoy 3
        case 3:
        // Baltoy 1
        case 4:
        // Beedrill
        case 7:
        // Carvanha
        case 9:
        // Exeggutor
        case 17:
        // Houndour 3
        case 27:
        // Houndour 1
        case 28:
        // To do houndour 2
        case 29:
        // Lugia
        case 35:
        // Mareep 3
        case 43:
        // Moltres
        case 49:
        // Rhydon
        case 64:
        // To do seedot 3
        case 69:
        // Shellder
        case 73:
        // Swellow
        case 84:
        // Tauros
        case 87:
        // Teddiursa
        case 88:
        // Togepi
        case 89:
        // Zangoose
        case 94:
        // Zapdos
        default:
            type = 0;
            break;
    }
}

// Checks if seed is valid for single nature lock
bool NatureLock::ivMethodSingleNL(uint32_t seed)
{
    rng.setSeed(seed);
    rng.reverseFrames(1);

    // Build PID
    pid = getPIDReverse();

    // Backwards nature lock check
    gender = pid & 255;
    return !(gender < genderLower || gender > genderUpper || pid % 25 != nature);
}

// Salamence is a special case of single nature lock and second shadow
// Checks if seed is valud for 1st shadow unset for Salamence
bool NatureLock::ivMethodSalamenceUnset(uint32_t seed)
{
    rng.setSeed(seed);
    rng.reverseFrames(13);

    // Build PID
    pid = getPIDReverse();

    // Backwards nature lock check
    gender = pid & 255;
    return !(gender < genderLower || gender > genderUpper || pid % 25 != nature);
}

// Checks if seed is valid for 1st shadow set for Salamence
bool NatureLock::ivMethodSalamenceSet(uint32_t seed)
{
    rng.setSeed(seed);
    rng.reverseFrames(11);

    // Build PID
    pid = getPIDReverse();

    // Backwards nature lock check
    gender = pid & 255;
    return !(gender < genderLower || gender > genderUpper || pid % 25 != nature);
}

// Checks if seed is valid for 1st shadow set unset and antishiny(aka Shiny Skip) for Salamence
bool NatureLock::ivMethodSalamenceShinySkip(uint32_t seed)
{
    rng.setSeed(seed);
    rng.reverseFrames(1);

    uint32_t psv, psvtemp;

    // Check how many advances from shiny skip and build PID
    psv = getPSVReverse();
    psvtemp = getPSVReverse();
        
    while (psv == psvtemp)
    {
        psvtemp = psv;
        psv = getPSVReverse();
    }

    rng.reverseFrames(10);
    pid = getPIDReverse();

    // Backwards nature lock check
    gender = pid & 255;
    return !(gender < genderLower || gender > genderUpper || pid % 25 != nature)
}

// Checks if seed is valid for single shadow case
bool NatureLock::ivMethodFirstShadow(uint32_t seed)
{
    rng.setSeed(seed);
    rng.reverseFrames(1);

    // Build temp pid first to not waste time looping if first backwards nl fails
    pidOriginal = getPIDReverse();

    // Backwards nature lock check
    gender = pidOriginal & 255;
    if (gender < lockInfo[0].getGenderLower() || gender > lockInfo[0].getGenderUpper() || pidOriginal % 25 != lockInfo[0].getNature())
        return false;

    // Backwards nature lock check loop
    for (x = 1; x < count; x++)
    {
        rng.reverseFrames(3);
        pid = getPIDReverse();
        getCurrLock();
        if (nature != 500)
        {
            gender = pid & 255;
            if (gender < genderLower || gender > genderUpper|| pid % 25 != nature)
                countBackTwo();
        }
    }

    rng.advanceFrames(1);

    // Forwards nature lock check loop
    for (x = count2; x >= 0; x--)
    {
        rng.advanceFrames(3);
        pid = getPIDForward();
        getCurrLock();
        if (nature != 500)
        {
            gender = pid & 255;
            if (gender < genderLower || gender > genderUpper || pid % 25 != nature)
                countForwardTwo();
        }
    }

    return pidOriginal == pid;
}

// Checks if seed is valid for second shadow with first shadow unset
bool NatureLock::ivMethodFirstShadowUnset(uint32_t seed)
{
    rng.setSeed(seed);
    rng.reverseFrames(13);

    // Build temp pid first to not waste time looping if first nl fails
    pidOriginal = getPIDReverse();

    // Backwards nature lock check
    gender = pidOriginal & 255;
    if (gender < lockInfo[0].getGenderLower() || gender > lockInfo[0].getGenderUpper() || pidOriginal % 25 != lockInfo[0].getNature())
        return false;
    
    // Backwards nature lock check loop
    for (x = 1; x < count; x++)
    {
        rng.reverseFrames(3);
        pid = getPIDReverse();
        getCurrLock();
        if (nature != 500)
        {
            gender = pid & 255;
            if (gender < genderLower || gender > genderUpper || pid % 25 != nature)
                countBackTwo();
        }
    }

    rng.advanceFrames(1);

    // Forwards nature lock check loop
    for (x = count2; x <= 0; x--)
    {
        rng.advanceFrames(3);
        pid = getPIDForward();
        getCurrLock();
        if (nature != 500)
        {
            gender = pid & 255;
            if (gender < genderLower || gender > genderUpper || pid % 25 != nature)
                countForwardTwo();
        }
    }

    return pidOriginal == pid;
}

// Checks if seed is valid for second shadow with first shadow set
bool NatureLock::ivMethodFirstShadowSet(uint32_t seed)
{
    rng.setSeed(seed);
    rng.reverseFrames(11);

    // Build temp pid first to not waste time looping if first nl fails
    pidOriginal = getPIDReverse();

    // Backwards nature lock check
    gender = pidOriginal & 255;
    if (gender < lockInfo[0].getGenderLower() || gender > lockInfo[0].getGenderUpper() || pidOriginal % 25 != lockInfo[0].getNature())
        return false;

    // Backwards nature lock check loop
    for (x = 1; x < count; x++)
    {
        rng.reverseFrames(3);
        pid = getPIDReverse();
        getCurrLock();
        if (nature != 500)
        {
            gender = pid & 255;
            if (gender < genderLower || gender > genderUpper || pid % 25 != nature)
                countBackTwo();
        }
    }

    rng.advanceFrames(1);

    //Forwards nature lock check
    for (x = count2; x >= 0; x--)
    {
        rng.advanceFrames(3);
        pid = getPIDForward();
        getCurrLock();
        if (nature != 500)
        {
            gender = pid & 255;
            if (gender < genderLower || gender > genderUpper || pid % 25 != nature)
                countForwardTwo();
        }
    }

    return pidOriginal == pid;
}

// Checks if seed is valid for second shadow with first shadow unset and antishiny(aka Shiny Skip)
bool NatureLock::ivMethodFirstShadowShinySkip(uint32_t seed)
{
    rng.setSeed(seed);
    rng.reverseFrames(1);

    uint32_t psv, psvtemp;

    //Check how many advances from shiny skip and build initial pid for first nl
    psv = getPSVReverse();
    psvtemp = getPSVReverse();
    while (psv == psvtemp)
    {
        psvtemp = psv;
        psv = getPSVReverse();
    }

    rng.reverseFrames(10);
    pidOriginal = getPIDReverse();

    // Backwards nature lock check
    gender = pidOriginal & 255;
    if (gender < lockInfo[0].getGenderLower() || gender > lockInfo[0].getGenderUpper() || pidOriginal % 25 != lockInfo[0].getNature())
        return false;

    // Backwards nature lock check loop
    for (x = 1; x < count; x++)
    {
        rng.reverseFrames(3);
        pid = getPIDReverse();
        getCurrLock();
        if (nature != 500)
        {
            gender = pid & 255;
            if (gender < genderLower || gender > genderUpper || pid % 25 != nature)
                countBackTwo();
        }
    }

    rng.advanceFrames(1);

    // Forwards nature lock check loop
    for (x = count2; x >= 0; x--)
    {
        rng.advanceFrames(3);
        pid = getPIDForward();
        getCurrLock();
        if (nature != 500)
        {
            gender = pid & 255;
            if (gender < genderLower || gender > genderUpper || pid % 25 != nature)
                countForwardTwo();
        }
    }

    return pidOriginal == pid;
}

// Keeps rerolling PID forward by 2 until it finds a match for current lock
void NatureLock::countForwardTwo()
{
    pid = getPIDForward();
    gender = pid & 255;
    while (gender < genderLower || gender > genderUpper || pid % 25 != nature)
    {
        pid = getPIDForward();
        gender = pid & 255;
    }
}

// Keeps rerolling PID backwards by 2 until it finds a match for current lock
void NatureLock::countBackTwo()
{
    pid = getPIDReverse();
    gender = pid & 255;
    while (gender < genderLower || gender > genderUpper || pid % 25 != nature)
    {
        pid = getPIDReverse();
        gender = pid & 255;
    }
}

// Generates the next PID forwards
uint32_t NatureLock::getPIDForward()
{
    return (rng.next32Bit() & 0xFFFF0000) | rng.next16Bit();
}

// Generates the next PID backwards
uint32_t NatureLock::getPIDReverse()
{
    return rng.prev16Bit() | (rng.prev32Bit() & 0xFFFF0000);
}

// Generates the PSV of the next PID backwards
uint32_t NatureLock::getPSVReverse()
{
    return (rng.prev16Bit() ^ rng.prev16Bit()) >> 3;
}

// Quick sets the values of the current lock
void NatureLock::getCurrLock()
{
    nature = lockInfo[x].getNature();
    genderLower = lockInfo[x].getGenderLower();
    genderUpper = lockInfo[x].getGenderUpper();
}