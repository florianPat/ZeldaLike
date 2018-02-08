#include "EventHitPlayer.h"
#include "Utils.h"

const unsigned int EventHitPlayer::eventId = utils::getGUID();

EventHitPlayer::EventHitPlayer(float health) : health(health), EventData(eventId)
{
}
