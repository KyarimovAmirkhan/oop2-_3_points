#include "SpellCard.h"
#include <cmath>

SpellCard::SpellCard(const std::string& name, int cost, int range)
    : spellName(name), manaCost(cost), castRange(range) {}

std::string SpellCard::getName() const
{
    return spellName;
}

int SpellCard::getManaCost() const
{
    return manaCost;
}

int SpellCard::getCastRange() const
{
    return castRange;
}

bool SpellCard::isInRange(int casterX, int casterY, int targetX, int targetY) const
{
    int distance = std::abs(casterX - targetX) + std::abs(casterY - targetY);
    return distance <= castRange;
}