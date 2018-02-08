#include "EventIsHitByPlayer.h"

const unsigned int EventIsHitByPlayer::eventId = utils::getGUID();

EventIsHitByPlayer::EventIsHitByPlayer(float health) : health(health), EventData(eventId)
{
}
