#include "EventIsHitByPlayer.h"

const unsigned int EventIsHitByPlayer::eventId = utils::getGUID();

EventIsHitByPlayer::EventIsHitByPlayer(float health, const std::string& bodyId) : health(health), bodyId(bodyId), EventData(eventId)
{
}
