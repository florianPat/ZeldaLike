#include "EventGetPlayerPos.h"

const unsigned int EventGetPlayerPos::eventId = utils::getGUID();

EventGetPlayerPos::EventGetPlayerPos(const sf::Vector2f * pos) : pos(pos), EventData(eventId)
{
}
