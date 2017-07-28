#include "NatureLock.hpp"

/* Each non-shadow before a shadow has to match
 * a specific gender/nature and these preset
 * values directly impact what spreads are available */

// Constructor for NatureLock
NatureLock::NatureLock(int lockNum)
{
    rng = LCRNG(0, rng_type::XD_RNG);
    natureLockSetup(lockNum);
    backCount = lockInfo.size();
    frontCount = backCount == 1 ? 0 : backCount - 2;
    x = 0;
    if (backCount == 1)
        getCurrLock();
}

// Changes which lock info is being used
void NatureLock::switchLock(int lockNum)
{
    natureLockSetup(lockNum);
    backCount = lockInfo.size();
    frontCount = backCount == 1 ? 0 : backCount - 2;
    x = 0;
    if (backCount == 1)
        getCurrLock();
}

// Sets up rest of nature lock data
void NatureLock::natureLockSetup(int lockNum)
{
    switch(lockNum)
    {
        case 0: // Altaria
            lockInfo = { LockInfo(24, 127, 255), LockInfo(0, 0, 126), LockInfo(12, 127, 255) };
            type = ShadowType::SecondShadow;
            break;
        case 1: // Arbok
            lockInfo =  { LockInfo(18, 0, 126), LockInfo(12, 0, 126), LockInfo(0, 0, 126), LockInfo(6, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 5: //Baltoy 2
            lockInfo =  { LockInfo(0, 127, 255), LockInfo(24, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 6: // Banette
            lockInfo =  { LockInfo(12, 0, 255), LockInfo(18, 0, 126), LockInfo(0, 0, 255) };
            type = ShadowType::SecondShadow;
            break;
        case 8: // Butterfree
            lockInfo = { LockInfo(0, 0, 126), LockInfo(6, 127, 255), LockInfo(12, 0, 190) };
            type = ShadowType::SecondShadow;
            break;
        case 10: // Chansey
            lockInfo = { LockInfo(24, 127, 255), LockInfo(6, 0, 126) };
            type = ShadowType::SecondShadow;
            break;
        case 11: // Delcatty
            lockInfo = { LockInfo(24, 127, 255), LockInfo(0, 127, 255), LockInfo(6, 0, 190) };
            type = ShadowType::FirstShadow;
            break;
        case 12: // Dodrio
            lockInfo =  { LockInfo(18, 0, 126) };
            type = ShadowType::SingleLock;
            break;
        case 13: // Dragonite
            lockInfo = { LockInfo(0, 127, 255), LockInfo(12, 0, 126), LockInfo(12, 0, 126), LockInfo(18, 127, 255), LockInfo(0, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 14: // Dugtrio
            lockInfo = { LockInfo(12, 127, 255), LockInfo(18, 127, 255), LockInfo(6, 0, 126), LockInfo(0, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 15: // Duskull
            lockInfo = { LockInfo(24, 127, 255), LockInfo(18, 0, 126), LockInfo(12, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 16: // Electabuzz
            lockInfo = { LockInfo(18, 0, 126), LockInfo(6, 0, 126), LockInfo(24, 63, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 18: // Farfetch'd  
            lockInfo = { LockInfo(24, 127, 255), LockInfo(0, 0, 126), LockInfo(12, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 19: // Golduck
            lockInfo = { LockInfo(18, 0, 126), LockInfo(6, 0, 126), LockInfo(24, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 20: // Grimer
            lockInfo = { LockInfo(18, 127, 255), LockInfo(12, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 21: // Growlithe
            lockInfo = { LockInfo(6, 0, 126), LockInfo(24, 127, 255) };
            type = ShadowType::SecondShadow;
            break;
        case 22: // Gulpin 3
        case 23: // Gulpin 1
            lockInfo = { LockInfo(6, 127, 255), LockInfo(12, 0, 126) };
            type = ShadowType::FirstShadow;
            break;
        case 24: // Gulpin 2
            lockInfo = { LockInfo(0, 0, 126), LockInfo(0, 0, 126), LockInfo(6, 127, 255), LockInfo(12, 0, 126) };
            type = ShadowType::FirstShadow;
            break;
        case 25: // Hitmonchan
            lockInfo = { LockInfo(18, 0, 126), LockInfo(6, 0, 126), LockInfo(24, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 26: // Hitmonlee
            lockInfo = { LockInfo(24, 0, 126), LockInfo(6, 0, 255), LockInfo(12, 0, 126), LockInfo(18, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 30: // Hypno
            lockInfo = { LockInfo(24, 127, 255), LockInfo(6, 0, 126), LockInfo(12, 0, 126), LockInfo(18, 0, 126) };
            type = ShadowType::SecondShadow;
            break;
        case 31: // Kangaskhan
            lockInfo = { LockInfo(12, 0, 255), LockInfo(18, 0, 126), LockInfo(0, 0, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 32: // Lapras
            lockInfo = { LockInfo(24, 127, 255), LockInfo(500, 500, 500), LockInfo(500, 500, 500), LockInfo(6, 0, 126) };
            type = ShadowType::SecondShadow;
            break;
        case 33: // Ledyba
            lockInfo = { LockInfo(0, 0, 126) };
            type = ShadowType::SingleLock;
            break;
        case 34: // Lickitung
            lockInfo = { LockInfo(6, 0, 255), LockInfo(24, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 36: // Lunatone
            lockInfo = { LockInfo(18, 127, 255), LockInfo(0, 0, 126) };
            type = ShadowType::FirstShadow;
            break;
        case 37: // Marcargo
            lockInfo = { LockInfo(12, 0, 126), LockInfo(6, 127, 255), LockInfo(24, 127, 255) };
            type = ShadowType::SecondShadow;
            break;
        case 38: // Magmar 
            lockInfo = { LockInfo(0, 0, 126), LockInfo(18, 191, 255), LockInfo(18, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 39: // Magneton
            lockInfo = { LockInfo(12, 0, 126), LockInfo(0, 127, 255), LockInfo(18, 0, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 40: // Makuhita
            lockInfo = { LockInfo(18, 0, 126), LockInfo(6, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 41: // Makuhita Colo
            lockInfo = { LockInfo(0, 0, 126), LockInfo(24, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 42: // Manectric
            lockInfo = { LockInfo(6, 0, 126) };
            type = ShadowType::SingleLock;
            break;
        case 44: // Mareep 1
            lockInfo = { LockInfo(12, 0, 126), LockInfo(24, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 45: // Mareep 2
            lockInfo = { LockInfo(0, 0, 255), LockInfo(12, 0, 126), LockInfo(24, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 46: // Marowak
            lockInfo = { LockInfo(24, 127, 255), LockInfo(500, 500, 500), LockInfo(500, 500, 500), LockInfo(6, 0, 126) };
            type = ShadowType::FirstShadow;
            break;
        case 47: // Mawile
            lockInfo = { LockInfo(18, 0, 126), LockInfo(6, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 48: // Meowth
            lockInfo =  { LockInfo(18, 0, 126), LockInfo(0, 0, 126), LockInfo(6, 63, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 50: // Mr. Mime
            lockInfo = { LockInfo(6, 0, 126), LockInfo(24, 127, 255), LockInfo(18, 127, 255), LockInfo(18, 127, 255) };
            type = ShadowType::SecondShadow;
            break;
        case 51: // Natu
            lockInfo = { LockInfo(0, 0, 126), LockInfo(24, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 52: // Nosepass
            lockInfo = { LockInfo(12, 0, 126), LockInfo(18, 127, 255), LockInfo(0, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 53: // Numel
            lockInfo = { LockInfo(24, 0, 126), LockInfo(0, 0, 255), LockInfo(6, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 54: // Paras
            lockInfo = { LockInfo(6, 0, 126), LockInfo(24, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 55: // Pidgeotto
            lockInfo = { LockInfo(18, 32, 255), LockInfo(12, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 56: // Pineco
            lockInfo = { LockInfo(6, 127, 255) };
            type = ShadowType::SingleLock;
            break;
        case 57: // Pinsir
            lockInfo = { LockInfo(0, 0, 126), LockInfo(18, 191, 255), LockInfo(18, 127, 255) };
            type = ShadowType::SecondShadow;
            break;
        case 58: // Poliwrath
            lockInfo = { LockInfo(6, 0, 126), LockInfo(24, 127, 255), LockInfo(18, 127, 255), LockInfo(18, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 59: // Poochyena
            lockInfo = { LockInfo(12, 0, 126) };
            type = ShadowType::SingleLock;
            break;
        case 60: // Primeape
            lockInfo = { LockInfo(24, 127, 255), LockInfo(6, 0, 126), LockInfo(12, 0, 126), LockInfo(18, 0, 126) };
            type = ShadowType::FirstShadow;
            break;
        case 61: // Ralts
            lockInfo = { LockInfo(18, 127, 255), LockInfo(6, 0, 126), LockInfo(0, 63, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 62: // Rapidash
            lockInfo = { LockInfo(12, 0, 126), LockInfo(6, 127, 255), LockInfo(24, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 63: // Raticate
            lockInfo = { LockInfo(18, 127, 255), LockInfo(500, 500, 500), LockInfo(18, 0, 126) };
            type = ShadowType::FirstShadow;
            break;
        case 65: // Roselia
            lockInfo = { LockInfo(18, 127, 255), LockInfo(6, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 66: // Sableye
            lockInfo = { LockInfo(18, 0, 126), LockInfo(6, 0, 126), LockInfo(24, 127, 255) };
            type = ShadowType::SecondShadow;
            break;
        case 67: // Salamence
            lockInfo = { LockInfo(6, 0, 126) };
            type = ShadowType::Salamence;
            break;
        case 68: // Scyther
            lockInfo = { LockInfo(24, 127, 255), LockInfo(6, 0, 126) };
            type = ShadowType::FirstShadow;
            break;
        case 70: // Seedot 1
            lockInfo = { LockInfo(12, 127, 255), LockInfo(0, 127, 255), LockInfo(18, 0, 126), LockInfo(24, 0, 126), LockInfo(6, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 71: // Seedot 2
            lockInfo = { LockInfo(6, 127, 255), LockInfo(0, 0, 126), LockInfo(0, 0, 126), LockInfo(24, 0, 126), LockInfo(6, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 72: // Seel
            lockInfo = { LockInfo(18, 0, 126), LockInfo(12, 127, 255), LockInfo(6, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 74: // Shroomish
            lockInfo = { LockInfo(0, 0, 126), LockInfo(24, 0, 126) };
            backCount = 2;
            type = ShadowType::FirstShadow;
            break;
        case 75: // Snorlax
            lockInfo =  { LockInfo(18, 0, 126), LockInfo(6, 0, 126), LockInfo(24, 63, 255) };
            type = ShadowType::SecondShadow;
            break;
        case 76: // Snorunt
            lockInfo = { LockInfo(6, 0, 126) };
            type = ShadowType::SingleLock;
            break;
        case 77: // Solrock
            lockInfo = { LockInfo(0, 0, 126), LockInfo(6, 127, 255), LockInfo(24, 0, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 78: // Spearow
            lockInfo = { LockInfo(6, 0, 126), LockInfo(18, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 79: // Spheal 3
        case 81: // Spheal 2
            lockInfo = { LockInfo(0, 0, 255), LockInfo(12, 0, 126), LockInfo(24, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 80: // Spheal 1
            lockInfo = { LockInfo(12, 0, 126), LockInfo(24, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 82: // Spinarak
            lockInfo = { LockInfo(6, 127, 255), LockInfo(12, 0, 126) };
            type = ShadowType::FirstShadow;
            break;
        case 83: // Starmie
            lockInfo = { LockInfo(18, 127, 255), LockInfo(500, 500, 500), LockInfo(0, 0, 126), LockInfo(6, 127, 255), LockInfo(24, 0, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 85: // Swinub
            lockInfo =  { LockInfo(0, 127, 255), LockInfo(18, 0, 126) };
            type = ShadowType::FirstShadow;
            break;
        case 86: // Tangela
            lockInfo = { LockInfo(0, 0, 126), LockInfo(6, 127, 255), LockInfo(12, 0, 190) };
            type = ShadowType::FirstShadow;
            break;
        case 90: // Venomoth
            lockInfo = { LockInfo(12, 127, 255), LockInfo(24, 0, 255), LockInfo(18, 0, 126) };
            type = ShadowType::FirstShadow;
            break;
        case 91: // Voltorb
            lockInfo = { LockInfo(12, 0, 126), LockInfo(12, 127, 255), LockInfo(0, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 92: // Vulpix
            lockInfo = { LockInfo(18, 127, 255), LockInfo(6, 0, 126), LockInfo(0, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 93: // Weepinbell
            lockInfo = { LockInfo(12, 127, 255), LockInfo(24, 0, 255), LockInfo(18, 0, 126) };
            type = ShadowType::SecondShadow;
            break;
        case 2: // Articuno
        case 3: // Baltoy 3
        case 4: // Baltoy 1
        case 7: // Beedrill
        case 9: // Carvanha
        case 17: // Exeggutor
        case 27: // Houndour 3
        case 28: // Houndour 1
        case 29: // To do houndour 2
        case 35: // Lugia
        case 43: // Mareep 3
        case 49: // Moltres
        case 64: // Rhydon
        case 69: // To do seedot 3
        case 73: // Shellder
        case 84: // Swellow
        case 87: // Tauros
        case 88: // Teddiursa
        case 89: // Togepi
        case 94: // Zangoose
        default: // Zapdos
            type = ShadowType::NoLock;
            break;
    }
}

// Returns what type the shadow is
ShadowType NatureLock::getType()
{
    return type;
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
    return (gender >= genderLower && gender <= genderUpper && pid % 25 == nature);
}

// Salamence is a special case of single nature lock and second shadow
// Checks if seed is valid for 1st shadow unset for Salamence
bool NatureLock::ivMethodSalamenceUnset(uint32_t seed)
{
    rng.setSeed(seed);
    rng.reverseFrames(8);

    // Build PID
    pid = getPIDReverse();

    // Backwards nature lock check
    gender = pid & 255;
    return (gender >= genderLower && gender <= genderUpper && pid % 25 == nature);
}

// Checks if seed is valid for 1st shadow set for Salamence
bool NatureLock::ivMethodSalamenceSet(uint32_t seed)
{
    rng.setSeed(seed);
    rng.reverseFrames(6);

    // Build PID
    pid = getPIDReverse();

    // Backwards nature lock check
    gender = pid & 255;
    return (gender >= genderLower && gender <= genderUpper && pid % 25 == nature);
}

// Checks if seed is valid for 1st shadow unset and antishiny(aka Shiny Skip) for Salamence
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

    rng.reverseFrames(5);
    pid = getPIDReverse();

    // Backwards nature lock check
    gender = pid & 255;
    return (gender > genderLower && gender < genderUpper && pid % 25 == nature);
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
    if (!(gender >= lockInfo[0].getGenderLower() && gender <= lockInfo[0].getGenderUpper() && pidOriginal % 25 == lockInfo[0].getNature()))
        return false;

    // Backwards nature lock check loop
    for (x = 1; x < backCount; x++)
    {
        rng.reverseFrames(3);
        pid = getPIDReverse();
        getCurrLock();
        if (nature != 500)
        {
            gender = pid & 255;
            if (!(gender >= genderLower && gender <= genderUpper && pid % 25 == nature))
                countBackTwo();
        }
    }

    rng.advanceFrames(1);

    // Forwards nature lock check loop
    for (x = frontCount; x >= 0; x--)
    {
        rng.advanceFrames(3);
        pid = getPIDForward();
        getCurrLock();
        if (nature != 500)
        {
            gender = pid & 255;
            if (!(gender >= genderLower && gender <= genderUpper && pid % 25 == nature))
                countForwardTwo();
        }
    }

    // Checks if first NL PID back from target matches
    return pidOriginal == pid;
}

// Checks if seed is valid for second shadow with first shadow unset
bool NatureLock::ivMethodFirstShadowUnset(uint32_t seed)
{
    rng.setSeed(seed);
    rng.reverseFrames(8);

    // Build temp pid first to not waste time looping if first nl fails
    pidOriginal = getPIDReverse();

    // Backwards nature lock check
    gender = pidOriginal & 255;
    if (!(gender >= lockInfo[0].getGenderLower() && gender <= lockInfo[0].getGenderUpper() && pidOriginal % 25 == lockInfo[0].getNature()))
        return false;
    
    // Backwards nature lock check loop
    for (x = 1; x < backCount; x++)
    {
        rng.reverseFrames(3);
        pid = getPIDReverse();
        getCurrLock();
        if (nature != 500)
        {
            gender = pid & 255;
            if (!(gender >= genderLower && gender <= genderUpper && pid % 25 == nature))
                countBackTwo();
        }
    }

    rng.advanceFrames(1);

    // Forwards nature lock check loop
    for (x = frontCount; x >= 0; x--)
    {
        rng.advanceFrames(3);
        pid = getPIDForward();
        getCurrLock();
        if (nature != 500)
        {
            gender = pid & 255;
            if (!(gender >= genderLower && gender <= genderUpper && pid % 25 == nature))
                countForwardTwo();
        }
    }

    // Checks if first NL PID back from target matches
    return pidOriginal == pid;
}

// Checks if seed is valid for second shadow with first shadow set
bool NatureLock::ivMethodFirstShadowSet(uint32_t seed)
{
    rng.setSeed(seed);
    rng.reverseFrames(6);

    // Build temp pid first to not waste time looping if first nl fails
    pidOriginal = getPIDReverse();

    // Backwards nature lock check
    gender = pidOriginal & 255;
    if (gender >= lockInfo[0].getGenderLower() && gender <= lockInfo[0].getGenderUpper() && pidOriginal % 25 == lockInfo[0].getNature())
        return false;

    // Backwards nature lock check loop
    for (x = 1; x < backCount; x++)
    {
        rng.reverseFrames(3);
        pid = getPIDReverse();
        getCurrLock();
        if (nature != 500)
        {
            gender = pid & 255;
            if (!(gender >= genderLower && gender <= genderUpper && pid % 25 == nature))
                countBackTwo();
        }
    }

    rng.advanceFrames(1);

    // Forwards nature lock check
    for (x = frontCount; x >= 0; x--)
    {
        rng.advanceFrames(3);
        pid = getPIDForward();
        getCurrLock();
        if (nature != 500)
        {
            gender = pid & 255;
            if (!(gender >= genderLower && gender <= genderUpper && pid % 25 == nature))
                countForwardTwo();
        }
    }
    
    // Checks if first NL PID back from target matches
    return pidOriginal == pid;
}

// Checks if seed is valid for second shadow with first shadow unset and antishiny(aka Shiny Skip)
bool NatureLock::ivMethodFirstShadowShinySkip(uint32_t seed)
{
    rng.setSeed(seed);
    rng.reverseFrames(1);

    uint32_t psv, psvtemp;

    // Check how many advances from shiny skip and build initial pid for first nl
    psv = getPSVReverse();
    psvtemp = getPSVReverse();
    while (psv == psvtemp)
    {
        psvtemp = psv;
        psv = getPSVReverse();
    }

    rng.reverseFrames(5);
    pidOriginal = getPIDReverse();

    // Backwards nature lock check
    gender = pidOriginal & 255;
    if (!(gender >= lockInfo[0].getGenderLower() && gender <= lockInfo[0].getGenderUpper() && pidOriginal % 25 == lockInfo[0].getNature()))
        return false;

    // Backwards nature lock check loop
    for (x = 1; x < backCount; x++)
    {
        rng.reverseFrames(3);
        pid = getPIDReverse();
        getCurrLock();
        if (nature != 500)
        {
            gender = pid & 255;
            if (!(gender >= genderLower && gender <= genderUpper && pid % 25 == nature))
                countBackTwo();
        }
    }

    rng.advanceFrames(1);

    // Forwards nature lock check loop
    for (x = frontCount; x >= 0; x--)
    {
        rng.advanceFrames(3);
        pid = getPIDForward();
        getCurrLock();
        if (nature != 500)
        {
            gender = pid & 255;
            if (!(gender >= genderLower && gender <= genderUpper && pid % 25 == nature))
                countForwardTwo();
        }
    }

    // Checks if first NL PID back from target matches
    return pidOriginal == pid;
}

// Keeps rerolling PID forward by 2 until it finds a match for current lock
void NatureLock::countForwardTwo()
{
    do
    {
        pid = getPIDForward();
        gender = pid & 255;
    }
    while (!(gender >= genderLower && gender <= genderUpper && pid % 25 == nature));
}

// Keeps rerolling PID backwards by 2 until it finds a match for current lock
void NatureLock::countBackTwo()
{
    do
    {
        pid = getPIDReverse();
        gender = pid & 255;
    }
    while (!(gender >= genderLower && gender <= genderUpper && pid % 25 == nature));
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

// Constructor for LockInfo
LockInfo::LockInfo(uint32_t nature, uint32_t genderLower, uint32_t genderUpper)
{
    this->nature = nature;
    this->genderLower = genderLower;
    this->genderUpper = genderUpper;
}

// Gets nature value
uint32_t LockInfo::getNature()
{
    return nature;
}

// Gets lower gender thresh value
uint32_t LockInfo::getGenderLower()
{
    return genderLower;
}

// Gets upper gender thresh value
uint32_t LockInfo::getGenderUpper()
{
    return genderUpper;
}