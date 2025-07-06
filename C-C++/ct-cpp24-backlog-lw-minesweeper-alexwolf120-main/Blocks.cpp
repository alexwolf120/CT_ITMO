#include "Blocks.h"

Blocks::Blocks() : sweeped(false), flaged(false), isMine(false), nearBy(0), questioned(false), status(Normal) {}
Blocks::~Blocks() {}
void Blocks::setStatus(Status _status)
{
    status = _status;
}

Blocks::Status Blocks::getStatus()
{
    return status;
}
void Blocks::setSweep(bool _sweep)
{
    sweeped = _sweep;
}

bool Blocks::getSweeped()
{
    return sweeped;
}

void Blocks::setFlag(bool _flag)
{
    flaged = _flag;
}

bool Blocks::getFlag()
{
    return flaged;
}

void Blocks::setNearBy(int _nearBy)
{
    nearBy = _nearBy;
}

int Blocks::getNearBy()
{
    return nearBy;
}

void Blocks::setMine(bool _isMine)
{
    isMine = _isMine;
}

bool Blocks::getMine()
{
    return isMine;
}

void Blocks::setLocked(bool _locked)
{
    locked = _locked;
}

bool Blocks::getLocked()
{
    return locked;
}

void Blocks::setQuestioned(bool _questioned)
{
    questioned = _questioned;
}

bool Blocks::getQuestioned()
{
    return questioned;
}
