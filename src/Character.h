#pragma once

#include "Precompiled.h"

#include "Object.h"

class Character : public Object
{
public:
	enum Sex
	{
		FEMALE	= 1,
		MALE	= 2
	};

	enum Race
	{
		DWARF		= 1,
		ELF			= 2,
		GNOME		= 3,
		HALF_ELF	= 4,
		HALF_orc	= 5,
		HALFLING	= 6,
		HUMAN		= 7
	};

	enum Class
	{
		BARBARIAN	= 1,
		BARD		= 2,
		CLERIC		= 3,
		DRUI		= 4,
		FIGHTER		= 5,
		MONK		= 6,
		PALADIN		= 7,
		RANGER		= 8,
		ROGUE		= 9,
		SORCERER	= 10,
		WIZARD		= 11
	};

	enum Alignment
	{
		LAWFUL_GOOD		= 1,
		LAWFUL_NEUTRAL	= 2,
		LAWFUL_EVIL		= 3,
		NEUTRAL_GOOD	= 4,
		NEUTRAL_		= 5,
		NEUTRAL_EVIL	= 6,
		CHAOTIC_GOOD	= 7,
		CHAOTIC_NEUTRAL	= 8,
		CHAOTIC_EVIL	= 9
	};

protected:
	unsigned short	mAge;
	unsigned short	mHeight;
	unsigned short	mWeight;

	unsigned char	mSex;
	unsigned char	mRace;
	unsigned char	mClass;
	unsigned char	mAlignment;

	unsigned char	mStrength;		// STR
	unsigned char	mDexterity;		// DEX
	unsigned char	mConstitution;	// CON
	unsigned char	mIntelligence;	// INT
	unsigned char	mWisdom;		// WIS
	unsigned char	mCharisma;		// CHA

public:
	Character(const std::string& name);

	bool update(float time);
};